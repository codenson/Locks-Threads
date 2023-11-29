#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

#include <trapframe.h>

int sys_reboot(int code);
//int sys_getppid(pid_t *retval); 
pid_t sys_getppid(); 
pid_t sys_getpid();
int sys_waitpid();
pid_t sys_fork(struct trapframe *parent_tf, int32_t retval); 

int sys_execv();
void sys_exit(int32_t procStat);

#endif /* _SYSCALL_H_ */
