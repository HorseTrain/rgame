#include <iostream>

#include "rgame/render_window.h"

void loop(render_window* window, void* arguments)
{
	glBegin(GL_TRIANGLES);

	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);

	glEnd();
}

int main()
{
	render_window test_window;

	render_window::create(&test_window, loop, 500, 500, 60, "minecraft");
	render_window::run(&test_window,nullptr);

	render_window::destroy(&test_window);
}