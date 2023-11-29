/*  getpid.c */

/*
 *  Function: sys_getpid()
 *
 */

#include <types.h>
#include <syscall.h>
#include <thread.h>
#include <curthread.h>

pid_t sys_getpid()
{
	return curthread->procs->pid;

	
}