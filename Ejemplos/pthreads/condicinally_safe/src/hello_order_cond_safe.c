// Copyright 2022 Fabio Sanabria<fabio.sanabria@ucr.ac.cr> CC-BY 4.0

#include <assert.h>  // Already seen
#include <inttypes.h>  // Already seen
#include <pthread.h>  // Already seen
#include <stdint.h>  // Already seen
#include <stdio.h>  // Already seen
#include <stdlib.h>  // Already seen
#include <time.h>  // Already seen
#include <unistd.h>  // Already seen

#define MAX_GREET_LEN 256  // This is a constant value

// thread_shared_data_t
typedef struct shared_data {
  // We create the greets to store the greets of a thread
  // and avoid to use the tools of concurrence
  char** greets;  // This is like a string in c++
  uint64_t thread_count;
} shared_data_t;

// thread_private_data_t
typedef struct private_data {
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Subroutine that store the greet of a thread
 *  into the array "greets", in this case we avoid the race
 * condition using a different space of memory to print
 * the greet of each thread. 
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
 * of fprintf if an error ocurred and free all the memory
 * that we use and do the thread_join, in this case we dont
 * use any structure of concurrence control, we store each greet
 * of a thread into an array of greets, this is used to work
 * in another part of the memory to avoid the data race that is
 * our worst enemy
 * @param shared_data_t shared_data
 * 
 * @return error code
 */
int create_threads(shared_data_t* shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  // The thread number depends of the number of processors in our computer
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->greets = (char**) calloc(thread_count, sizeof(char*));
    shared_data->thread_count = thread_count;

    if (shared_data->greets) {
      // TIme of execution the program
      struct timespec start_time, finish_time;
      clock_gettime(CLOCK_MONOTONIC, &start_time);

      error = create_threads(shared_data);

      clock_gettime(CLOCK_MONOTONIC, &finish_time);
      double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
      // Print the execution time of the program
      printf("Execution time: %.9lfs\n", elapsed_time);

      free(shared_data->greets);  // Free the greets to avoid the memory leaks
    } else {
      // Error of allocate greets
      fprintf(stderr, "Error: could not allocate greets\n");
      error = 13;
    }
    free(shared_data);  // Free the shared data
  } else {
    // Error of allocate shared memory
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = 12;
  }
  return error;
}  // end procedure


int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  pthread_t* threads = (pthread_t*)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      shared_data->greets[thread_number] = (char*)
        malloc(MAX_GREET_LEN * sizeof(char));  // Ask for memory in the heap
        // for greets (Dinamic array)
      if (shared_data->greets[thread_number]) {
        // Inicialize the firts element of greet in 0
        // *shared_data->greets[thread_number] = '\0'; // this is the same
        shared_data->greets[thread_number][0] = '\0';
        private_data[thread_number].thread_number = thread_number;
        private_data[thread_number].shared_data = shared_data;
        // create_thread(greet, thread_number)
        // At this part we create the thread and the greets array otbtain all
        // the thread greets that we need
        error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
          , /*arg*/ &private_data[thread_number]);
        if (error == EXIT_SUCCESS) {
        } else {
          fprintf(stderr, "Error: could not create secondary thread\n");
          error = 21;
          break;
        }
      } else {
        fprintf(stderr, "Error: could not init semaphore\n");
        error = 22;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    // for thread_number := 0 to thread_count do
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      // print greets[thread_number]
      // At this part we print the greets of all the threads in
      // order, that because we work with the print in a different
      // place of the memory that prevent all types of data race
      // And finally, when we print a greet in specific, that greet
      // wil be free or destroye because we dont need it anymore
      printf("%s\n", shared_data->greets[thread_number]);
      free(shared_data->greets[thread_number]);
    }  // end for

    free(private_data);  // Free the private data
    free(threads);  // Free the threads
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

  // greets[thread_number] := format("Hello from secondary thread"
  // , thread_number, " of ", thread_count)
  // This is one of the most important instructions of the
  // condicionally safe, we store the greet of the thread into
  // the greets array
  // El linter se va a quejar de usar el sprintf, lo voy a dejar a como
  // Jeisson lo tiene
  sprintf(shared_data->greets[private_data->thread_number]
    , "Hello from secondary thread %" PRIu64 " of %" PRIu64
    , private_data->thread_number, shared_data->thread_count);

  return NULL;
}  // end procedure
