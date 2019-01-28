#pragma once
#include <vector>
#include <SDL.h>

class Window
{
public:
	Window(int width, int height);
	bool is_initialized() const;
	void handle_events();
	void render() const;
	~Window();

private:
	int width, height;
	bool initialized;
	SDL_Window* window;
	SDL_Renderer* renderer;

	void init(int width, int height);
	void on_btn_mouse_release(SDL_MouseButtonEvent mouse_button_event);
};