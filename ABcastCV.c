#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


int thread_count;
int message;
int consumeOK;
void *Consumer(void* rank);
void *Producer();

int count;
pthread_mutex_t mutex;
pthread_cond_t cv;
int r;

int main(int argc, char* argv[]){
  long thread;
  pthread_t idp;
  pthread_t* thread_handles;
  srand(time(NULL));
  
  int i = 0;
  
  for(i = 0; i < 10; i++){
    consumeOK = 0;
    r = rand();
    printf("loop %d ----------------\n", i);
    count = 0;
    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count*sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cv, NULL);
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
  pthread_mutex_lock(&mutex);
  printf("I AM THE PRODUCER\n");
  message = r;
  consumeOK = 1;
  
  while(count < thread_count){
    pthread_cond_wait(&cv, &mutex);
  }
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void *Consumer(void* rank){
  
    
  
  while(consumeOK != 1){
    NULL;
  }
    
  long my_rank = (long) rank;
  
  count++;
  if(count == thread_count)
    pthread_cond_signal(&cv);
  printf("%ld has received message %d\n", my_rank, message);
  
  return NULL;
}
