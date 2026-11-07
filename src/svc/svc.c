#include "svc.h"
#include "common.h"

static void trap(void* pData);
static void panic();

void __impl_SVCall_Handler(uint32_t trap_id, void* pData)
{
	switch (trap_id)
	{
	case TRAP_ID:
		trap(pData);
		break;
	default:
		panic();
	}
}

static void panic()
{
	while (1)
	{
	}
}

static void trap(void* pData)
{
}