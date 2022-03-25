/*
     producer / consumer problem
*/
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 5

// shared stuff
char shared_storage[SIZE];
int in, out; // index tracking for producer/consumer
sem_t total_empty, total_stored, s;
int stop_count;
int max;

void *producer(void *arg);
void *consumer(void *arg);

int main(void)
{
  pthread_t thread1, thread2;
  sem_init(&total_empty, 0, SIZE);
  sem_init(&total_stored, 0, 0);
  sem_init(&s, 0, 1);
  in = 0;
  out = 0;
  stop_count = 0;
  max = 30;
  
  pthread_create(&thread1, NULL, &producer, NULL);
  pthread_create(&thread2, NULL, &consumer, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  sem_destroy(&total_empty);
  sem_destroy(&total_stored);
  sem_destroy(&s);  
}


void *producer(void *arg)
{
  // we are gonna put an 'X' in each
  
  while (stop_count < max)
    {
      sem_wait(&total_empty);
      sem_wait(&s);
      shared_storage[in] = 'X';
      printf("Producer added '%c' to shared_storage[%d]\n", shared_storage[in], in);
      printf("stop_count at: %d\n", stop_count);
      stop_count++;
      in = (in + 1) % SIZE;      
      sem_post(&s);
      sem_post(&total_stored);
    }
  return (0);
}

void *consumer(void *arg)
{
  
  while (stop_count < max)
  {
    sem_wait(&total_stored);
    sem_wait(&s);
    printf("Consumer taking '%c' from shared_storage[%d]\n", shared_storage[out], out);
    printf("stop_count at: %d\n", stop_count);
      stop_count++;
    out = (out + 1) % SIZE;
    sem_post(&s);
    sem_post(&total_empty);
  }
  return (0);
}
