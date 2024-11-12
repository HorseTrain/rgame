#include "maze_generator.h"
#include "maze.h"
#include <vector>

glm::ivec2 vector_directions[] =
{
	{0, -1},
	{1, 0},
	{0, 1},
	{-1, 0}
};

struct depth_search_bot_manager
{
	maze_generator* maze_generator_context;

	struct bot
	{
		glm::ivec2 current_position;

		//Debugging
		static void advance_recursive(bot* bot_context, depth_search_bot_manager* depth_search_bot_manager_context)
		{
			maze* maze_context = depth_search_bot_manager_context->maze_generator_context->maze_context;

			maze_cell* working_cell = maze::get_cell(maze_context, bot_context->current_position.x, bot_context->current_position.y);

			working_cell->visited = true;

			while (true)
			{
				if (maze_cell::is_dead_end(working_cell))
				{
					return;
				}

				maze_cell* target_location;
				int new_cell_index;

				while (true)
				{
					new_cell_index = rand() % 4;

					target_location = working_cell->neighbors[new_cell_index];

					if (target_location == nullptr)
						continue;

					if (target_location->visited)
						continue;

					break;
				}

				working_cell->walls[new_cell_index]->is_open = true;

				bot child_bot = { {target_location->x, target_location->y} };

				bot::advance_recursive(&child_bot, depth_search_bot_manager_context);
			}
		}
	};

	struct bot_stack
	{
		std::vector<bot>	bots;
		int					current_top;

		bot_stack()
		{
			current_top = -1;
		}

		void push(bot bot_data)
		{
			while (bots.size() <= (current_top + 1))
			{
				bots.push_back({});
			}

			current_top++;

			bots[current_top] = bot_data;
		}

		void pop()
		{
			current_top--;
		}

		bool is_empty()
		{
			return current_top < 0;
		}

		bot get_top_bot()
		{
			return bots[current_top];
		}
	};

	static void generate_maze(depth_search_bot_manager* depth_search_bot_manager_context)
	{
		bot top_bot = { {0,0} };

		bot_stack stack;

		stack.push(top_bot);

		maze* maze_context = depth_search_bot_manager_context->maze_generator_context->maze_context;

		maze_cell* entry_cell = maze::get_cell(maze_context, 0, 0);
		maze_cell* final_cell = maze::get_cell(maze_context, maze_context->cell_width - 1, maze_context->cell_height - 1);

		entry_cell->walls[3]->is_open = true;
		final_cell->walls[1]->is_open = true;

		bool solution_generated = false;

		std::vector<maze_cell*>	solution_cells;

		while (true)
		{
			bot working_bot = stack.get_top_bot();

			maze_cell* working_cell = maze::get_cell(maze_context, working_bot.current_position.x, working_bot.current_position.y);

			working_cell->visited = true;

			if (working_cell == final_cell && !solution_generated)
			{
				solution_generated = true;

				for (int i = 0; i < stack.current_top + 1; ++i)
				{
					bot working_bot = stack.bots[i];

					solution_cells.push_back(maze::get_cell(maze_context, working_bot.current_position.x, working_bot.current_position.y));
				}
			}

			if (maze_cell::is_dead_end(working_cell))
			{
				stack.pop();

				if (stack.is_empty())
				{
					break;
				}

				continue;
			}

			maze_cell* target_location;
			int new_cell_index;

			while (true)
			{
				new_cell_index = rand() % 4;

				target_location = working_cell->neighbors[new_cell_index];

				if (target_location == nullptr)
					continue;

				if (target_location->visited)
					continue;

				break;
			}

			working_cell->walls[new_cell_index]->is_open = true;

			bot top_bot = { {target_location->x, target_location->y} };

			stack.push(top_bot);
		}

		maze_context->solution = solution_cells;
	}
};

void maze_generator::create(maze_generator* result, maze* maze_context)
{
	result->maze_context = maze_context;
}

void maze_generator::generate_randomizd_depth_first_search(maze_generator* generator_context)
{
	depth_search_bot_manager bot_manager = { generator_context };

	depth_search_bot_manager::generate_maze(&bot_manager);
}
