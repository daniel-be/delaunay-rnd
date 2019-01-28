#pragma once
#include <vector>
#include "geometry.hpp"
#include "edge.hpp"

class Delaunay_rnd
{
public:
	Delaunay_rnd(const std::vector<Point>& pts);
	~Delaunay_rnd();

private:
	const std::shared_ptr<Edge> start_edge;

	Edge* locate(const std::shared_ptr<Vertex> v) const;
	void insert_site(const std::shared_ptr<Vertex> v);
	Edge* connect(Edge* a, Edge* b);
	void delete_quad_edge(Edge* edg);
	void swap(Edge* edg);
};