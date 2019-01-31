#pragma once
#include <vector>
#include <map>
#include "geometry.hpp"
#include "edge.hpp"

class Delaunay_rnd
{
public:
	Delaunay_rnd();
	Delaunay_rnd(const Point& a, const Point& b, const Point& c, std::vector<Point>& pts);
	std::map<int, Edge*> get_edges() const;
	~Delaunay_rnd();

private:
	std::shared_ptr<Edge> start_edge;
	std::map<int, Edge*> edgs;

	void calc_delaunay(std::vector<Point>& pts);
	void insert_site(const Point& p);
	Edge* locate(const std::shared_ptr<Vertex> v) const;
	Edge* connect(Edge* a, Edge* b);
	Edge* make_edge();
	void delete_edge(Edge* edg);
	void swap(Edge* edg) const;
	std::vector<Point> convex_hull(std::vector<Point>& pts) const;
};