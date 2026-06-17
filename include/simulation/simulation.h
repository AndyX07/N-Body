#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "body.h"
#include "integrator.h"
#include "BarnesHutTree.h"
#include "ThreadPool.h"

class Simulation {
public:
	Simulation();
	void addBody(const Body& body);
	void update(double dt);
	const std::vector<Body>& getBodies() const;
	void computeForces();
private:
	std::vector<Body> bodies;
	BarnesHutTree     tree;
	ThreadPool pool;
};

#endif