#ifndef RAY_H
#define RAY_H

#include "vec3.h"
/*
	A ray is defined by a function P(t)=A+tb where:
		- A is the ray origin
		- b is the ray direction
		- t is a real number pointing at the direction of the ray (+ infront of origin A, - behind origin A; from 0 to the point value t)
*/

class ray {
public:
	ray() {}
	ray(const point3& origin, const vec3& direction)
		: orig(origin), dir(direction)
	{}

	// Returns the origin point of the ray.
	point3 origin() const { return orig; }

	// Returns the direction vector of the ray.
	vec3 direction() const { return dir; }

	// Computes and returns a point along the ray at parameter t.
	point3 at(double t) const {
		return orig + t * dir;
	}


public:
	point3 orig;
	vec3 dir;
};

#endif