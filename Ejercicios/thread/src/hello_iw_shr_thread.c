// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0

// All the includes was seen before
#include <assert.h>
#include <inttypes.h>

// Library of threads, its
// similar to pthreads
#include <threads.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// thread_shared_data_t
typedef struct shared_data {
  uint64_t thread_count;
} shared_data_t;

// thread_private_data_t
typedef struct private_data {
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;


/**
 * @brief Says hello in a different thread, in this case "greet" 
 * is going to be used by the number of threads requested by the user using
 * shared_data as a shared variable. Another case, we use threads instef of
 * using pthreads, it works perfectly, only a warning of casting is the problem
 * @return void
 */
void* greet(void* data);

/**
 * @brief Subrutine responsible for creating the threads requested by the user
 * and use of dinamic memory is going to be used by the number of threads requested
 *  by the user using shared_data, the process of creation of threads is different of
 * the phtreads becasuse they need a function that return a thrd_start_t insted of
 * void*, and is a little bit difficulty to search information about that because 
 * 
 * @return void
 */
int create_threads(shared_data_t* shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  // _SC_NPROCESSORS_ONLN = Number of proccesors
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {  // Scan the number
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {  // Use the shared data to know the thread count
    shared_data->thread_count = thread_count;
    error = create_threads(shared_data);
    free(shared_data);  // Free shared data
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    return 12;
  }
  return error;
}  // end procedure

int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  // We stract the count of the shared_data to know
  // how many threads do we need
  thrd_t* threads = (thrd_t*)
    malloc(shared_data->thread_count * sizeof(thrd_t));
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      private_data[thread_number].thread_number = thread_number;
      //  Save the shared_data into the shared_data of the private_data
      // for each thread
      private_data[thread_number].shared_data = shared_data;
      // create_thread(greet, thread_number)
      error = thrd_create(&threads[thread_number], (thrd_start_t)greet
        , /*arg*/ &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      thrd_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    // Free all the memory
    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , shared_data->thread_count);
    error = 22;
  }

  return error;
}

// procedure greet:
void* greet(void* data) {
  // assert(data);
  // Extract the shared_data of private_data and
  // put his memory direction int the new one
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  // print "Hello from secondary thread" using shared_data
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , private_data->thread_number, shared_data->thread_count);
  return NULL;
}  // end procedure
