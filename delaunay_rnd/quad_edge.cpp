#include "quad_edge.hpp"

Edge* Quad_edge::make_edge()
{
	Quad_edge* qe = new Quad_edge;
	return &qe->e[0];
}

void Quad_edge::delete_edge(Edge* edg)
{
	delete edg->get_quad_edge().get();
}

void Quad_edge::splice(Edge* const a, Edge* const b)
{
	Edge* alpha = a->o_next()->rot();
	Edge* beta = b->o_next()->rot();

	Edge* edg0 = a->o_next();
	Edge* edg1 = b->o_next();
	Edge* edg2 = alpha->o_next();
	Edge* edg3 = beta->o_next();

	a->set_next(edg1);
	b->set_next(edg0);
	alpha->set_next(edg3);
	beta->set_next(edg2);
}

Quad_edge::Quad_edge() 
{ 
	this->e[0].set_r(0);
	this->e[1].set_r(1);
	this->e[2].set_r(2);
	this->e[3].set_r(3);
	this->e[0].set_next(&e[0]);
	this->e[1].set_next(&e[3]);
	this->e[2].set_next(&e[2]);
	this->e[3].set_next(&e[1]);
	std::shared_ptr<Quad_edge> qe = std::shared_ptr<Quad_edge>(this);
	this->e[0].set_quad_edge(qe);
	this->e[1].set_quad_edge(qe);
	this->e[2].set_quad_edge(qe);
	this->e[3].set_quad_edge(qe);
}

Quad_edge::~Quad_edge() { }
