#include <types.h>
#include <lib.h>
#include <uio.h>
#include <thread.h>
#include <syscall.h>

pid_t sys_getppid(){
	return thread->procs->ppid;
}
