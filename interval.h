#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>

// Define positive and negative infinity for convenience
constexpr double POS_INFINITY = std::numeric_limits<double>::infinity();
constexpr double NEG_INFINITY = -std::numeric_limits<double>::infinity();

class interval {
public:
    double min, max;

    interval() : min(POS_INFINITY), max(NEG_INFINITY) {} // This assignment means that it will always be invalid/empty
    interval(double _min, double _max) : min(_min), max(_max) {}

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x&& x < max;
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const interval empty;
    static const interval universe;
};

const interval interval::empty(POS_INFINITY, NEG_INFINITY);
const interval interval::universe(NEG_INFINITY, POS_INFINITY);

#endif
