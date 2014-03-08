#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int thread_count;
int message;
void *Consumer(void* rank);
void *Producer();
sem_t ProductNum;
sem_t AllConsumed;
sem_t mutex;
sem_t next;
int counter = 0;
sem_t count_sem;
sem_t barrier_sem;
int r;
int main(int argc, char* argv[]){
  long thread;
  pthread_t idp;
  pthread_t* thread_handles;
  srand(time(NULL));
  
  int i = 0;
  
  
  r = rand();
  thread_count = strtol(argv[1], NULL, 10);
  thread_handles = malloc(thread_count*sizeof(pthread_t));
  sem_init(&ProductNum, 0, 0);
  sem_init(&AllConsumed, 0, 0);
  sem_init(&mutex, 0, 1);
  sem_init(&next, 0, 0);
  sem_init(&count_sem, 0, 1);
  sem_init(&barrier_sem, 0, 0);
  for(thread = 0; thread<thread_count; thread++){
    pthread_create(&thread_handles[thread], NULL, Consumer, (void*) thread);
    if(thread == r%thread_count)
      pthread_create(&idp, NULL, Producer, NULL);
  }
  
  
  for(thread = 0; thread < thread_count; thread++)
    pthread_join(thread_handles[thread], NULL);
  //pthread_join(idp, NULL);
  
  
  
  free(thread_handles);
  

  return 0;
  

}

void *Producer(){
  int z;
  for(z = 0; z < 10; z++){
    printf("Message passing loop %d\n", z);
    int randnum = rand();
    message = randnum;
    printf("I AM THE PRODUCER\n");
    int a;
    for(a = 0; a < thread_count; a++){
      sem_post(&ProductNum);
      sem_wait(&next);
    }
    sem_wait(&AllConsumed);
    //for(a = 0; a < thread_count; a++){
    //  sem_wait(&AllConsumed);
      
    //}
    printf("All has been consumed\n");

  }
  
  return NULL;
}

void *Consumer(void* rank){
  int x;
  long my_rank = (long)rank;
  for(x = 0; x < 10; x++){
    
    sem_wait(&mutex);
    
    sem_wait(&ProductNum);
    sem_post(&next);
    //sem_post(&AllConsumed);
    
    printf("%ld has consumed %d\n", my_rank, message);
    sem_post(&mutex);
    
    //sem_wait(&next);
    sem_wait(&count_sem);
    if(counter == (thread_count-1)){
      counter = 0;
      sem_post(&count_sem);
      
      int y;
      for(y = 0; y < (thread_count-1); y++)
	sem_post(&barrier_sem);
      sem_post(&AllConsumed);
    } else {
      
      counter++;
      sem_post(&count_sem);
      sem_wait(&barrier_sem);
    }
    
    
  }
  return NULL;
}
