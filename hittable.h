#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class material;

class hit_record {
public:
    point3 p;
    vec3 normal;
    double t;
    shared_ptr<material> mat;
};

class hittable {
public:
    virtual ~hittable() = default;

    // Intersections outside of interval [tmin, tmax] considered invalid
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif