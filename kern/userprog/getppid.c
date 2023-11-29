/** getppid.c */

/*
 *  Function: sys_getppid()
 *
 */

#include <types.h>
#include <syscall.h>
#include <thread.h>
#include <curthread.h>

// int
// sys_getppid(pid_t *retval) 

pid_t sys_getppid() 
{

 
    
    if (curthread->procs == NULL || !curthread->procs->exitStat )  {
        return -1; 
    }
	

    return curthread->procs->ppid;

}