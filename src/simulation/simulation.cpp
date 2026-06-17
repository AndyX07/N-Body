#include "simulation.h"
#include "constants.h"
#include <limits>
#include <latch>

Simulation::Simulation() : tree(Constants::BH_THETA), pool() {}

void Simulation::addBody(const Body& body) {
    bodies.push_back(body);
}

void Simulation::update(double dt) {
    Integrator::leapfrogKick(bodies, dt);
    Integrator::leapfrogDrift(bodies, dt);
    computeForces();
    Integrator::leapfrogKick(bodies, dt);
}

const std::vector<Body>& Simulation::getBodies() const {
    return bodies;
}

void Simulation::computeForces() {
    if (bodies.empty()) return;

    for (auto& body : bodies) {
        body.acceleration = { 0.0, 0.0 };
    }

    double x1 = std::numeric_limits<double>::max(),  x2 = std::numeric_limits<double>::lowest();
    double y1 = std::numeric_limits<double>::max(),  y2 = std::numeric_limits<double>::lowest();
    for (const auto& body : bodies) {
        if (body.position.x < x1) x1 = body.position.x;
        if (body.position.x > x2) x2 = body.position.x;
        if (body.position.y < y1) y1 = body.position.y;
        if (body.position.y > y2) y2 = body.position.y;
    }
    glm::dvec2 center((x1 + x2) / 2.0, (y1 + y2) / 2.0);
    double size = std::max({ x2 - x1, y2 - y1, 1.0 });

    tree.build(center, size, bodies);
	size_t num_threads = pool.getThreadCount();
	size_t chunk_size = (bodies.size() + num_threads - 1) / num_threads;
    std::latch work_done(num_threads);
    for (size_t i = 0; i < num_threads; i++) {
        size_t start = i * chunk_size;
        size_t end = std::min(start + chunk_size, bodies.size());
		pool.enqueue([this, start, end, &work_done] {
            for (size_t j = start; j < end; j++) {
                Body* body_ptr = &bodies[j];
                body_ptr->acceleration = tree.calculateForce(body_ptr, Constants::G) / body_ptr->mass;
            }
            work_done.count_down();
		});
    }
	work_done.wait();
}