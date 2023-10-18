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

//#include <stdlib.h>
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
int get_Right_turn( unsigned long vehicledirection);
void yield(unsigned long lane); 
void wait(unsigned long lane); 

/**Locks where they represent turns. We implement three locks since each would represent a turn */

static struct lock *lockAB; // Lock for segment AB
static struct lock *lockBC; // Lock for segment BC
static struct lock *lockCA; // Lock for segment CA
static struct lock *truck_in_inter; // Lock for segment CA
struct lock *temp;
struct lock *Atruck,*Btruck,*Ctruck;/*trucks on A,B,and C routes.*/

//volatile int truck_in_intersection = 0;  // This is a shared variable among threads (vehicles)
///struct lock intersection_lock;  // Lock to protect shared state
static struct lock *intersection_lock; //


/*
 * Number of vehicles created.
 */

#define NVEHICLES 20
static const char *route[] = { "A", "B", "C" };
//static const char *vehicle_Type[] = {"CAR", "TRUCK"}; 
//static const char *vehicle_direction[] = {"left", "right"};

int vehicles_left; 


int *Total_Cars;
int *Total_Trucks; 
volatile int truck_in_intersection; 
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

    /// if 
	// if (vehicletype == 1) {
	// 	truck_in_intersection = 0; 
	// }
	///kprintf("Left func: begginign : Vehicle Type : %lu with vehivle Number: %lu is attempting to make a left turn \n",vehicletype, vehiclenumber ); 

   int correct_turn =  get_turn(vehicledirection);

	if (vehicledirection == 0){

		

			lock_acquire(lockAB);
			assert(lock_do_i_hold(lockAB));
           // kprintf("Left func: begginign : Vehicle Type : %lu with vehivle Number: %lu is attempting to make a left turn \n",vehicletype, vehiclenumber ); 
           /// kprintf("Vehicle type: %s vehicle Num: %lu is at section AB in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);
           /// kprintf("Vehicle type: %lu vehicle Num: %lu is at section AB in the intersection\n", vehicletype,vehiclenumber);


			lock_acquire(lockBC);
		 	assert(lock_do_i_hold(lockBC));
		//	kprintf("Vehicle type: %s vehicle Num: %lu is at section BC in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);
           /// kprintf("Vehicle type: %lu vehicle Num: %lu is at section BC in the intersection\n", vehicletype,vehiclenumber);

		 	lock_release(lockBC);
		 	lock_release(lockAB);
	
	}




	else if (vehicledirection == 1){

			lock_acquire(lockBC);
			assert(lock_do_i_hold(lockBC));
			//kprintf("Vehicle type: %s vehicle Num: %lu is at section BC in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);
            //kprintf("Vehicle type: %lu vehicle Num: %lu is at section BC in the intersection\n", vehicletype,vehiclenumber);

			lock_acquire(lockCA);
		    assert(lock_do_i_hold(lockCA));
		//	kprintf("Vehicle type: %s vehicle Num: %lu is at section CA in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);
			//kprintf("Vehicle type: %lu vehicle Num: %lu is at section CA in the intersection\n", vehicletype,vehiclenumber);

			
            lock_release(lockCA);
		 	lock_release(lockBC);

	}

	else// if (vehicledirection == 2)
	{


		

		lock_acquire(lockCA);
		assert(lock_do_i_hold(lockCA));
		//kprintf("Vehicle type: %s vehicle Num: %lu is at section CA in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);
		///kprintf("Vehicle type: %lu vehicle Num: %lu is at section CA in the intersection\n", vehicletype,vehiclenumber);


		lock_acquire(lockAB);
		assert(lock_do_i_hold(lockAB));
		//kprintf("Vehicle type: %s vehicle Num: %lu is at section AB in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);
       //	kprintf("Vehicle type: %lu vehicle Num: %lu is at section AB in the intersection\n", vehicletype,vehiclenumber);

			
        lock_release(lockAB);
		lock_release(lockCA);
	}




		


}

