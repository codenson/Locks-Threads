/**
 * exev.c file. 
*/
#include <types.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <thread.h>
#include <curthread.h>
#include <vm.h>
#include <vfs.h>
#include <test.h>
#include "copyinout.h"

static int copyin_args(char* const user_argv[], char** kernel_argv, int* argc); 

static int copyout_args(char** kernel_argv, int argc, vaddr_t* ptr); 

#define MAX 4096 // maximum argument size

int execv(const char* path, char* const argv[]) {
    struct vnode *node;
    vaddr_t start, stackptr;
    int result;
    int argc;
    char **kernel_argv; // Array of pointers in kernel space
     
     // memry allocation. 
     kernel_argv = kmalloc(MAX * sizeof(char*));
    if (kernel_argv == NULL) {
        return ENOMEM; 
    }

    
    result = copyin_args(argv, kernel_argv, &argc);//  copy args to kernel from user space. 
    if (result) {
        return result; //  error if copying failed
    }

    // Open the executable and create a new address space and load the elf into it. 
    result = vfs_open((char *)path, O_RDONLY, &node);
    if (result) {
        kfree(kernel_argv); // Clean up kernel buffer
        return result;
    }

    assert(curthread->t_vmspace == NULL);

    curthread->t_vmspace = as_create();
    if (curthread->t_vmspace == NULL) {
        vfs_close(node);
        kfree(kernel_argv);
        return ENOMEM;
    }

    as_activate(curthread->t_vmspace);

    result = load_elf(node, &start);
    if (result) {
        vfs_close(node);
        as_destroy(curthread->t_vmspace);
        curthread->t_vmspace = NULL;
        kfree(kernel_argv);
        return result;
    }

    vfs_close(node);
    result = as_define_stack(curthread->t_vmspace, &stackptr);
    if (result) {
        as_destroy(curthread->t_vmspace);
        curthread->t_vmspace = NULL;
        kfree(kernel_argv);
        return result;
    }

    
    result = copyout_args(kernel_argv, argc, &stackptr);
    if (result) {
        as_destroy(curthread->t_vmspace);
        curthread->t_vmspace = NULL;
        kfree(kernel_argv);
        return result;
    }
   
    int in = 0; 
    // Free the kernel buffer. 
    while (in < argc){

         kfree(kernel_argv[in]);
         in++; 
    }
    kfree(kernel_argv);

    //Return to user mode by calling md_usermode.
    md_usermode(argc, (userptr_t)stackptr, stackptr, start);

    panic("md_usermode returned\n");
    return EINVAL;
}

/**
 * copies args from user space into kernel.it uses a function to copy defined in copyinout.c provided in os161. 
*/
static int copyin_args(char* const user_argv[], char** kernel_argv, int* argc) {
    int counter = 0;
    size_t size;
    int result;
    int i = 0; 

    // get total arguments num 
    while (user_argv[counter] != NULL) {
        counter++;
    }
    *argc = counter;

    // mmry for 
    kernel_argv = kmalloc((*argc + 1) * sizeof(char*));
    if (kernel_argv == NULL) {
        return ENOMEM;
    }

    for (i = 0; i < *argc; i++) {
        kernel_argv[i] = kmalloc(MAX); 
      
        if (kernel_argv[i] == NULL) {
            // failure consideration. 
            while (i-- > 0) {
                kfree(kernel_argv[i]);
            }
            kfree(kernel_argv);
            return ENOMEM; 
        }

        result = copyinstr((const_userptr_t)user_argv[i], kernel_argv[i], MAX, &size);
        int j = 0; 
        if (result) {
            // Clean up in case of failure
            for ( j = 0; j <= i; j++) {
                kfree(kernel_argv[j]);
            }
            kfree(kernel_argv);
            return result;
        }
    }

    kernel_argv[*argc] = NULL;
    return 0;
}


/**
 * Function to copy args. it uses the fucntoion defined in copyinout.c provided in os161. 
*/
static int copyout_args(char** kernel_argv, int argc, vaddr_t* ptr) { 
    vaddr_t pos = *ptr;
    int result;
    size_t len;
    int i = 0; 
    int index = 0; 

    pos -= (argc + 1) * sizeof(char*);
   
    // Copy  args to user stack
    while (index < argc ){

           len = strlen(kernel_argv[index]) + 1; 
        pos -= len;

        // Copy  from  from kernel to user space
        result = copyout(kernel_argv[index], (userptr_t)pos, len);
        if (result) { //if resutl is true.. not zero. 
            return result;
        }
        kernel_argv[index] = (char*)pos;
        i++; 
    }
 
    // Copy out the array of pointers//
    // calling the function in copyinout.c 
    result = copyout(kernel_argv, (userptr_t)(pos - (argc + 1) * sizeof(char*)), (argc + 1) * sizeof(char*));
    if (result) {
        return result;
    }

    *ptr = pos;
    return 0;
}


