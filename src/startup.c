#include "common.h"

#define SRAM_START 0x20000000
#define SRAM_SIZE  0x1FFFFFFF
#define STACK_TOP  ((SRAM_START) + (SRAM_SIZE) - 4)

void Reset_Handler(void);
void Default_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVCall_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMonitor_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

uint32_t exception_handlers[ARCHITECTURE_SYSTEM_EXCEPTIONS_NUMBER + 1]
	__attribute__((section(".exception_handlers"))) = {
		STACK_TOP,
		(uint32_t)Reset_Handler,
		(uint32_t)NMI_Handler,
		(uint32_t)HardFault_Handler,
		(uint32_t)MemManage_Handler,
		(uint32_t)BusFault_Handler,
		(uint32_t)UsageFault_Handler,
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)0,
		(uint32_t)SVCall_Handler,
		(uint32_t)DebugMonitor_Handler,
		(uint32_t)0,
		(uint32_t)PendSV_Handler,
		(uint32_t)SysTick_Handler,
};

int32_t		   a = 23;
const uint32_t b = 23;

void Reset_Handler(void)
{
	while (1)
	{
	}
}

void Default_Handler(void)
{
	while (1)
	{
	}
}