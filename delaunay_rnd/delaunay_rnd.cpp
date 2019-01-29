#include "delaunay_rnd.hpp"
#include "geometry_utils.hpp"
#include "quad_edge.hpp"

Delaunay_rnd::Delaunay_rnd(): start_edge(nullptr) { }

Delaunay_rnd::Delaunay_rnd(const Point& a, const Point& b, const Point& c)
{
	std::shared_ptr<Point> p0 = std::make_shared<Point>(a);
	std::shared_ptr<Point> p1 = std::make_shared<Point>(b);
	std::shared_ptr<Point> p2 = std::make_shared<Point>(c);
	std::shared_ptr<Vertex> v0 = std::make_shared<Vertex>(p0);
	std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>(p1);
	std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>(p2);
	Edge* p0p1 = this->make_edge();
	p0p1->set_endpoints(v0, v1);
	Edge* p1p2 = this->make_edge();
	Quad_edge::splice(p0p1->sym(), p1p2);
	p1p2->set_endpoints(v1, v2);
	Edge* p2p0 = this->make_edge();
	Quad_edge::splice(p1p2->sym(), p2p0);
	p2p0->set_endpoints(v2, v0);
	Quad_edge::splice(p2p0->sym(), p0p1);
	this->start_edge = std::shared_ptr<Edge>(p0p1);
}

void Delaunay_rnd::insert_site(const Point& p)
{
	std::shared_ptr<Point> pv = std::make_shared<Point>(p);
	std::shared_ptr<Vertex> v = std::make_shared<Vertex>(pv);
	Edge* edg = this->locate(v);
	if (*edg->org()->get_pos() == *v->get_pos() || *edg->dest()->get_pos() == *v->get_pos()) return;
	else if (!ccw(*edg->org()->get_pos(), *edg->dest()->get_pos(), *v->get_pos()))
	{
		Edge* t = edg->o_prev();
		this->delete_edge(edg);
		edg = t;
	}

	Edge* base = this->make_edge();
	std::shared_ptr<Vertex> first = edg->org();
	std::shared_ptr<Vertex> last = edg->org();
	base->set_endpoints(first, v);
	Quad_edge::splice(base, edg);
	do
	{
		base = this->connect(edg, base->sym());
		edg = base->o_prev();
		std::shared_ptr<Vertex> dest = edg->dest();
	} while (edg->dest() != first);

	edg = base->o_prev();
	do
	{
		Edge* t = edg->o_prev();
		Point t_dest = *t->dest()->get_pos();
		Point e_org = *edg->org()->get_pos();
		Point e_dest = *edg->dest()->get_pos();
		Point vp = *v->get_pos();
		if (right_of(*t->dest()->get_pos(), edg) && in_circle(*edg->org()->get_pos(), *t->dest()->get_pos(), *edg->dest()->get_pos(), *v->get_pos()))
		{
			this->swap(edg);
			edg = t;
		}
		else if (edg->org() == first) return;
		else edg = edg->o_next()->l_prev();
	} while (true);
}

std::shared_ptr<Edge> Delaunay_rnd::get_start_edge() const
{
	return this->start_edge;
}

std::map<int, Edge*> Delaunay_rnd::get_edges() const
{
	return this->edgs;
}

Delaunay_rnd::~Delaunay_rnd() { }

Edge* Delaunay_rnd::locate(const std::shared_ptr<Vertex> v) const
{
	Edge* edg = this->start_edge.get();
	while (true)
	{
		if (*v->get_pos() == *edg->org()->get_pos() || *v->get_pos() == *edg->dest()->get_pos()) return edg;
		else if (right_of(*v->get_pos(), edg)) edg = edg->sym();
		else if (!right_of(*v->get_pos(), edg->o_next())) edg = edg->o_next();
		else if (!right_of(*v->get_pos(), edg->d_prev())) edg = edg->d_prev();
		else return edg;
	}
}

Edge* Delaunay_rnd::connect(Edge* a, Edge* b)
{
	Edge* edg = this->make_edge();
	edg->set_endpoints(a->dest(), b->org());
	Quad_edge::splice(edg, a->l_next());
	Quad_edge::splice(edg->sym(), b);
	return edg;
}

Edge* Delaunay_rnd::make_edge()
{
	Edge* edg = Quad_edge::make_edge();
	this->edgs[edg->get_id()] = edg;
	return edg;
}

void Delaunay_rnd::delete_edge(Edge* edg)
{
	Quad_edge::splice(edg, edg->o_prev());
	Quad_edge::splice(edg->sym(), edg->sym()->o_prev());
	this->edgs.erase(edg->get_id());
	Quad_edge::delete_edge(edg);
}

void Delaunay_rnd::swap(Edge* edg) const
{
	Edge* a = edg->o_prev();
	Edge* b = edg->sym()->o_prev();
	Quad_edge::splice(edg, a);
	Quad_edge::splice(edg->sym(), b);
	Quad_edge::splice(edg, a->l_next());
	Quad_edge::splice(edg->sym(), b->l_next());
	edg->set_endpoints(a->dest(), b->dest());
}