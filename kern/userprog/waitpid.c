
#include <types.h>        
#include <kern/errno.h>   
#include <thread.h>
#include <lib.h>  	

struct process *get_child_process(pid_t pid);
void process_cleanup(struct process *proc);
int sys_waitpid(pid_t pid, userptr_t status, int options, int *retval) {
 
    if (status == NULL) {
        *retval = EFAULT;
         return -1;
        }

	//options check
	if(options != 0){
		*retval = EINVAL;
		return -1;
	}
    struct process *child_process = get_child_process(pid);
    if (child_process == NULL) {
        *retval = EINVAL;
        return -1;
    }

    while (!child_process->terminated) { 
        thread_sleep(child_process);  
    }

    if (copyout(&child_process->terminated, status, sizeof(int)) != 0) {  
        *retval = EFAULT;
        return -1;
    }

    //process_cleanup(child_process);

    return pid;
}

/**
 * function to retrieve process child off the list. not compeleted yet. 
*/
struct process *get_child_process(pid_t pid){


    return NULL; 
}