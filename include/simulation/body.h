#ifndef BODY_H
#define BODY_H

#include <glm.hpp>

struct Body {
	double mass;
	double radius;
	glm::dvec2 position;
	glm::dvec2 velocity;
	glm::dvec2 acceleration;
	glm::vec3 color;
};

#endif