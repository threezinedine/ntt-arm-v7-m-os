#include "common.h"
#include "svc/svc.h"
#include "tasks/tasks.h"

static uint32_t count = 0;

static void Add(void);
static void Sub();

void HardFault_Handler(void)
{
	while (1)
	{
	}
}

void main(void)
{
	task_control_block_id task1 = task_create(Add);
	task_control_block_id task2 = task_create(Sub);

	task_run_default(task1);
}

static void Add()
{
	while (1)
	{
		count += 1;

		if (~(count & 1))
		{
			syscall(SYSCAL_PING);
		}
	}
}

static void Sub()
{
	while (1)
	{
		count += 1;
	}
}