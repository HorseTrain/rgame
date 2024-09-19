#pragma once

#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <thread>
#include <inttypes.h>
#include <vector>

typedef void(*thread_function)(uint64_t* arguments, int argument_count);

struct thread_manager
{
	bool				is_running;
	bool				execution_done;
	std::thread*		raw_thread;
	thread_function		function_to_run;
	std::vector<void*>	current_arguments;

	uint64_t			time;

	static void			create(thread_manager* result, thread_function function_to_run);
	static void			destroy(thread_manager* thread_manager_context);
	static void			start_thread(thread_manager* thread_manager_context, std::vector<void*> arguments);
};

#endif