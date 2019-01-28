#pragma once
#include <memory>
#include "edge.hpp"
#include "geometry.hpp"

double ccw(const Point& a, const Point& b, const Point& c);
bool right_of(const Point& p, Edge* edg);
bool left_of(const Point& p, Edge* edg);
double coplanar(const Point& p, const Plane& pla);
double in_circle(const Point& a, const Point& b, const Point& c, const Point& d);
