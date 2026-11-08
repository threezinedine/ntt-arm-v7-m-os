#include "tasks.h"
#include "common.h"
#include "svc/svc.h"

const task_control_block_id	 INVALID_TCB_ID = (task_control_block_id)-1;
static task_control_block	 tasks[RTOS_TASKS_COUNT];
static task_control_block_id s_head_tcb_id	 = INVALID_TCB_ID;
static task_control_block_id s_tail_tcb_id	 = INVALID_TCB_ID;
static task_control_block*	 s_p_current_tcb = NULL;

extern uint32_t get_control_register();
extern void		set_control_register(uint32_t);
extern void		set_psp(uint32_t);
extern void		to_user_space(uint32_t);
extern void		__init_task_stack(uint32_t* sp, uint32_t pc, uint32_t xpsr, uint32_t lr);

#define SYSTICK_BASE (0xE000E010UL)
#define SYST_CSR	 (*(volatile uint32_t*)(SYSTICK_BASE + 0x00))
#define SYST_RVR	 (*(volatile uint32_t*)(SYSTICK_BASE + 0x04))
#define SYST_CVR	 (*(volatile uint32_t*)(SYSTICK_BASE + 0x08))

#define SYST_CSR_ENABLE	   (1UL << 0)
#define SYST_CSR_TICKINT   (1UL << 1)
#define SYST_CSR_CLKSOURCE (1UL << 2)

static uint32_t g_systick = 0;

void task_init()
{
	// Disable counter
	SYST_CSR = 0;

	SYST_RVR = RELOAD_CLOCK_VALUE & 0x00FFFFFF;
	SYST_CVR = 0;

	SYST_CSR = SYST_CSR_ENABLE | SYST_CSR_TICKINT | SYST_CSR_CLKSOURCE;
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
	if (s_p_current_tcb == NULL)
	{
		s_p_current_tcb = pTask;
	}

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

static void task_next(uint32_t new_psp);

void __impl_SysTick_Handler(uint32_t new_psp)
{
	g_systick++;
	task_next(new_psp);
}

static void task_next(uint32_t new_psp)
{
	if (s_p_current_tcb == NULL)
	{
		return;
	}

	task_control_block* pCurrent_tcb = s_p_current_tcb;
	task_control_block* pNext_tcb	 = pCurrent_tcb->pNext;

	if (pNext_tcb == NULL)
	{
		pNext_tcb = &tasks[s_head_tcb_id];
	}

	if (pCurrent_tcb == pNext_tcb)
	{
		// There's only 1 task
		return;
	}

	set_psp(pNext_tcb->sp);
	if (pNext_tcb->state == TASK_STATE_CREATED)
	{
		__init_task_stack(&(pNext_tcb->sp), pNext_tcb->pFunc, 0x01000000, 0x00);
	}

	pCurrent_tcb->sp = new_psp;

	pCurrent_tcb->state = TASK_STATE_PENDING;
	pNext_tcb->state	= TASK_STATE_RUNNING;
	s_p_current_tcb		= pNext_tcb;
}