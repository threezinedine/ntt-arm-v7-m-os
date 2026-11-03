#include "systick/systick.h"
#include "tasks/tasks.h"

static uint32_t count = 0;

static void Add(void);

void main(void)
{
	task_control_block_id task1 = task_create(Add);

	task_run_default(task1);
}

static void Add()
{
	while (1)
	{
		count += 1;
	}
}