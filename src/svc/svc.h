#ifndef _SVC_H_
#define _SVC_H_
#include "common.h"

void panic();
void syscall(uint32_t func);

#endif // _SVC_H_