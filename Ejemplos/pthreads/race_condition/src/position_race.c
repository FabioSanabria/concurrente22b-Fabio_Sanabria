// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>  // Already senn
#include <inttypes.h>  // Already senn
#include <pthread.h>  // Already senn
#include <stdint.h>  // Already senn
#include <stdio.h>  // Already senn
#include <stdlib.h>  // Already senn
#include <time.h>  // Already senn
#include <unistd.h>  // Already senn

// thread_shared_data_t
// (this is considered shared memory)
// In this struct we have the pthread_mutex
// that serves as a boolean to eliminate the
// race condition when serializing a part of the code
typedef struct shared_data {
  uint64_t position;  // position in which a thread arrived
  pthread_mutex_t can_access_position;
  uint64_t thread_count;
} shared_data_t;

// thread_private_data_t
typedef struct private_data {
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Subroutine that simulates a race between threads,
 *  where the order in which the position is printed is in ascending 
 * order and not indeterministic since the mutex is being used to
 *  eliminate the race code that affected this code so much

 * @param private_data_t data (Will be parsed)
 * 
 * @return NULL
 */
void* race(void* data);

/**
 * @brief Subroutine that creates all the threads that
 * we have to use in this program, if we dont put an 
 * especific number the program will create threads
 * as much as processors we have, this method has a lot
 * of fprintf if an error ocurred and free al the memory
 * that we use and do the thread_join
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
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
        // Error code
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->position = 0;  // Inicialize the position value in 0
    // Init the pthread mutex to be used in the subroutine race
    error = pthread_mutex_init(&shared_data->can_access_position, /*attr*/NULL);
    if (error == EXIT_SUCCESS) {
      shared_data->thread_count = thread_count;

      // The library time is used in this part of the code
      struct timespec start_time, finish_time;
      clock_gettime(CLOCK_MONOTONIC, &start_time);

      error = create_threads(shared_data);

      clock_gettime(CLOCK_MONOTONIC, &finish_time);

      // Process to obtain the time that we do all the operations
      double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
      // Print the cant of sgs that the program spend executing the program
      printf("Execution time: %.9lfs\n", elapsed_time);
      // Part where the mutex is released, when using the sanitizers
      // they didn't show any hint of a memory leak but better to be sure
      pthread_mutex_destroy(&shared_data->can_access_position);
      free(shared_data);
    } else {
        // Error code (error of create a mutex)
      fprintf(stderr, "Error: could not init mutex\n");
      return 13;
    }
  } else {
    // Error code (shared_memory)
    fprintf(stderr, "Error: could not allocate shared data\n");
    return 12;
  }
  return error;
}  // end procedure


int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  // Dinamic memory of the threads
  pthread_t* threads = (pthread_t*)
    malloc(shared_data->thread_count * sizeof(pthread_t));
    // Calloc: Inicialize all the memory in 0's
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
       // Inicialize of the atributes of private_data
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, race
        , /*arg*/ &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        // Error if cant create a secondary thread
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
        // Wait for all the threads and free them
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    free(private_data);  // free the memory used
    free(threads);  // free the memory used
  } else {
    // Print an erro if the program cannot allocate threads
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , shared_data->thread_count);
    error = 22;
  }

  return error;
}

// procedure greet:
void* race(void* data) {
  assert(data);
  // Assignation of data in private attributes
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  // lock(can_access_position)
  // Toda esta parte de codigo es serializada para que
  // no de problemas con el las condiciones descritas
  // abajo, el mutex lock es el inicio del puente, el
  // que entra es el owner del puente y cuando sale quita
  // el candado y entra otro hilo a realizar sus procesos
  pthread_mutex_lock(&shared_data->can_access_position);
  // race condition/data race/condición de carrera:
  // modificación concurrente de memoria compartida
  // position := position + 1
  ++shared_data->position;
  // my_position := position
  uint64_t my_position = shared_data->position;
  // print "Hello from secondary thread"
  printf("Thread %" PRIu64 "/%" PRIu64 ": I arrived at position %" PRIu64 "\n"
    , private_data->thread_number, shared_data->thread_count, my_position);

  // unlock(can_access_position)
  // En esta parte el hilo sale y le da espacio a otro hilo para entrar
  // y realizar sus procesos, luego de salir de este mutex, todo lo
  // demas se vuelve concurrente de nuevo
  pthread_mutex_unlock(&shared_data->can_access_position);
  // Gracias a esto el programa logra imprimir todo en orden
  // Si el print estuviera afuera habria problemas ya que el print
  // contiene un mutex escondido
  return NULL;
}  // end procedure
