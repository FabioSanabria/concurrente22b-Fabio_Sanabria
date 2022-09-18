// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0

#include <assert.h>  // Already seen
#include <inttypes.h>  // Already seen
#include <pthread.h>  // Already seen
#include <stdint.h>  // Already seen
#include <stdio.h>  // Already seen
#include <stdlib.h>  // Already seen

// Library that provides all the tools of
// time like clock_gettime, seed of rand
#include <time.h>
#include <unistd.h>  // Already seen

// thread_shared_data_t
// (this is considered shared memory)
// In this struct we have the next_thread
// and the number of threads
typedef struct shared_data {
  uint64_t next_thread;
  uint64_t thread_count;
  uint64_t delay_shared;
} shared_data_t;

// thread_private_data_t
// (This is considered private memory)
// In this struct we have the thread number
// and a shared_data_t
typedef struct private_data {
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;  // shared memory
  uint64_t delay_private;
} private_data_t;

/**
 * @brief Subroutine that realize a secondary thread
 * in this case this subroutine has a busywait, This means that
 * a thread has to wait for another thread so that it can continue its execution,
 * this eliminates the indeterminism but can use 100% of the processor and damage it
 * 
 * @param private_data_t data (Will be parsed)
 * 
 * @return NULL
 */
void* greet(void* data);

/**
 * @brief Subroutine that creates all the threads that
 * we have to use in this program, if we dont put an 
 * especific number the program will create threads
 * as much as processors we have, this method has a lot
 * of fprintf if an error ocurred and free al the memory
 * that we use and do the thread_join
 * @param private_data_t data (Will be parsed)
 * 
 * @return NULL
 */
int create_threads(shared_data_t* shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  uint64_t delay = 0;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  // If the user dont put a number of threads this will
  // create threads as much as processors we have
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 3) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      // Error of cant of threads
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }

    if (sscanf(argv[2], "%" SCNu64, &delay) == 1) {
    } else {
      fprintf(stderr, "Error: invalid number of delay\n");
      return 17;
    }
  }
  // calloc of shared data
  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->next_thread = 0;
    shared_data->thread_count = thread_count;
    shared_data->delay_shared = delay;
    // This will tell us the execution time of the program
    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);  // Ignore the error

    error = create_threads(shared_data);  // Create the threads
    // Operations that says me the time in sgs of duration
    // of the program
    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

    // Print of the execution time of the program
    printf("Execution time: %.9lfs\n", elapsed_time);
    // free shared memory
    free(shared_data);
  } else {
    // Error of allocate memory
    fprintf(stderr, "Error: could not allocate shared data\n");
    return 12;  // Code of error
  }
  return error;
}  // end procedure


int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  // Dinamic memory of the threads
  pthread_t* threads = (pthread_t*)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      // Inicialize of the atributes of private_data
      private_data[thread_number].delay_private = shared_data->delay_shared;
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
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
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    free(private_data);  // free memory
    free(threads);  // free memory
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , shared_data->thread_count);
    error = 22;
  }

  return error;
}

// procedure greet:
void* greet(void* data) {
  assert(data);
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  // Wait until it is my turn, DANGER DONT USE BUSY WAIT
  // inside of this cycle there is nothing, this it only serves
  // to disturb the machine
  while (shared_data->next_thread < private_data->thread_number) {
     sleep(private_data->delay_private);
  }  // end while

  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , private_data->thread_number, shared_data->thread_count);

  // Allow subsequent thread to do the task
  ++shared_data->next_thread;

  return NULL;
}  // end procedure
