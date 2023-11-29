#include <types.h>
#include <syscall.h>
#include <thread.h>
#include <curthread.h>


/*
 *  Function: sys_exit()
 *  Will take the exit code and place it into the thread and make sure to clarify that the code will be gone.
 * This will make sure that the array can check if this thread is gone and can be replaced with another.
 * The exit code will help with thread exit and it will destroy this thread. Before that, it will checck if its null.
 */

void sys_exit(int exitNum){

	KASSERT(curthread!=NULL);
	
	curthread->procs->gone = 1;
	curthread->procs->exitStat = exitNum; 
	thread_exit();
}
