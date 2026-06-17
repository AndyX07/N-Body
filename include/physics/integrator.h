#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "body.h"
#include <vector>

namespace Integrator {
	void leapfrogKick(std::vector<Body>& bodies, double dt);
	void leapfrogDrift(std::vector<Body>& bodies, double dt);
}

#endif