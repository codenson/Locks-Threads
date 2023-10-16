/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>
//#include <string.h>


/*
 *
 * Constants
 *
 */
#define CAR 0
#define TRUCK 1
int createvehicles(int nargs,char ** args); 
void send_to_turn( int turn , unsigned long vehicledirection,unsigned long vehiclenumber,unsigned long vehicletype); 
static void approachintersection(void * unusedpointer, unsigned long vehiclenumber); 
static void turnright(unsigned long vehicledirection, unsigned long vehiclenumber, unsigned long vehicletype); 
int get_turn( unsigned long vehicledirection);
static void turnleft(unsigned long vehicledirection, unsigned long vehiclenumber, unsigned long vehicletype); 

/**Locks where they represent turns. We implement three locks since each would represent a turn */

static struct lock *lockAB; // Lock for segment AB
static struct lock *lockBC; // Lock for segment BC
static struct lock *lockCA; // Lock for segment CA

volatile int truck_in_intersection = 0;  // This is a shared variable among threads (vehicles)
///struct lock intersection_lock;  // Lock to protect shared state
static struct lock *intersection_lock; //


/*
 * Number of vehicles created.
 */

#define NVEHICLES 20



/*
 *
 * Function Definitions
 *
 */



/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long vehicledirection: the direction from which the vehicle
 *              approaches the intersection.
 *      unsigned long vehiclenumber: the vehicle id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long vehicledirection,
		unsigned long vehiclenumber,
		unsigned long vehicletype)
{
	/*
	 * Avoid unused variable warnings.
	 */

	(void) vehicledirection;
	(void) vehiclenumber;
	(void) vehicletype;
/*	lock *lockAB; // Lock for segment AB
 lock *lockBC; // Lock for segment BC
lock *lockCA;*/
    // let route  A =  0
	//Route B = 1
	// Route C  = 2
	/*
	          C               B
	            \            /
				 \          /
				     / BC \
				    /     \
				   /        \
				   _CA____AB__
				       |
					   |
					   A
	*/ 
     //Route A = 0 ; B = 1; C = 2
	 //
	 // turns : AB = 0; BC = 1; CA = 2

    /// if 
	kprintf("Vehicle Type : %lu with vehivle Number: %lu is attempting to make a left turn \n",vehicletype, vehiclenumber ); 

    int correct_turn =  get_turn(vehicledirection);
	lock_acquire(lockAB); 
	lock_acquire(lockBC); 
	lock_acquire(lockCA); 
	kprintf( "Vehicle Type : %lu with vehivle Number: %lu completed its left turn to %d \n",vehicletype, vehiclenumber,correct_turn ); 

	//if 
	//  
	// if (correct_turn  == 0 ){
	// 	while ()


	// }
	//lock_release(intersection_lock);


}

/**
* gets correct turn based on the traveled direction. 
* functon to return the correct left  turn the vehicle is trying to make. 
*/

int get_turn( unsigned long vehicledirection){
	if (vehicledirection  == 0){
		return 2; 
	}
	else if (vehicledirection == 1){
		return 0; 
	}
	else if (vehicledirection == 2){
		return 1; 
	}

	return -1; 


} 


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long vehicledirection: the direction from which the vehicle
 *              approaches the intersection.
 *      unsigned long vehiclenumber: the vehicle id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long vehicledirection,
		unsigned long vehiclenumber,
		unsigned long vehicletype)
{
	/*
	 * Avoid unused variable warnings.
	 */

	(void) vehicledirection;
	(void) vehiclenumber;
	(void) vehicletype;
}


/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long vehiclenumber: holds vehicle id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createvehicles().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */

 /**
 *This fucntion controls who goes first and to redirect the vehicles toward right or left. 
 *  cars can enter the intersection if a truck is not already there. 
 * cars have the priotiry in the intersection. each vehiles acquires the lock to the intersection. 
 *
 */

