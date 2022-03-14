/*
Condition Variables and Mutex to make Monitor Concurrency Control
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t con_var = PTHREAD_COND_INITIALIZER;

void *functionEven();
void *functionOdd();
int count = 0;
int retval;

#define COUNT_DONE 10

int main()
{
  pthread_t thr_even, thr_odd;

  pthread_create(&thr_odd, NULL, &functionOdd, NULL);
  pthread_create(&thr_even, NULL, &functionEven, NULL);

  //pthread_join(thr_odd, NULL); // hang
  pthread_join(thr_even, NULL);  

  fprintf(stdout, "Final count: %d\n", count);

  exit(0);
}
// Write odd numbers
void *functionOdd()
{
  for(;;) // almost while loop
    {
      if (count >= COUNT_DONE)
        {
          //return(NULL);      
          pthread_exit(&retval);
        }
      
      pthread_mutex_lock(&count_mutex);
      pthread_cond_wait(&con_var, &count_mutex);
      count++;
      printf("Counter value functionOdd: %3d\n", count);

      pthread_mutex_unlock(&count_mutex);

      
    }  
}

// Write even numbers
void *functionEven()
  {
    for(;;)
      {
        if (count >= COUNT_DONE)
          { 
            //pthread_exit(&retval);
            return(NULL);
          }
        
        pthread_mutex_lock(&count_mutex);
        if(count < 1 || count % 2 == 0)
          {
            pthread_cond_signal(&con_var);
          }
        else
          {
            count++;          
            printf("Counter value functionEven: %2d\n", count);
          }    

        pthread_mutex_unlock(&count_mutex);

        
      }
  }
