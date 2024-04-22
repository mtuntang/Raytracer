#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include <cmath>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;
using std::fabs;

// CONSTANTS
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// UTILITY FUNCTIONS
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Returns a random real in [0,1).
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

// Returns a random real in [min,max).
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

#endif