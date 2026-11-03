.syntax unified
.cpu cortex-m3
.thumb

.section .text

.global to_user_space
.global get_control_register
.global set_control_register
.global set_psp

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