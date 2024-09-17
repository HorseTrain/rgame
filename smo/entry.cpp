#include "rgame/render_window.h"

int main()
{
	render_window working_window;

	render_window::create(&working_window, nullptr, 500, 500, 60, "MARIO");

	render_window::run(&working_window, nullptr);

	render_window::destroy(&working_window);
}