// /**
// * gets correct turn based on the traveled direction. 
// * functon to return the correct left  turn the vehicle is trying to make. 
// */

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

	///kprintf("right func : Vehicle Type : %lu with vehivle Number: %lu is attempting to make a right turn \n",vehicletype, vehiclenumber ); 
	int correct_turn =  get_Right_turn(vehicledirection);

	if (vehicledirection == 0){

		lock_acquire(lockAB); 
		assert(lock_do_i_hold(lockAB)==1);
		///kprintf("Vehicle type: %s vehicle Num: %lu is at rigth section AB in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);
        //kprintf("Vehicle type: %lu vehicle Num: %lu is at rigth section AB in the intersection\n", vehicletype,vehiclenumber);

		lock_release(lockAB); 
		
	}
	else if (vehicledirection == 1){

		lock_acquire(lockBC); 
		//kprintf("Vehicle type: %s vehicle Num: %lu is at right section BC in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);
		///kprintf("Vehicle type: %lu vehicle Num: %lu is at right section BC in the intersection\n",vehicletype,vehiclenumber);

		lock_release(lockBC); 

		
	}
	else if (vehicledirection == 2){

		lock_acquire(lockCA) ; 
		//kprintf("Vehicle type: %s vehicle Num: %lu is at right section CA in the intersection\n", vehicle_Type[vehicletype],vehiclenumber);
		//kprintf("Vehicle type: %lu vehicle Num: %lu is at right section CA in the intersection\n", vehicletype,vehiclenumber);

		lock_release(lockCA); 

	
	}

	//kprintf( "Vehicle Type : %lu with vehivle Number: %lu completed its right turn to %d \n",vehicletype, vehiclenumber,correct_turn ); 

}

