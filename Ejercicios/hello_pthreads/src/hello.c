// Copyright 2021 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0

#include <pthread.h> // Library of pthreads
#include <stdio.h> // Standard input- output header
#include <stdlib.h> // Standard library

/**
 * @brief Says hello in a different thread than the main one
 * @param data Thats a pointer that we don't know what type it is, we only know that it points something
 * @return It returns null because we dont need nothing, only print hello
 */
void* greet(void* data);

/**
 * @brief Says hello in the main thread
 * @return error code, 0 if the code success and 1 if the code fails
 */
int main(void) {
  // create_thread(greet)
  pthread_t thread;
  int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ NULL);
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    printf("Hello from main thread\n");
    pthread_join(thread, /*value_ptr*/ NULL);
  } else {
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  return error;
}  // end procedure

// procedure greet:
void* greet(void* data) {
  (void)data;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread\n");
  return NULL;
}  // end procedure
