/*
Semaphores in C
*/
#define _DEFAULT_SOURCE // To stop replit warning about usleep()
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>



int myglobal = 0;
sem_t mySem;


void *thread_function(void *arg) 
{
    
    for (int i = 0; i < 20; i++) 
    {
        // sem_wait(&mySem);
      sem_wait(&mySem);
      int j = myglobal;
      j = j + 1;
      myglobal = j;   
      sem_post(&mySem);
      
      printf(".");    // Child thread prints dots
      fflush(stdout);
      usleep(200);    // Sleep 200 us
      //myglobal = j;   // Save j back in myglobal
      //sem_post(&mySem);
    }
	  
  
    return NULL;
}

int main(void) 
{
    sem_init(&mySem, 0, 1);
    pthread_t mythread;
    
    if (pthread_create(&mythread, NULL, thread_function, NULL))
    {
        printf("Error creating thread.\n");
        exit(1);
    }

    
    for (int i = 0; i < 20; i++) 
	  {
        sem_wait(&mySem);
		    myglobal = myglobal + 1;
        sem_post(&mySem);
      
        printf("o");    // Main thread prints o
		    fflush(stdout);
		    usleep(210);    // Sleep 210 us
        // sem_post(&mySem);
	  }
    // sem_post(&mySem);

	if (pthread_join(mythread, NULL)) {
		printf("error joining thread.");
		exit(1);
	}

	printf("\nmyglobal equals %d\n", myglobal);
  
  sem_destroy(&mySem);

	exit(0);
}
 

