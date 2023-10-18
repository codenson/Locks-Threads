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

#include <machine/spl.h>



/*
 *
 * Constants
 *
 */

int createvehicles(int nargs,char ** args); 
void send_to_turn( int turn , unsigned long vehicledirection,unsigned long vehiclenumber,unsigned long vehicletype); 
static void approachintersection(void * unusedpointer, unsigned long vehiclenumber); 
static void turnright(unsigned long vehicledirection, unsigned long vehiclenumber, unsigned long vehicletype); 
static void turnleft(unsigned long vehicledirection, unsigned long vehiclenumber, unsigned long vehicletype); 


/**Locks where they represent turns. We implement three locks since each would represent a turn */

static struct lock *lockAB; // Lock for segment AB
static struct lock *lockBC; // Lock for segment BC
static struct lock *lockCA; // Lock for segment CA

static struct lock *intersection_lock; //
#define CAR 0
#define TRUCK 1

/*
 * Number of vehicles created.
 */

#define NVEHICLES 20
static const char *route[] = { "A", "B", "C" };
static const char *vehicle_Type[] = {"CAR  ", "TRUCK"}; 
static const char *vehicle_direction[] = {"left", "right"};

int vehicles_left; 


int *Total_Cars;
volatile int Vehicle_counter=0;



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

static void turnleft(unsigned long vehicledirection, unsigned long vehiclenumber, unsigned long vehicletype){
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

	if (vehicledirection == 0){

			lock_acquire(lockAB);
			assert(lock_do_i_hold(lockAB));
            kprintf("Vehicle type: %s vehicle Num: %lu is at section AB in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);

            lock_release(lockAB);
			

			lock_acquire(lockBC);
		 	assert(lock_do_i_hold(lockBC));
			kprintf("Vehicle type: %s vehicle Num: %lu is at section BC in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);


		 	lock_release(lockBC);
		 
	
	}

	else if (vehicledirection == 1){

			lock_acquire(lockBC);
			assert(lock_do_i_hold(lockBC));
			kprintf("Vehicle type: %s vehicle Num: %lu is at section BC in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);
     
            lock_release(lockBC);

			lock_acquire(lockCA);
		    assert(lock_do_i_hold(lockCA));
			kprintf("Vehicle type: %s vehicle Num: %lu is at section CA in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);


			
            lock_release(lockCA);
		 	

	}

	else// if (vehicledirection == 2)
	{
	

		lock_acquire(lockCA);
		assert(lock_do_i_hold(lockCA));
		kprintf("Vehicle type: %s vehicle Num: %lu is at section CA in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);
	
        lock_release(lockCA);


		lock_acquire(lockAB);
		assert(lock_do_i_hold(lockAB));
		kprintf("Vehicle type: %s vehicle Num: %lu is at section AB in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);
			
        lock_release(lockAB);
		
	}

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

	if (vehicledirection == 0){

		lock_acquire(lockAB); 
		kprintf("Vehicle type: %s vehicle Num: %lu is at rigth section AB in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);

		lock_release(lockAB); 
		
	}
	else if (vehicledirection == 1){

		lock_acquire(lockBC); 
		kprintf("Vehicle type: %s vehicle Num: %lu is at right section BC in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);

		lock_release(lockBC); 

		
	}
	else if (vehicledirection == 2){

		lock_acquire(lockCA) ; 
		kprintf("Vehicle type: %s vehicle Num: %lu is at right section CA in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);

		lock_release(lockCA); 
	
	}
}


/**
*
* function to determine which turn to use. left if 0  or right if 1
*/
void send_to_turn( int turn , unsigned long vehicledirection,unsigned long vehiclenumber,unsigned long vehicletype){

	turn == 0 ? turnleft(vehicledirection,vehiclenumber,vehicletype) : turnright(vehicledirection,vehiclenumber,vehicletype);
}



int get_dest_direction(int dir,  int turn ){
	if (turn == 0)
	{

	if (dir == 0){
		return 2; 
	}
	else if (dir == 1){

		return 0; 
	}
	else {

		return 1; 
	}
	} 
	else {
		if (dir == 0){
		return 1; 
	}
	else if (dir == 1){

		return 2; 
	}
	else{

		return 0; 
	}

	}

	return  -1; 

}


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

	int direct =  get_dest_direction(vehicledirection,turndirection ); 

	kprintf("Vehicle type: %s vehicle Num: %lu approach %s destination : %s direction: %s\n", vehicle_Type[vehicletype],vehiclenumber, route[vehicledirection] ,route[direct], vehicle_direction[turndirection]);

	if(vehicletype==CAR){
		
		Total_Cars[vehicledirection]+=1;  // increment cars  on road. 
     

	     send_to_turn(turndirection,  (unsigned long) vehicledirection,vehiclenumber, (unsigned long) vehicletype);
         Total_Cars[vehicledirection]-=1; 
       
	   if(Total_Cars[vehicledirection]==0){ /*if there are no cars,trucks can proceed. */

	     int spl=splhigh();
		thread_wakeup(intersection_lock);
		 splx(spl);
       }
	}

	else {
      

			if(Total_Cars[vehicledirection] > 0) {  
			   int spl=splhigh();

              thread_sleep(intersection_lock);

            splx(spl);
            }
		send_to_turn(turndirection,  (unsigned long) vehicledirection,vehiclenumber, (unsigned long) vehicletype);
	}


	  Vehicle_counter++;
         if(Vehicle_counter == NVEHICLES) {
              Vehicle_counter=0;
		     	int spl = splhigh();
				thread_wakeup(intersection_lock);
		     	splx(spl);
  }
  	kprintf("Vehicle type: %s vehicle Num: %lu exited %s destination : %s direction: %s\n", vehicle_Type[vehicletype],vehiclenumber, route[vehicledirection] ,route[direct], vehicle_direction[turndirection]);
  vehicles_left--; 


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
	


  /*an int array to keep track of numcars on each of the three routes*/
     Total_Cars = (int *)kmalloc(3 * sizeof(int));
	*(Total_Cars) = 0;
	*(Total_Cars + 1) = 0;
	*(Total_Cars + 2) = 0;


	vehicles_left = NVEHICLES; 
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
	

	while (vehicles_left != 0){


	}



	kprintf("****************vehicles_left: %d   *****************\n", vehicles_left);
lock_destroy(lockAB); 
lock_destroy(lockBC); 
lock_destroy(lockCA); 
//lock_destroy(intersection_lock); 

kfree(Total_Cars);

	return 0;
}