// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0

#include <assert.h>  // Already seen
#include <inttypes.h>  // Already seen
#include <pthread.h>  // Already seen
// Library that provides all the semaphore functions
// like the wait and signal
#include <semaphore.h>

#include <stdint.h>  // Already seen
#include <stdio.h>  // Already seen
#include <stdlib.h>  // Already seen
#include <time.h>  // Already seen
#include <unistd.h>  // Already seen

// thread_shared_data_t
// Shared_data of threads
typedef struct shared_data {
  sem_t* can_greet;  // Signal if the thread can greet
  // its functions will be seen later
  uint64_t thread_count;  // Cant of threads
} shared_data_t;

// thread_private_data_t
// Private data of threads
typedef struct private_data {
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;  // shared_data
} private_data_t;

/**
 * @brief Subroutine that makes the threads greet each other in order
 *  without the need for a busy wait, this is thanks to 
 * the use of a semaphore array that allows effective communication 
 * between them, the semaphore has a counter that check out if
 * the thread has to wait or to execute code
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
 * that we use and do the thread_join, also inicialize the
 * semaphore to use it later and then the subroutine destroys
 * it
 * @param shared_data data
 * 
 * @return error code
 */
int create_threads(shared_data_t* shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  // Creates the threads based of the processors
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;  // Error code
    }
  }

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->can_greet = (sem_t*) calloc(thread_count, sizeof(sem_t));
    shared_data->thread_count = thread_count;  // Calloc of the semaphore

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      // can_greet[thread_number] := create_semaphore(not thread_number)
      error = sem_init(&shared_data->can_greet[thread_number], /*pshared*/ 0
        , /*value*/ !thread_number);
    }

    if (shared_data->can_greet) {
      // Clock that sees the execution time
      struct timespec start_time, finish_time;
      clock_gettime(CLOCK_MONOTONIC, &start_time);

      error = create_threads(shared_data);
      // Exact time
      clock_gettime(CLOCK_MONOTONIC, &finish_time);
      double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
      // print the time
      printf("Execution time: %.9lfs\n", elapsed_time);

      free(shared_data->can_greet);
    } else {
      // Error of the semaphores
      fprintf(stderr, "Error: could not allocate semaphores\n");
      error = 13;
    }
    free(shared_data);
  } else {
    // Error of shared memory
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = 12;
  }
  return error;
}  // end procedure


int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  // Creates a malloc of threads in the heap
  pthread_t* threads = (pthread_t*)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  // Creates a calloc of private data in the heap
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  // If there are no errors
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      if (error == EXIT_SUCCESS) {
        // Store the thread number into the private data
        private_data[thread_number].thread_number = thread_number;
        // Store the shared data created here into shared data of private data
        private_data[thread_number].shared_data = shared_data;
        // create_thread(greet, thread_number)
        error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
          , /*arg*/ &private_data[thread_number]);
        if (error == EXIT_SUCCESS) {
        } else {
          // Error of threads
          fprintf(stderr, "Error: could not create secondary thread\n");
          error = 21;
          break;
        }

      } else {
        // Error of init the semaphore
        fprintf(stderr, "Error: could not init semaphore\n");
        error = 22;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      // Wait for all the threads and destroy them
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
      // Free all the semaphores
      sem_destroy(&shared_data->can_greet[thread_number]);
    }

    free(private_data);  // Free the private data
    free(threads);  // Free the threads
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , shared_data->thread_count);
    error = 23;
  }

  return error;
}

// procedure greet:
void* greet(void* data) {
  assert(data);
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  // Wait until it is my turn
  // wait(can_greet[thread_number])
  // In this case the wait decrease the counter of the semaphore
  // If the counter is -1 that means that a thread has to wait to
  // continue his process, later a thread increase the counter
  int error = sem_wait(&shared_data->can_greet[private_data->thread_number]);
  if (error) {
    fprintf(stderr, "error: could not wait for semaphore\n");
  }

  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , private_data->thread_number, shared_data->thread_count);

  // Allow subsequent thread to do the task
  // signal(can_greet[(thread_number + 1) mod thread_count])
  const uint64_t next_thread = (private_data->thread_number + 1)
    % shared_data->thread_count;

  // In this point the thread has finished is process so he
  // increase the counter of the semaphore to allow a thread
  // to access in this part of the code and then the thread
  // decrease again the counter until he get to the last thread
  error = sem_post(&shared_data->can_greet[next_thread]);
  if (error) {
    // Error of increasing the semaphore counter
    fprintf(stderr, "error: could not increment semaphore\n");
  }

  return NULL;
}  // end procedure