int get_Right_turn( unsigned long vehicledirection){
	if (vehicledirection  == 0){
		return 0; 
	}
	else if (vehicledirection == 1){
		return 1; 
	}
	else if (vehicledirection == 2){
		return 2; 
	}

	return -1; 


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

// static
// void
// approachintersection(void * unusedpointer,
// 		unsigned long vehiclenumber)
// {
// 	int vehicledirection, turndirection, vehicletype;

// 	/*
// 	 * Avoid unused variable and function warnings.
// 	 */

// 	(void) unusedpointer;
// 	(void) vehiclenumber;
// 	(void) turnleft;
// 	(void) turnright;

// 	/*
// 	 * vehicledirection is set randomly.
// 	 */

// 	vehicledirection = random() % 3;
// 	turndirection = random() % 2;
// 	vehicletype = random() % 2;
// 	//kprintf("vehicle type %d with number : %lu is approaching the intersection from %d and is traveling to  %d.\n", vehicletype,vehiclenumber, vehicledirection,turndirection);
    
	
	
// 	if (vehicletype == CAR && truck_in_intersection == 0  ){
// 		kprintf("1 vehicle type CAR + TRUCK 66666666666666666666666666666CAR \n");
// 		while(intersection_lock->owner != NULL){

// 			thread_sleep(intersection_lock);
// 		}
// 		lock_acquire(intersection_lock);
// 		assert(lock_do_i_hold(intersection_lock)==1);
// 		send_to_turn(turndirection, (unsigned long) vehicledirection, vehiclenumber, (unsigned long) vehicletype); 
		
// 	}
// 	// if a truck already is in the intersection, and a car is approaching, truck goes first
// 	else if (vehicletype == CAR && truck_in_intersection == 1){ 
// 		kprintf("2 vehicle type CAR + TRUCK  66666666666666666666666666666CAR \n");
	
// 		while (truck_in_intersection == 1 && intersection_lock->owner != NULL ){
// 		thread_sleep(intersection_lock); 
// 		} 
// 		lock_acquire(intersection_lock); 
// 		assert(lock_do_i_hold(intersection_lock)==1);
// 		send_to_turn(turndirection, (unsigned long) vehicledirection,vehiclenumber, (unsigned long) vehicletype); 

		


// 	}
// 	else if (vehicletype == TRUCK && truck_in_intersection == 1  ){
// 		kprintf("3 vehicle type  TRUCK 999999999999999 \n");
// 		while (truck_in_intersection == 1 ){
// 		thread_sleep(intersection_lock); 
// 		} 
// 		lock_acquire(intersection_lock);
// 		assert(lock_do_i_hold(intersection_lock)==1);
//         send_to_turn( turndirection, (unsigned long) vehicledirection, vehiclenumber, (unsigned long) vehicletype); 
		
		
// 	}
// 	else { // tack is the firt at the entersection. 
// 	kprintf("elseeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee \n");
// 	lock_acquire(intersection_lock);
// 	assert(lock_do_i_hold(intersection_lock)==1);
// 	truck_in_intersection = 1; 
// 	send_to_turn(turndirection,  (unsigned long) vehicledirection,vehiclenumber, (unsigned long) vehicletype); 
	


// 	}
// 		lock_release(intersection_lock);
// 		vehicles_left--; 

// }

/**
*
* function to determine which turn to use. left if 0  or right if 1
*/
void send_to_turn( int turn , unsigned long vehicledirection,unsigned long vehiclenumber,unsigned long vehicletype){

	if (turn == 0 ){
	
		turnleft(vehicledirection,vehiclenumber,vehicletype);
	}
	else {
		
		turnright(vehicledirection,vehiclenumber,vehicletype);

	}



}

// static
// void
// approachintersection(void * unusedpointer,
// 		unsigned long vehiclenumber)
// {
// 	int vehicledirection, turndirection, vehicletype;

// 	/*
// 	 * Avoid unused variable and function warnings.
// 	 */

// 	(void) unusedpointer;
// 	(void) vehiclenumber;
// 	(void) turnleft;
// 	(void) turnright;

// 	/*
// 	 * vehicledirection is set randomly.
// 	 */

// 	//vehicledirection = random() % 3;
// 	//turndirection = random() % 2;
// 	//vehicletype = random() % 2;

// 	if(vehicletype == CAR){

// 		lock_acquire(intersection_lock);
// 		if (truck_in_intersection == 1){

// 			//car proceeds
// 			send_to_turn(turndirection,  (unsigned long) vehicledirection,vehiclenumber, (unsigned long) vehicletype); 
// 		}
// 		else { //car waits
// 		lock_release(intersection_lock);
//         thread_sleep(intersection_lock);
//         lock_acquire(intersection_lock);



// 		}
// 		 lock_release(intersection_lock);
// 	}
// 	else if(vehicletype == TRUCK) {

//                lock_acquire(intersection_lock);

//                if(truck_in_intersection == 1) {
//                // Truck 1 waits 
// 			   lock_release(intersection_lock);
//                thread_sleep(intersection_lock);
//               lock_acquire(intersection_lock);
//                }
// 			     truck_in_intersection = 1; // Set truck present

//   // Truck proceeds
//              send_to_turn(turndirection,  (unsigned long) vehicledirection,vehiclenumber, (unsigned long) vehicletype); 

//               truck_in_intersection = 0; // Clear truck present

//             lock_release(intersection_lock);

//            }
// 		   vehicles_left--; 

// }

int get_direction_left(int dir,  int turn ){
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

	/// vehicle number, vehicle type (car or truck), approach direction and destination direction
	int direct =  get_direction_left(vehicledirection,turndirection ); 

	//kprintf("Vehicle type: %s vehicle Num: %lu approach %s destination : %s direction: %s\n", vehicle_Type[vehicletype],vehiclenumber, route[vehicledirection] ,route[direct], vehicle_direction[turndirection]);
    //kprintf("Vehicle type: %d vehicle Num: %lu approach  \n", vehicletype, vehiclenumber);

	if(vehicletype==CAR){
		
		Total_Cars[vehicledirection]+=1;  // increment cars on road. 
       /// if (truck_in_intersection   != 0 )
	
	
		// if (Total_Trucks[vehicledirection] != 0)
		// {
        //    yield(vehicledirection);

		// }
		//else {

	///kprintf("Vehicle type: %s vehicle Num: %lu entering %s destination : %s direction: %s\n", vehicle_Type[vehicletype],vehiclenumber, route[vehicledirection] ,route[direct], vehicle_direction[turndirection]);
	//kprintf("Vehicle type: %d vehicle Num: %lu entering \n", vehicletype,vehiclenumber);

	     send_to_turn(turndirection,  (unsigned long) vehicledirection,vehiclenumber, (unsigned long) vehicletype);
         Total_Cars[vehicledirection]-=1; 
       
	   if(Total_Cars[vehicledirection]==0){ /*if there are no cars,trucks can proceed. */
        yield(vehicledirection);
       }
		//}  


	}

	else {


		if(Total_Cars[vehicledirection] > 0) {  
             /// wait(vehicledirection); /*truck threads are put to sleep if there are cars on the same root. */
			   int spl=splhigh();

              thread_sleep(Atruck);

              splx(spl);
            }
			//truck_in_intersection ++; 


			//Total_Trucks[vehicledirection] +=1;
		send_to_turn(turndirection,  (unsigned long) vehicledirection,vehiclenumber, (unsigned long) vehicletype);
		//truck_in_intersection--; 
		//Total_Trucks[vehicledirection] -=1; 

	}

   // lock_acquire(intersection_lock);

	  Vehicle_counter++;
         if(Vehicle_counter == NVEHICLES) {
              Vehicle_counter=0;
		     	int spl = splhigh();
		    	thread_wakeup(temp);
		     	splx(spl);
  }
  	///kprintf("Vehicle type: %s vehicle Num: %lu exited %s destination : %s direction: %s\n", vehicle_Type[vehicletype],vehiclenumber, route[vehicledirection] ,route[direct], vehicle_direction[turndirection]);
   // kprintf("Vehicle type: %d vehicle Num: %lu exited \n", vehicletype,vehiclenumber);

   //  lock_release(intersection_lock);
  vehicles_left--; 


}

/*wake up a truck based on its direction*/
void yield(unsigned long lane)
{
  int spl=splhigh();
 /// thread_wakeup(Atruck);
   switch(lane){
     case 0:
      thread_wakeup(Atruck);
      break;
    case 1:  
      thread_wakeup(Btruck);
      break;
    case 2:
      thread_wakeup(Ctruck);
      break;     
   }
   splx(spl);
}

// void wait(unsigned long lane)
// {
//   int spl=splhigh();

//   thread_sleep(Atruck);

//    splx(spl); 
// }


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
	truck_in_intersection = 0; 
	 temp=lock_create("temp");
	  // Atruck = lock_create("Atruck");
    //    Btruck = lock_create("Btruck");
    //    Ctruck = lock_create("Ctruck");
  /*an int array to keep track of numcars on each of the three routes*/
     Total_Cars = (int *)kmalloc(3 * sizeof(int));
	*(Total_Cars) = 0;
	*(Total_Cars + 1) = 0;
	*(Total_Cars + 2) = 0;
	// Total_Trucks = (int *)kmalloc(3 * sizeof(int)); 
	// *(Total_Trucks) = 0;
	// *(Total_Trucks + 1) = 0;
	// *(Total_Trucks + 2) = 0;

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

	intersection_lock = lock_create("intersection_lock");
if (intersection_lock == NULL) {
    panic("createvehicles: couldn't create intersection lock");
}
    truck_in_inter = lock_create("truck_in_inter");


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
	//kprintf("****************vehicles_left: %d   *****************\n", vehicles_left);
	// After all vehicle threads have completed.
// 	while(1)
// 		{
// 		if(vehicles_left==0)
// 		{
// 			kprintf("****************vehicles_left: %d   *****************\n", vehicles_left);
// lock_destroy(lockAB); 
// lock_destroy(lockBC); 
// lock_destroy(lockCA); 
// lock_destroy(intersection_lock); 
// 		}
// 		}

		int spl = splhigh();
   if(Vehicle_counter < NVEHICLES) {
  		thread_sleep(temp);
  	}
  	splx(spl);


	kprintf("****************vehicles_left: %d   *****************\n", vehicles_left);
lock_destroy(lockAB); 
lock_destroy(lockBC); 
lock_destroy(lockCA); 
lock_destroy(intersection_lock); 
lock_destroy(temp);
// lock_destroy(Atruck);
// lock_destroy(Btruck);
// lock_destroy(Ctruck);
kfree(Total_Cars);
//kfree(Total_Trucks);


 
   

	return 0;
}