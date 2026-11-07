#include "tasks.h"
#include "common.h"
#include "svc/svc.h"

const task_control_block_id	 INVALID_TCB_ID = (task_control_block_id)-1;
static task_control_block	 tasks[RTOS_TASKS_COUNT];
static task_control_block_id s_head_tcb_id = INVALID_TCB_ID;
static task_control_block_id s_tail_tcb_id = INVALID_TCB_ID;

extern uint32_t get_control_register();
extern void		set_control_register(uint32_t);
extern void		set_psp(uint32_t);
extern void		to_user_space(uint32_t);

void task_init()
{
}

void task_shutdown()
{
}

task_control_block_id task_create(task_function pFunc)
{
	task_control_block_id free_id = INVALID_TCB_ID;
	for (task_control_block_id i = 0; i < RTOS_TASKS_COUNT; ++i)
	{
		if (tasks[i].state == TASK_STATE_EMPTY)
		{
			free_id = i;
			goto out;
		}
	}

	panic();

out:
	task_control_block* pTask = &tasks[free_id];

	pTask->sp		 = STACK_TOP - KERNEL_STACK_SIZE - TASK_STACK_SIZE * free_id;
	pTask->top_stack = pTask->sp;
	pTask->pFunc	 = pFunc;
	pTask->state	 = TASK_STATE_CREATED;

	if (s_head_tcb_id != INVALID_TCB_ID)
	{
		pTask->pNext			   = &tasks[s_head_tcb_id];
		tasks[s_tail_tcb_id].pNext = pTask;
	}
	else
	{
		s_tail_tcb_id = free_id;
	}

	s_head_tcb_id = free_id;

	return free_id;
}

void task_run_default(task_control_block_id id)
{
	task_control_block* pTask = task_acquire(id);

	pTask->state = TASK_STATE_RUNNING;

	// switch to user space

	set_psp(pTask->sp);
	to_user_space((uint32_t)pTask->pFunc);
}

task_control_block* task_acquire(task_control_block_id id)
{
	if (!task_check(id))
	{
		panic();
	}

	return &tasks[id];
}

bool task_check(task_control_block_id id)
{
	if (id > RTOS_TASKS_COUNT)
	{
		panic();
	}

	return tasks[id].state != TASK_STATE_EMPTY;
}

void task_destroy(task_control_block_id id)
{
	if (id > RTOS_TASKS_COUNT)
	{
		panic();
	}
}