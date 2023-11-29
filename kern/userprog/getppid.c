/** getppid.c */

/*
 *  Function: sys_getppid()
 *
 */

#include <types.h>
#include <syscall.h>
#include <process.h>
#include <thread.h>
#include <curthread.h>

int
sys_getpid(pid_t *retval) 
{
	//if (curthread->procs->pid )
	return 0;
}