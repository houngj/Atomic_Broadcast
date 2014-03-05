#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int thread_count;
char* message;
void *Consumer(void* rank);
void *Producer();
sem_t ProductNum;
sem_t AllConsumed;
sem_t mutex;
int counter;
sem_t count_sem;
sem_t barrier_sem;
int main(int argc, char* argv[]){
  long thread;
  pthread_t idp;
  pthread_t* thread_handles;
  srand(time(NULL));
  
  int i = 0;
  
  for(i = 0; i<10; i++){
    int r = rand();
    printf("loop %d -----------\n", i);
    
    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count*sizeof(pthread_t));
    sem_init(&ProductNum, 0, 0);
    sem_init(&AllConsumed, 0, -(thread_count-1));
    sem_init(&mutex, 0, 1);
    sem_init(&count_sem, 0, 1);
    sem_init(&barrier_sem, 0, 0);
    for(thread = 0; thread<thread_count; thread++){
      pthread_create(&thread_handles[thread], NULL, Consumer, (void*) thread);
      if(thread == r%thread_count)
	pthread_create(&idp, NULL, Producer, NULL);
    }  

    
    

    for(thread=0; thread<thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
    free(thread_handles);
    
  }
  return 0;
  

}

void *Producer(){
  printf("I AM THE PRODUCER\n");
  int i = 0;
  message = "This a Message from the Producer";
  for(i = 0; i < thread_count; i++)
    sem_post(&ProductNum);
  sem_wait(&AllConsumed);
  return NULL;
}

void *Consumer(void* rank){
  long my_rank = (long) rank;
  sem_wait(&mutex);
  sem_wait(&ProductNum);
  sem_post(&AllConsumed);
  printf("%ld has received the message: %s\n", my_rank, message);
  sem_post(&mutex);


  if(counter == thread_count-1){
      counter = 0;
      sem_post(&count_sem);
      int j;
      for(j = 0; j < thread_count-1; j++)
	sem_post(&barrier_sem);
    } else {
      counter++;
      sem_post(&count_sem);
      sem_wait(&barrier_sem);
  }
  
  return NULL;
  
}
