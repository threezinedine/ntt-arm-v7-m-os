#include "svc.h"
#include "common.h"

static uint32_t ping_count = 0;

extern __syscall(uint32_t, void*);

static void __impl_trap(void* pData);
static void __impl_panic();

#define PANIC_ID 0x00
#define TRAP_ID	 0x80

void __impl_SVCall_Handler(uint32_t trap_id, void* pData)
{
	switch (trap_id)
	{
	case TRAP_ID:
		__impl_trap(pData);
		break;
	default:
		__impl_panic();
	}
}

static void __impl_panic()
{
	while (1)
	{
	}
}

static void __impl_trap(void* pData)
{
	uint32_t func = (uint32_t)pData;

	switch (func)
	{
	case 0:
		ping_count++;
		break;
	default:
		__impl_panic();
	}
}

void panic()
{
	__syscall(PANIC_ID, NULL);
}

void syscall(uint32_t func)
{
	__syscall(TRAP_ID, (void*)func);
}