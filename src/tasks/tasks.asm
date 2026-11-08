.syntax unified
.cpu cortex-m3
.thumb

.section .text

.global to_user_space
.global get_control_register
.global set_control_register
.global set_psp
.global SysTick_Handler
.global __init_task_stack

.extern __impl_SysTick_Handler

.type SysTick_Handler, %function

get_control_register: @ uint32_t (void)
    mrs r0, control
    bx  lr

set_control_register: @ void (uint32_t)
    msr control, r0
    bx lr

to_user_space: @ void (uint32_t)
    mov r1, 0x03
    msr control, r1
    mov lr, r0
    bx  lr

set_psp: @ void (uint32_t)
    msr psp, r0
    bx lr

__init_task_stack: @ void(uint32_t* sp, uint32_t pc, uint32_t xpsr, uint32_t lr)
    mrs  r5, psp
    subs r5, r5, #60
    str  r2, [r5, #60]  @ xpsr
    str  r1, [r5, #56]  @ pc
    str  r3, [r5, #52]  @ lr
    movs r1, #0     
    str  r1, [r5, #48]  @ r12
    str  r1, [r5, #44]  @ r3
    str  r1, [r5, #40]  @ r2
    str  r1, [r5, #36]  @ r1
    str  r1, [r5, #32]  @ r0
    str  r1, [r5, #0]   @ r4
    str  r1, [r5, #4]   @ r5
    str  r1, [r5, #8]   @ r6
    str  r1, [r5, #12]  @ r7
    str  r1, [r5, #16]  @ r8
    str  r1, [r5, #20]  @ r9
    str  r1, [r5, #24]  @ r10
    str  r1, [r5, #28]  @ r11
    msr  psp, r5
    bx  lr

SysTick_Handler: @ void(void)
    push  {lr}
    mrs   r0, psp
    stmdb r0!, {r4-r11}
    bl    __impl_SysTick_Handler
    mrs   r0, psp
    ldmia r0!, {r4-r11}
    msr   psp, r0
    pop   {pc}