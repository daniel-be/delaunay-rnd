#include "delaunay_rnd.hpp"
#include "geometry_utils.hpp"
#include "quad_edge.hpp"

Delaunay_rnd::Delaunay_rnd(const std::vector<Point>& pts)
{
	for (Point p : pts)
	{
		std::shared_ptr<Vertex> v = std::make_shared<Vertex>(std::make_shared<Point>(p));
		this->insert_site(v);
	}
}

Delaunay_rnd::~Delaunay_rnd() { }

Edge* Delaunay_rnd::locate(const std::shared_ptr<Vertex> v) const
{
	Edge* edg = this->start_edge.get();
	while (true)
	{
		if (v == edg->org() || v == edg->dest()) return edg;
		else if (!right_of(*v->get_pos(), edg->o_next())) edg = edg->o_next();
		else if (!right_of(*v->get_pos(), edg->d_prev())) edg = edg->d_prev();
		else return edg;
	}
}

void Delaunay_rnd::insert_site(const std::shared_ptr<Vertex> v)
{

}

Edge* Delaunay_rnd::connect(Edge* a, Edge* b)
{
	Edge* edg = Quad_edge::make_edge();
	edg->set_endpoints(a->dest(), b->org());
	Quad_edge::splice(edg, a->l_next());
	Quad_edge::splice(edg->sym(), b);
	return edg;
}

void Delaunay_rnd::delete_quad_edge(Edge* edg)
{
	Quad_edge::splice(edg, edg->o_prev());
	Quad_edge::splice(edg->sym(), edg->sym()->o_prev());
	delete &edg->get_quad_edge();
}

void Delaunay_rnd::swap(Edge* edg)
{
	Edge* a = edg->o_prev();
	Edge* b = edg->sym()->o_prev();
	Quad_edge::splice(edg, a);
	Quad_edge::splice(edg->sym(), b);
	Quad_edge::splice(edg, a->l_next());
	Quad_edge::splice(edg->sym(), b->l_next());
	edg->set_endpoints(a->dest(), b->dest());
}