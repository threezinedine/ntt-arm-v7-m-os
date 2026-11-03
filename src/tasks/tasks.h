#ifndef _TASKS_H_
#define _TASKS_H_

#include "common.h"
#include "config.h"

typedef void (*task_function)(void);

enum task_state
{
	TASK_STATE_EMPTY,
	TASK_STATE_CREATED,
	TASK_STATE_FAILED,
	TASK_STATE_PENDING,
	TASK_STATE_RUNNING,
	TASK_STATE_COUNT,
};

typedef enum task_state task_state;

struct task_control_block
{
	uint32_t				   top_stack;
	uint32_t				   sp;
	struct task_control_block* pNext;
	task_function			   pFunc;
	task_state				   state;
};

typedef struct task_control_block  task_control_block;
typedef uint8_t					   task_control_block_id;
extern const task_control_block_id INVALID_TCB_ID;

void task_init();
void task_shutdown();

/**
 * Automatically raise exception if exceed the task count
 */
task_control_block_id task_create(task_function pFunc);
task_control_block*	  task_acquire(task_control_block_id id);
bool				  task_check(task_control_block_id id);
void				  task_destroy(task_control_block_id id);
void				  task_run_default(task_control_block_id id);

#endif // _TASKS_H_