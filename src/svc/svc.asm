.section .text

.global SVCall_Handler @ void(uint32_t)
.global __syscall @ void(uint32_t, void*)

.extern __impl_SVCall_Handler

.type SVCall_Handler, %function

SVCall_Handler:
    push {lr}
    mrs  r0, psp
    ldr  r1, [r0, #4]
    ldr  r0, [r0]
    bl   __impl_SVCall_Handler
    pop {pc}

__syscall:
    push {lr}
    svc  #0
    pop  {pc}