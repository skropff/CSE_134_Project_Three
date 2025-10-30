#include "dining.h"
#include <semaphore.h>

#include <pthread.h>
#include <stdlib.h>

typedef struct dining {
  // TODO: Add your variables here
  int capacity;
  int capacity1;
  sem_t semaphore1;
  pthread_mutext_t mutex1;
  pthred_cond_t cond1;
  bool bool1;
} dining_t;

dining_t *dining_init(int capacity) {
  // TODO: Initialize necessary variables
  dining_t *dining = malloc(sizeof(dining_t));
  dining->capacity = capacity;
  dining->capacity1 = 0;
  sem_init(dining->semaphore1, 0, capacity);
  pthread_mutex_init(&(dining->mutex1));
  pthread_cond_init(&(dining->cond1));
  dining->bool1 = false;
  return dining;
}
void dining_destroy(dining_t **dining) {
  // TODO: Free dynamically allocated memory
  free(*dining);
  *dining = NULL;
}

void dining_student_enter(dining_t *dining) {
  // TODO: Your code goes here
  sem_wait(&(dining->semaphore1));
  dining->bool1 = false;
  dining->capacity1 = dining->capacity1 + 1;
}

void dining_student_leave(dining_t *dining) {
  // TODO: Your code goes here
  dining->capacity1 = dining->capacity1 - 1;
  sem_post(&(dining->semaphore1));
  if (dining->capacity1 == dining->capacity) {
    dining->bool1 = true;
    pthread_cond_signal(&(dining->semaphore1));
    pthread_mutex_unlock(&(dining->mutex1));
  }
}

void dining_cleaning_enter(dining_t *dining) {
  // TODO: Your code goes here
  pthread_mutex_lock(&(dining->mutex1));
  while (not(dining->bool1)) {
    pthread_cond_wait(&(dining->cond1), &(dining->mutex1));
  }
}

void dining_cleaning_leave(dining_t *dining) {
  // TODO: Your code goes here
  pthread_mutex_unlock(&(dining->mutex1));
}