static
void
approachintersection(void * unusedpointer,
		unsigned long vehiclenumber)
{
	int vehicledirection, turndirection, vehicletype;

	/*
	 * Avoid unused variable and function warnings.
	 */

	(void) unusedpointer;
	(void) vehiclenumber;
	(void) turnleft;
	(void) turnright;

	/*
	 * vehicledirection is set randomly.
	 */

	vehicledirection = random() % 3;
	turndirection = random() % 2;
	vehicletype = random() % 2;
	kprintf("vehicle type %d with number : %lu is approaching the intersection from %d and is traveling to  %d.\n", vehicletype,vehiclenumber, vehicledirection,turndirection);
    
	///if (lock_acquire())
	
	if (vehicletype == CAR && truck_in_intersection == 0  ){
		lock_acquire(intersection_lock);
		send_to_turn(turndirection, (unsigned long) vehicledirection, vehiclenumber, (unsigned long) vehicletype); 
		
	}
	// if a truck already is in the intersection, and a car is approaching, truck goes first
	else if (vehicletype == CAR && truck_in_intersection == 1){ 
	
		while (truck_in_intersection == 1 ){
		thread_sleep(intersection_lock); 
		} 
		lock_acquire(intersection_lock); 
		send_to_turn(turndirection, (unsigned long) vehicledirection,vehiclenumber, (unsigned long) vehicletype); 

		


	}
	else if (vehicletype == TRUCK && truck_in_intersection == 1  ){
		while (truck_in_intersection == 1 ){
		thread_sleep(intersection_lock); 
		} 
		lock_acquire(intersection_lock);
        send_to_turn( turndirection, (unsigned long) vehicledirection, vehiclenumber, (unsigned long) vehicletype); 
		
	}
	else { // tack is the firt at the entersection. 
	lock_acquire(intersection_lock);
	truck_in_intersection = 1; 
	send_to_turn(turndirection,  (unsigned long) vehicledirection,vehiclenumber, (unsigned long) vehicletype); 
	


	}
		lock_release(intersection_lock);
}

/**
*
* function to determine which turn to use. left if 0  or right if 1
*/
void send_to_turn( int turn , unsigned long vehicledirection,unsigned long vehiclenumber,unsigned long vehicletype){
	if (turn == 0 ){
		kprintf("vehiles type : %lu with number : %lu is trying to make a left turn %lu",vehicletype , vehiclenumber, vehicledirection );
		turnleft(vehicledirection,vehiclenumber,vehicletype);
	}
	else {
		kprintf("vehiles type : %lu with number : %lu is trying to make a right turn %lu",vehicletype , vehiclenumber, vehicledirection );
		turnright(vehicledirection,vehiclenumber,vehicletype);

	}



}


/*
 * createvehicles()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createvehicles(int nargs,
		char ** args)
{
	int index, error;

	/*
	 * Avoid unused variable warnings.
	 */

	(void) nargs;
	(void) args;
	 // Initialize the locks for each segment of the intersection
    lockAB = lock_create("lockAB");
    if (lockAB == NULL) {
        panic("createvehicles: couldn't create lock AB");
    }

    lockBC = lock_create("lockBC");
    if (lockBC == NULL) {
        panic("createvehicles: couldn't create lock BC");
    }

    lockCA = lock_create("lockCA");
    if (lockCA == NULL) {
        panic("createvehicles: couldn't create lock CA");
    }

	intersection_lock = lock_create("intersection_lock");
if (intersection_lock == NULL) {
    panic("createvehicles: couldn't create intersection lock");
}


	/*
	 * Start NVEHICLES approachintersection() threads.
	 */

	for (index = 0; index < NVEHICLES; index++) {

		error = thread_fork("approachintersection thread",
				NULL,
				index,
				approachintersection,
				NULL
				);

		/*
		 * panic() on error.
		 */

		if (error) {

			panic("approachintersection: thread_fork failed: %s\n",
					strerror(error)
				 );
		}
	}

	return 0;
}