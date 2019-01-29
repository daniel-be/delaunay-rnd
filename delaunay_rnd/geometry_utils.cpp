#include "geometry_utils.hpp"

double ccw(const Point& a, const Point& b, const Point& c)
{
	return (b.x[1] - a.x[1]) * (c.x[2] - a.x[2]) - (b.x[2] - a.x[2]) * (c.x[1] - a.x[1]);
}

bool right_of(const Point& p, Edge* edg)
{
	return ccw(*edg->org()->get_pos(), *edg->dest()->get_pos(), p) < 0;
}

bool left_of(const Point& p, Edge* edg)
{
	return ccw(*edg->org()->get_pos(), *edg->dest()->get_pos(), p) > 0;
}

double coplanar(const Point& p, const Plane& pla)
{
	Point np{
			pla.u.o.x[1] * pla.v.o.x[2] - pla.u.o.x[2] * pla.v.o.x[1],
			pla.u.o.x[2] * pla.v.o.x[0] - pla.u.o.x[0] * pla.v.o.x[2],
			pla.u.o.x[0] * pla.v.o.x[1] - pla.u.o.x[1] * pla.v.o.x[0]
	};
	Vector n{ np };
	return n.o.x[0] * p.x[0] + n.o.x[1] * p.x[1] + n.o.x[2] * p.x[2] - (n.o.x[0] * pla.p.o.x[0] + n.o.x[1] * pla.p.o.x[1] + n.o.x[2] * pla.p.o.x[2]);
}

bool in_circle(const Point& a, const Point& b, const Point& c, const Point& d)
{
	Point p_d{ d.x[1], d.x[2], d.x[1] * d.x[1] + d.x[2] * d.x[2] };
	Vector v0{ Point{ a.x[1], a.x[2], a.x[1] * a.x[1] + a.x[2] * a.x[2] } };
	Vector v1{ Point{ b.x[1], b.x[2], b.x[1] * b.x[1] + b.x[2] * b.x[2] } };
	Vector v2{ Point{ c.x[1], c.x[2], c.x[1] * c.x[1] + c.x[2] * c.x[2] } };
	Vector v0v1 = v1 - v0;
	Vector v0v2 = v2 - v0;
	return coplanar(p_d, Plane{ v0, v0v1, v0v2 }) < 0;
}
