#pragma once
#include <memory>
#include "geometry.hpp"

class Vertex
{
public:
	Vertex();
	Vertex(const std::shared_ptr<Point> pos);
	const std::shared_ptr<Point> get_pos() const;
	void set_pos(const std::shared_ptr<Point> pos);
	~Vertex();

private:
	std::shared_ptr<Point> pos;
};
