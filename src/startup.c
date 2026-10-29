#include "common.h"

#define SRAM_START 0x20000000
#define SRAM_SIZE  (64 * 1024)
#define STACK_TOP  ((SRAM_START) + (SRAM_SIZE) - 4)

extern uint32_t _data_start;
extern uint32_t _data_end;

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
	uint8_t* flash_data	 = (uint8_t*)&_data_start;
	uint8_t* ram_data	 = (uint8_t*)SRAM_START;
	uint32_t data_length = (uint32_t)&_data_end - (uint32_t)&_data_start;

	for (uint32_t i = 0; i < data_length; ++i)
	{
		ram_data[i] = flash_data[i];
	}

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