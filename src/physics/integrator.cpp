#include "integrator.h"

void Integrator::leapfrogKick(std::vector<Body>& bodies, double dt) {
	for (auto& body : bodies) {
		body.velocity += body.acceleration * (dt * 0.5);
	}
}

void Integrator::leapfrogDrift(std::vector<Body>& bodies, double dt) {
	for (auto& body : bodies) {
		body.position += body.velocity * dt;
	}
}