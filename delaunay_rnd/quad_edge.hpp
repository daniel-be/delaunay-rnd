#pragma once
#include <memory>
#include <array>
#include "edge.hpp"

class Edge;

class Quad_edge
{
public:
	static Edge* make_edge();
	static void delete_edge(Edge* edg);
	static void splice(Edge* const a, Edge* const b);
	Quad_edge();
	~Quad_edge();

private:
	std::array<Edge, 4> e;
};