.section .text

.global SVCall_Handler @ void(void)
.global __syscall @ void(void)

.extern __impl_SVCall_Handler

.type SVCall_Handler, %function

SVCall_Handler:
    push {lr}
    bl __impl_SVCall_Handler
    pop {pc}

__syscall:
    push {lr}
    svc #132