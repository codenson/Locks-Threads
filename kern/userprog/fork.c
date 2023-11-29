#include <types.h>
#include <syscall.h>
#include <process.h>
#include <thread.h>
#include <curthread.h>
#include <lib.h>
#include <uio.h>

/*system fork that will split a process when the function is called.
  err checks and returns the pid of child or parent.
*/
pid_t sys_fork(struct trapframe *parent_tf, int32_t retval)
{
    struct trapframe *child_tf;
    struct addrspace *child_addrs;
    struct thread *chthread;
	    
	// Allocate memory for the child's trapframe
    child_tf = kmalloc(sizeof(struct trapframe));
    if (child_tf == NULL) {
        return ENOMEM;
    }

    // Allocate memory for the child's address space
    child_addrs = kmalloc(sizeof(struct addrspace));
    if (child_addrs == NULL) {
        kfree(child_tf);  // Free previously allocated memory
        return ENOMEM;
    }

    // Copy the parent's trapframe to the child's trapframe
    memcpy(child_tf, parent_tf, sizeof(struct trapframe));
	
	
    // Set the child's parent process ID
    chthread->ppid = curthread->pid;
	
	
    // Copy the address space from the parent to the child
    int err = as_copy(curthread->t_vmspace, &chthread->t_vmspace);
    if (err) {
        // Handle the error: clean up and return an error code
		kfree(child_tf);
		return err;
    }

    // Activate the new address space
    as_activate(chthread->t_vmspace);

	// Create a new thread for the child process and check to see if it worked or not
    chthread = thread_fork("Child Process", parent_tf, (unsigned long)child_tf, md_forkentry, NULL);
    if (chthread == NULL) {
        kfree(child_addrs);
		kfree(child_tf);
        return ENOMEM;
    }
	
    return 0;
}
