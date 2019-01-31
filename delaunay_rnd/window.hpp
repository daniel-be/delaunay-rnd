#pragma once
#include <vector>
#include <SDL.h>
#include "delaunay_rnd.hpp"

class Window
{
public:
	Window(int width, int height);
	bool is_initialized() const;
	void handle_events();
	void render();
	~Window();

private:
	int width, height;
	bool initialized;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::vector<Point> pts;

	void init(int width, int height);
	void on_btn_mouse_release(SDL_MouseButtonEvent mouse_button_event);
	void draw_delaunay();
	void draw_line(const Point& s, const Point& e) const;
};