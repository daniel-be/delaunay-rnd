#include "window.hpp"

int main(int argc, char *args[])
{
	Window w{ 801, 801 };
	while (w.is_initialized())
	{
		w.handle_events();
		w.render();
	}
	return 0;
}