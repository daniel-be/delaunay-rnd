#pragma once
#include <memory>
#include "vertex.hpp"

class Quad_edge;

class Edge
{
public:
	Edge();
	int get_id() const;
	Edge* rot() const;
	Edge* inv_rot() const;
	Edge* sym() const;
	Edge* o_next() const;
	Edge* o_prev() const;
	Edge* d_next() const;
	Edge* d_prev() const;
	Edge* l_next() const;
	Edge* l_prev() const;
	Edge* r_next() const;
	Edge* r_prev() const;
	std::shared_ptr<Vertex> org() const;
	std::shared_ptr<Vertex> dest() const;
	void set_r(const int r);
	void set_endpoints(const std::shared_ptr<Vertex> org, const std::shared_ptr<Vertex> dest);
	void set_next(Edge* const edg);
	std::shared_ptr<Quad_edge> get_quad_edge() const;
	void set_quad_edge(std::shared_ptr<Quad_edge> quad_edge);
	~Edge();

private:
	static int id_cnt;
	int id;
	int r; // Index in quad edge structure.
	Edge* next; // Next CCW edge.
	std::shared_ptr<Vertex> data; // Origin vertex.
	std::shared_ptr<Quad_edge> quad_edge;
};