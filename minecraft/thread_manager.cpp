#include "thread_manager.h"

static void call_function_set_running(thread_manager* thread_manager_context)
{
	thread_manager_context->is_running = true;

	while (thread_manager_context->is_running)
	{
		thread_manager_context->time++;

		thread_manager_context->function_to_run((uint64_t*)thread_manager_context->current_arguments.data(), thread_manager_context->current_arguments.size());

		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}

	thread_manager_context->execution_done = true;
}

void thread_manager::create(thread_manager* result, thread_function function_to_run)
{
	result->function_to_run = function_to_run;
	result->is_running = false;
	result->execution_done = false;
	result->time = 0;

	result->raw_thread = new std::thread();
}

void thread_manager::destroy(thread_manager* thread_manager_context)
{
	std::thread* raw_thread = thread_manager_context->raw_thread;

	thread_manager_context->is_running = false;

	while (!thread_manager_context->execution_done);
	while (!raw_thread->joinable());

	raw_thread->join();

	delete thread_manager_context->raw_thread;
}

void thread_manager::start_thread(thread_manager* thread_manager_context, std::vector<void*> arguments)
{
	if (thread_manager_context->is_running)
		return;

	thread_manager_context->current_arguments = arguments;

	*thread_manager_context->raw_thread = std::thread(call_function_set_running, thread_manager_context);
	//thread_manager_context->raw_thread->detach();
}