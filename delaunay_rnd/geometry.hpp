#pragma once
#include <array>

struct Point
{
	std::array<double, 3> x;

	Point();
	Point(double x, double y);
	Point(double x, double y, double z);
	~Point();
};

struct Vector
{
	Point o;

	Vector();
	Vector(Point p);
	bool operator==(const Vector& v) const;
	Vector& operator-(const Vector& v) const;
	~Vector();
};

struct Plane
{
	Vector p, u, v;

	Plane();
	Plane(Vector p, Vector u, Vector v);
	~Plane();
};