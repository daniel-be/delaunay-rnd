#include "vertex.hpp"

Vertex::Vertex() { }

Vertex::Vertex(const std::shared_ptr<Point> pos) : pos(pos) { }

const std::shared_ptr<Point> Vertex::get_pos() const
{
	return this->pos;
}

void Vertex::set_pos(const std::shared_ptr<Point> pos)
{
	this->pos = pos;
}

Vertex::~Vertex() { }