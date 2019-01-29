#include "window.hpp"

Window::Window(int width, int height) : width(width), height(height), initialized(false), window(nullptr), renderer(nullptr)
{
	this->dlny = Delaunay_rnd{ Point{ 0, 0 }, Point{ 0, -1600 }, Point{ 1600, 0 } };
	this->init(width, height);
}

bool Window::is_initialized() const
{
	return this->initialized;
}

void Window::handle_events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			this->initialized = false;
			break;
		case SDL_MOUSEBUTTONUP:
			this->on_btn_mouse_release(event.button);
			break;
		}
	}
}

void Window::render() const
{
	SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(this->renderer);
	this->draw_delaunay();
	SDL_RenderPresent(this->renderer);
}

Window::~Window()
{
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

void Window::init(int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
		return;
	}

	this->window = SDL_CreateWindow("Convex Hull", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
	if (window == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
		return;
	}

	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create Renderer: %s", SDL_GetError());
		return;
	}

	this->initialized = true;
}

void Window::on_btn_mouse_release(SDL_MouseButtonEvent mouse_button_event)
{
	if (mouse_button_event.button == SDL_BUTTON_LEFT)
	{
		SDL_Point p;
		p.x = mouse_button_event.x;
		p.y = -mouse_button_event.y;
		this->dlny.insert_site(Point{ static_cast<double>(p.x), static_cast<double>(p.y) });
	}
}

void Window::draw_delaunay() const
{
	std::map<int, Edge*> edgs = this->dlny.get_edges();
	for (std::pair<int, Edge*> edg : edgs)
	{
		if (!(edg.first == 0 || edg.first == 8))
		{
			this->draw_line(*edg.second->org()->get_pos(), *edg.second->dest()->get_pos());
		}
	}
}

void Window::draw_line(const Point& s, const Point& e) const
{
	SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(this->renderer, s.x[1], -s.x[2], e.x[1], -e.x[2]);
}