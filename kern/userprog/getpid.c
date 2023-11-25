#include <types.h>
#include <lib.h>
#include <uio.h>
#include <thread.h>
#include<syscall.h>

pid_t getpid(){
	return thread->procs->pid;
}
