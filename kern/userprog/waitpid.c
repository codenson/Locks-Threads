#include <errno.h>
#include <thread.h> 

int sys_waitpid(pid_t pid, userptr_t status, int options, int *retval){


    //status check. 
    if (status == NULL) {
        *retval = EFAULT;
         return -1;
        }

	//options check
	if(options != 0){
		retval = EINVAL;
		return -1;
	}

	 // Retrieving  the child process. if child process is NULL, returns a fault code. 
    struct process *child_proccess = get_child_process(pid);
    if (child_p == NULL) {
        *retval = EINVAL;
        return -1;
    }
    // putting the child process to sleep using thread_sleep() untill woken up. 
    while (!child_proccess->has_exited) {
        thread_sleep(); 
    }

    // Copying out the process and exit status
    if (copyout(&child_proccess->exitcode, status, sizeof(int)) != 0) {
        *retval = EFAULT;
        return -1;
    }

    // Cleaning up  child process. 
     process_cleanup(child_proccess);

    return pid;
}
	
	
	
