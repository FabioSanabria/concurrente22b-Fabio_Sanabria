// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0

#include <assert.h>  // defines one macro function that can be used as a standard debugging tool:
#include <inttypes.h>  // Header with library support for width-based integral types.
#include <pthread.h>  // Already seen
#include <stdint.h>  // Already seen
#include <stdio.h>  // Already seen
#include <stdlib.h>  // Already seen
#include <unistd.h>  // Already seen

// thread_private_data_t
typedef struct private_data {
  uint64_t thread_number;  // rank
  uint64_t thread_count;  // Number of threads
} private_data_t;

/**
 * @brief Says hello in a different thread, in this case "greet" 
 * is going to be used by the number of threads requested by the user
 * @return void
 */
void* greet(void* data);

/**
 * @brief Subrutine responsible for creating the threads requested by the user
 * and use of dinamic memory 
 * is going to be used by the number of threads requested by the user
 * @return void
 */
int create_threads(uint64_t thread_count);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  error = create_threads(thread_count);  // Calls the subrutine
  return error;
}  // end procedure

int create_threads(uint64_t thread_count) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  // Allocate dinamic memory
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
  // Allocate dinamic memory (inicialize all the values in 0)
  private_data_t* private_data = (private_data_t*)
    calloc(thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    // Cycle that create the threads to greet
    for (uint64_t thread_number = 0; thread_number < thread_count
        ; ++thread_number) {
      // Inicialize thread_number
      private_data[thread_number].thread_number = thread_number;
      // Inicialize thread_count
      private_data[thread_number].thread_count = thread_count;
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
        , /*arg*/ &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;  // Code of error
        break;
      }
    }

    // print "Hello from main thread"
    // Wait for all the threads created
    printf("Hello from main thread\n");
    for (uint64_t thread_number = 0; thread_number < thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }
    // Free all the dinamic memory used by the structs
    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , thread_count);  // Error of allocate threads
    error = 22;  // Code of error
  }

  return error;
}

// procedure greet:
void* greet(void* data) {
  // assert(data);
  // Allocate the variable data to a new private variable
  private_data_t* private_data = (private_data_t*) data;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , (*private_data).thread_number, private_data->thread_count);
  return NULL;
}  // end procedure
