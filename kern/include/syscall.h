#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
pid_t sys_getppid();
pid_t getpid();
int sys_waitpid();
int sys_fork(struct trapframe *parent_tf, int *ret);
int sys_execv();
void sys__exit(int32_t procStat);

#endif /* _SYSCALL_H_ */
