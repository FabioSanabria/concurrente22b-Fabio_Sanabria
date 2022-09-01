// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>

/**
 * @brief ...
 */
void* greet(void* data);

// procedure main:
int main(void) {
  // create_thread(greet)
  pthread_t thread;
  size_t number = 2;
  int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ (void*)number);
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    // usleep(1);  // indeterminism
    printf("Hello from main thread\n");
    pthread_join(thread, /*value_ptr*/ NULL);
  } else {
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  return error;
}  // end procedure

// procedure greet:
void* greet(void* data) {
  size_t number = (size_t)data;

  if(number > 0) {
    pthread_t thread;
    printf("Hello from secondary thread-> %zu\n", number);
    pthread_create(&thread, /*attr*/ NULL, greet, 
    /*arg*/ (void*)(number-1));
    pthread_join(thread, NULL);
  }
  else
  {
    // print "Hello from secondary thread"
    printf("Goodbye from secondary thread -> %zu\n", number);
  }
  return NULL;
}  // end procedure
