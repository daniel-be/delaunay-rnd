#include "pch.h"
#include "../delaunay_rnd/geometry.cpp"
#include "../delaunay_rnd/vertex.cpp"
#include "../delaunay_rnd/edge.cpp"
#include "../delaunay_rnd/quad_edge.cpp"
#include "../delaunay_rnd/geometry_utils.cpp"

TEST(edge, EdgeAlgebra) 
{
	Edge* e = Quad_edge::make_edge();
	EXPECT_EQ(e, e->rot()->rot()->rot()->rot());
	EXPECT_NE(e, e->rot()->rot());
	EXPECT_EQ(e, e->sym()->sym());
	EXPECT_EQ(e->dest(), e->sym()->org());
	EXPECT_EQ(e->o_next(), e->rot()->l_next()->inv_rot());
	EXPECT_EQ(e->r_next(), e->inv_rot()->r_prev()->inv_rot());
}

TEST(edge, OrgDest)
{
	Edge* e0 = Quad_edge::make_edge();
	Edge* e1 = Quad_edge::make_edge();
	std::shared_ptr<Point> p0 = std::make_shared<Point>(1, 1);
	std::shared_ptr<Point> p1 = std::make_shared<Point>(2, 2);
	std::shared_ptr<Point> p2 = std::make_shared<Point>(0, 1);
	std::shared_ptr<Vertex> org0 = std::make_shared<Vertex>(p0);
	std::shared_ptr<Vertex> dest0 = std::make_shared<Vertex>(p1);
	std::shared_ptr<Vertex> org1 = dest0;
	std::shared_ptr<Vertex> dest1 = std::make_shared<Vertex>(p2);
	e0->set_endpoints(org0, dest0);
	e1->set_endpoints(org1, dest1);
	EXPECT_EQ(e0->dest(), e1->org());
	EXPECT_EQ(e0->dest(), e1->sym()->dest());
	EXPECT_EQ(e0->sym()->org(), e1->org());
	EXPECT_EQ(e0->sym()->rot()->l_next()->rot()->dest(), e0->dest());
	EXPECT_EQ(e0->sym()->get_quad_edge(), e0->get_quad_edge());
	EXPECT_NE(e0->get_quad_edge(), e1->get_quad_edge());
}

TEST(geometry, VectorArithmetic)
{
	Vector v0{ Point { 4, 7, 9 } };
	Vector v1{ Point { 2, 1, 0 } };
	Vector v2{ Point { 2, 6, 9 } };
	bool eq = v2 == (v0 - v1);
	EXPECT_TRUE(eq);
	eq = v0 == v1;
	EXPECT_FALSE(eq);
}

TEST(geometry_utils, CCW)
{
	EXPECT_EQ(0, ccw(Point(0, 0), Point(1, 1), Point(2, 2)));
	EXPECT_LT(ccw(Point(0, 0), Point(1, 1), Point(2, 1)), 0);
	EXPECT_GT(ccw(Point(0, 0), Point(1, 1), Point(2, 3)), 0);
}

TEST(geometry_utils, RightLeftOf)
{
	Edge* e = Quad_edge::make_edge();
	std::shared_ptr<Point> p0 = std::make_shared<Point>(0, 0);
	std::shared_ptr<Point> p1 = std::make_shared<Point>(5, 5);
	e->set_endpoints(std::make_shared<Vertex>(p0), std::make_shared<Vertex>(p1));
	EXPECT_TRUE(right_of(Point(2, 0), e));
	EXPECT_FALSE(right_of(Point(0, 2), e));
	EXPECT_TRUE(left_of(Point(0, 2), e));
	EXPECT_FALSE(left_of(Point(2, 0), e));
}

TEST(geometry_utils, InCircle)
{
	EXPECT_EQ(0, in_circle(Point(3, 0), Point(6, 3), Point(3, 6), Point(0, 3)));
	EXPECT_LT(in_circle(Point(3, 0), Point(6, 3), Point(3, 6), Point(3, 3)), 0);
	EXPECT_LT(in_circle(Point(3, 0), Point(6, 3), Point(3, 6), Point(2, 2)), 0);
	EXPECT_LT(in_circle(Point(3, 0), Point(6, 3), Point(3, 6), Point(3, 1)), 0);
	EXPECT_LT(in_circle(Point(3, 0), Point(6, 3), Point(3, 6), Point(1, 3)), 0);
	EXPECT_LT(in_circle(Point(0, 0), Point(800, 0), Point(800, 800), Point(0, 799)), 0);
	EXPECT_GT(in_circle(Point(3, 0), Point(6, 3), Point(3, 6), Point(0, 0)), 0);
	EXPECT_GT(in_circle(Point(3, 0), Point(6, 3), Point(3, 6), Point(10, 10)), 0);
	EXPECT_GT(in_circle(Point(3, 0), Point(6, 3), Point(3, 6), Point(7, 3)), 0);
	EXPECT_GT(in_circle(Point(3, 0), Point(6, 3), Point(3, 6), Point(40, 40)), 0);
	EXPECT_GT(in_circle(Point(3, 0), Point(6, 3), Point(3, 6), Point(800, 800)), 0);
	EXPECT_GT(in_circle(Point(0, 0), Point(800, 0), Point(800, 800), Point(0, 801)), 0);
}