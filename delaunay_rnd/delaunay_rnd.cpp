#include "delaunay_rnd.hpp"
#include "geometry_utils.hpp"
#include "quad_edge.hpp"

Delaunay_rnd::Delaunay_rnd(): start_edge(nullptr) { }

Delaunay_rnd::Delaunay_rnd(const Point& a, const Point& b, const Point& c, std::vector<Point> pts)
{
	if (pts.size() < 3) return;
	/*std::shared_ptr<Point> p0 = std::make_shared<Point>(a);
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
	Quad_edge::splice(p2p0->sym(), p0p1);*/
	std::vector<Point> ch = this->convex_hull(pts);
	std::shared_ptr<Point> p0 = std::make_shared<Point>(ch[0]);
	std::shared_ptr<Vertex> v0 = std::make_shared<Vertex>(p0);
	std::shared_ptr<Point> pn = std::make_shared<Point>(ch[1]);
	std::shared_ptr<Vertex> vn = std::make_shared<Vertex>(pn);
	Edge* edg_first = this->make_edge();
	edg_first->set_endpoints(v0, vn);
	Edge* edg = edg_first;
	for (int i = 2; i < ch.size(); i++)
	{
		std::shared_ptr<Point> pnn = std::make_shared<Point>(ch[i]);
		std::shared_ptr<Vertex> vnn = std::make_shared<Vertex>(pnn);
		Edge* edgn = this->make_edge();
		edgn->set_endpoints(vn, vnn);
		Quad_edge::splice(edg->sym(), edgn);
		vn = vnn;
		edg = edgn;
	}
	Edge* edg_last = this->make_edge();
	edg_last->set_endpoints(vn, v0);
	Quad_edge::splice(edg->sym(), edg_last);
	Quad_edge::splice(edg_last->sym(), edg_first);
	
	this->start_edge = std::shared_ptr<Edge>(edg_first);
	this->calc_delaunay(pts);
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

void Delaunay_rnd::calc_delaunay(std::vector<Point>& pts)
{
	
	for (Point p : pts)
	{
		this->insert_site(p);
	}

	/*std::vector<Point> ch_pts;
	Edge* edg = this->start_edge.get();
	std::shared_ptr<Vertex> first = edg->org();
	do
	{
		Edge* ch_edg = edg->o_next();
		do
		{
			ch_pts.push_back(*ch_edg->dest()->get_pos());
			this->edgs.erase(ch_edg->get_id());
			this->edgs.erase(ch_edg->sym()->get_id());
			ch_edg = ch_edg->o_next();
		} while (ch_edg != edg->o_prev());
		this->edgs.erase(edg->get_id());
		this->edgs.erase(edg->sym()->get_id());
		edg = edg->r_prev();
	} while (edg->org() != first);

	std::vector<Point> ch = this->convex_hull(ch_pts);
	for (int i = 0; i < ch.size(); i++)
	{
		Edge* edg = this->make_edge();
		std::shared_ptr<Vertex> va;
		std::shared_ptr<Vertex> vb;
		if (i < ch.size() - 1)
		{
			va = std::make_shared<Vertex>(std::make_shared<Point>(ch[i]));
			vb = std::make_shared<Vertex>(std::make_shared<Point>(ch[i + 1]));
		}
		else
		{
			va = std::make_shared<Vertex>(std::make_shared<Point>(ch[i]));
			vb = std::make_shared<Vertex>(std::make_shared<Point>(ch[0]));
		}
		edg->set_endpoints(va, vb);
	}*/
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

bool del(Point p) 
{ 
	Point comp{ -1, -1 };
	return p == comp;
}

std::vector<Point> Delaunay_rnd::convex_hull(std::vector<Point>& pts) const
{
	std::vector<Point> ret;
	if (pts.empty()) return ret;

	int p1 = 0;
	for (size_t i = 1; i < pts.size(); i++)
	{
		if (pts[i] < pts[p1])
		{
			p1 = i;
		}
	}

	int p = p1;
	int q = 0;
	do
	{
		ret.push_back(pts[p]);

		q = (p + 1) % pts.size();
		for (size_t i = 0; i < pts.size(); i++)
		{
			if (ccw(pts[p], pts[i], pts[q]) > 0)
			{
				q = i;
			}
		}

		p = q;
	} while (p != p1);

	for (int i = 0; i < ret.size(); i++)
	{
		for (int j = 0; j < pts.size(); j++)
		{
			if (ret[i] == pts[j]) pts.erase(pts.begin() + j);
		}
	}

	return ret;
}