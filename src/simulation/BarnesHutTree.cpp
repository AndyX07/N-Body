#include "BarnesHutTree.h"
#include "constants.h"
#include <cmath>

BarnesHutTree::BarnesHutTree(double theta)
    : theta(theta), root(nullptr) {}

static int getQuadrant(const Node* node, const glm::dvec2& pos) {
    if (node->center.x >= pos.x) {
        return (node->center.y >= pos.y) ? 0 : 2;
    }
    else {
        return (node->center.y >= pos.y) ? 1 : 3;
    }
}

static glm::dvec2 getChildCenter(const Node* node, int q) {
    double offset = node->size / 4.0;
    switch (q) {
        case 0: return node->center + glm::dvec2(-offset, -offset);
        case 1: return node->center + glm::dvec2( offset, -offset);
        case 2: return node->center + glm::dvec2(-offset,  offset);
        case 3: return node->center + glm::dvec2( offset,  offset);
        default: return node->center;
    }
}

void BarnesHutTree::insertIntoChild(Node* node, Body* body) {
    int q = getQuadrant(node, body->position);
    if (node->children[q] == nullptr)
        node->children[q] = pool.alloc(getChildCenter(node, q), node->size / 2.0);
    insert(node->children[q], body);
}

void BarnesHutTree::insert(Node* node, Body* body) {
    if (node->isLeaf) {
        if (node->body == nullptr) {
            node->body = body;
            return;
        }
        Body* existing = node->body;
        node->body = nullptr;
        node->isLeaf = false;
        if (existing->position == body->position)
            body->position.x += Constants::BH_POS_JITTER;
        insertIntoChild(node, existing);
    }
    insertIntoChild(node, body);
}

void BarnesHutTree::computeMassDistribution(Node* node) {
    if (node == nullptr) return;
    if (node->isLeaf) {
        if (node->body != nullptr) {
            node->mass = node->body->mass;
            node->com = node->body->position;
        }
        return;
    }
    node->mass = 0.0;
    node->com  = { 0.0, 0.0 };
    for (int i = 0; i < 4; i++) {
        computeMassDistribution(node->children[i]);
        if (node->children[i] != nullptr && node->children[i]->mass > 0.0) {
            node->mass += node->children[i]->mass;
            node->com += node->children[i]->com * node->children[i]->mass;
        }
    }
    if (node->mass > 0.0) node->com /= node->mass;
}

void BarnesHutTree::build(const glm::dvec2& center, double size, std::vector<Body>& bodies) {
    pool.reset();
    root = pool.alloc(center, size);
    for (auto& body : bodies) insert(root, &body);
    computeMassDistribution(root);
}

glm::dvec2 BarnesHutTree::calculateForce(Body* body, double G) const {
    return calculateForce(root, body, G);
}

glm::dvec2 BarnesHutTree::calculateForce(const Node* node, Body* body, double G) const {
    if (node == nullptr || node->mass == 0.0) return { 0.0, 0.0 };
    if (node->isLeaf && node->body == body) return { 0.0, 0.0 };

    glm::dvec2 dir = node->com - body->position;
    double distSq = glm::dot(dir, dir) + Constants::BH_SOFTENING_SQ;
    double dist = std::sqrt(distSq);

    if (node->isLeaf || (node->size / dist) < theta) {
        return (G * body->mass * node->mass / (distSq * dist)) * dir;
    }

    glm::dvec2 total{ 0.0, 0.0 };
    for (int i = 0; i < 4; i++) {
        total += calculateForce(node->children[i], body, G);
    }
    return total;
}