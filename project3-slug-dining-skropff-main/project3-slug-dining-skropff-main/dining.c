#include "dining.h"
#include <semaphore.h>
#include <stdbool.h> 
#include <stdio.h>

#include <pthread.h>
#include <stdlib.h>

typedef struct dining {
  // TODO: Add your variables here
  int capacity;
  int capacity1;
  sem_t semaphore1;
  sem_t semaphore2;
  sem_t semaphore3;
  pthread_mutex_t mutex1;
  pthread_cond_t cond1;
  bool bool1;
  pthread_mutex_t mutex2;
  pthread_cond_t cond2;
  bool bool2;
  pthread_mutex_t *mutex_array;
  pthread_t *id_array;
  pthread_cond_t everyone;
  bool cleaning_done;
  pthread_mutex_t throwaway;
} dining_t;

dining_t *dining_init(int capacity) {
  // TODO: Initialize necessary variables
  dining_t *dining = malloc(sizeof(dining_t));
  dining->capacity = capacity;
  dining->capacity1 = 0;
  sem_init(&(dining->semaphore1), 0, capacity);
  // sem_init(&(dining->semaphore1), 0, 1);
  pthread_mutex_init(&(dining->mutex1), NULL);
  pthread_mutex_init(&(dining->throwaway), NULL);
  pthread_cond_init(&(dining->cond1), NULL);
  dining->bool1 = true;
  pthread_mutex_init(&(dining->mutex2), NULL);
  pthread_cond_init(&(dining->cond2), NULL);
  pthread_cond_init(&(dining->everyone), NULL);
  dining->cleaning_done = true;
  dining->bool2 = true;
  dining->mutex_array = (pthread_mutex_t *) malloc(capacity * sizeof(pthread_mutex_t));
  dining->id_array = (pthread_t *) calloc(capacity, sizeof(pthread_t));
  for (int i = 0; i < capacity; i = i + 1) {
     pthread_mutex_init(dining->mutex_array + i, NULL);
  }
  return dining;
}
void dining_destroy(dining_t **dining) {
  // TODO: Free dynamically allocated memory
  free(*dining);
  *dining = NULL;
}

void dining_student_enter(dining_t *dining) {
  // TODO: Your code goes here
  pthread_mutex_lock(&(dining->throwaway);
  sem_wait(&(dining->semaphore1));
  pthread_mutex_t personal;
  pthread_mutex_init(&personal, NULL);
  int result;
  result = -1;
  /*
  pthread_mutex_lock(&(dining->mutex2));
  while (!(dining->bool2)) {
    pthread_cond_wait(&(dining->cond2), &(dining->mutex2));
  }
  */
  dining->bool1 = false;
  dining->capacity1 = dining->capacity1 + 1;
  while (result == -1) {
    for (int i = 0; i < dining->capacity; i = i + 1) {
      result = pthread_mutex_trylock(dining->mutex_array + i);
      printf("result: %d\n", result);
      if (result == 0) {
        (dining->id_array)[i] = pthread_self();
        break;
      }
    }
    if (result == -1) {
      pthread_mutex_lock(&(dining->throwaway));
      pthread_mutex_unlock(&(dining->throwaway));
    }
  }
  /*
    if (result == -1) {
      pthread_mutex_lock(&personal);
      while (!dining->cleaning_done) {   
        pthread_cond_wait(&(dining->everyone), &personal);
      }
      pthread_mutex_unlock(&personal);
    }
    */
}

void dining_student_leave(dining_t *dining) {
  // TODO: Your code goes here
  dining->capacity1 = dining->capacity1 - 1;
  for (int i = 0; i < dining->capacity; i = i + 1) {
    if ((dining->id_array)[i] == pthread_self()) {
      pthread_mutex_unlock(dining->mutex_array + i);
      (dining->id_array)[i] = 0;
    }
  }
  sem_post(&(dining->semaphore1));
  /*
  if (dining->capacity1 == 0) {
    dining->bool1 = true;
    pthread_cond_signal(&(dining->cond1));
    pthread_mutex_unlock(&(dining->mutex1));
  }
  */
}

void dining_cleaning_enter(dining_t *dining) {
  // TODO: Your code goes here
  /*
  if (!(dining->bool1)) {
    pthread_mutex_lock(&(dining->mutex1));
    while (!(dining->bool1)) {
      pthread_cond_wait(&(dining->cond1), &(dining->mutex1));
    }
  }
  */
  pthread_mutex_lock(&(dining->throwaway));
  dining->bool2 = false;
  // pthread_mutex_unlock(&(dining->mutex1));
  dining->cleaning_done = false;
  for (int i = 0; i < dining->capacity; i = i + 1) {
    pthread_mutex_lock(dining->mutex_array + i);
  }
}

void dining_cleaning_leave(dining_t *dining) {
  // TODO: Your code goes here
  for (int i = 0; i < dining->capacity; i = i + 1) {
    pthread_mutex_unlock(dining->mutex_array + i);
  }
  pthread_mutex_unlock(&(dining->throwaway));
  dining->bool2 = true;
}
