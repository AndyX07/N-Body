#ifndef BARNES_HUT_TREE_H
#define BARNES_HUT_TREE_H

#include <vector>
#include <memory>
#include "body.h"

struct Node {
    glm::dvec2 center;
    double     size;
    double     mass;
    glm::dvec2 com;
    Body*      body;
    Node*      children[4];
    bool       isLeaf;

    void init(const glm::dvec2& c, double s) {
        center = c; size = s;
        mass = 0.0; com = { 0.0, 0.0 };
        body = nullptr; isLeaf = true;
        children[0] = children[1] = children[2] = children[3] = nullptr;
    }
};

// Chunked pool allocator for Node objects.
// Nodes are handed out from fixed-size chunks; chunks are never freed or moved,
// so every Node* remains valid until reset() is called.
// reset() reuses existing chunks — no heap allocation on subsequent frames.
struct NodePool {
    static constexpr size_t CHUNK = 2048;

    std::vector<std::unique_ptr<Node[]>> chunks;
    size_t used = 0;

    void reset() { used = 0; }

    Node* alloc(const glm::dvec2& center, double size) {
        size_t ci = used / CHUNK;
        size_t ni = used % CHUNK;
        if (ci >= chunks.size())
            chunks.emplace_back(new Node[CHUNK]);
        ++used;
        Node* n = &chunks[ci][ni];
        n->init(center, size);
        return n;
    }
};

class BarnesHutTree {
public:
    explicit BarnesHutTree(double theta);
    void build(const glm::dvec2& center, double size, std::vector<Body>& bodies);

    glm::dvec2 calculateForce(Body* body, double G) const;

private:
    double   theta;
    NodePool pool;
    Node*    root;

    void insertIntoChild(Node* node, Body* body);
    void insert(Node* node, Body* body);
    void computeMassDistribution(Node* node);
    glm::dvec2 calculateForce(const Node* node, Body* body, double G) const;
};

#endif
