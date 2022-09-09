// Copyright 2022 Fabio Sanabria<fabio.sanabria@ucr.ac.cr> CC-BY-4
// Simulates a producer and a consumer that share a bounded buffer

// @see `man feature_test_macros`
#define _DEFAULT_SOURCE

#include <assert.h>  // Already seen
#include <errno.h>  // Already seen
#include <pthread.h>  // Already seen
#include <semaphore.h>  // Already seen
#include <stdio.h>  // Already seen
#include <stdlib.h>  // Already seen
// Library that provide us everything related
// to random numbers
#include <sys/random.h>
#include <unistd.h>  // Already seen

enum {
  // Declare the different types of errors
  // Increaseing by 1
  ERR_NOMEM_SHARED = EXIT_FAILURE + 1,
  ERR_NOMEM_BUFFER,
  ERR_NO_ARGS,
  ERR_BUFFER_CAPACITY,
  ERR_ROUND_COUNT,
  ERR_MIN_PROD_DELAY,
  ERR_MAX_PROD_DELAY,
  ERR_MIN_CONS_DELAY,
  ERR_MAX_CONS_DELAY,
  ERR_CREATE_THREAD,
};

// Struct of shared_data_t
// Shared memory between threads
typedef struct {
  size_t thread_count;  // Cant of threads
  size_t buffer_capacity;  // capacity of the buffer
  double* buffer;  // Temporal array
  size_t rounds;  // The number of rounds, laps, or
  // times the buffer should be filled and emptied.
  useconds_t producer_min_delay;  // The minimum duration in milliseconds
  // that the producer takes to generate a product.
  useconds_t producer_max_delay;  // The maximum duration in milliseconds
  // that the producer takes to generate a product.
  useconds_t consumer_min_delay;  // The minimum duration in milliseconds
  // that the consumer takes to consume a product.
  useconds_t consumer_max_delay;  // The maximum duration in milliseconds
  // that the consumer takes to consume a product.

  sem_t can_produce;  // semaphore that tells threads if they can produce or not
  sem_t can_consume;  // semaphore that tells threads if they can consume or not
} shared_data_t;

// Struct of private_data_t
// Private of each thread
typedef struct {
  size_t thread_number;  // Number of thread
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Take the arguments of the terminal and analyze if argc and argv are valid
 * @param shared_data Pointer to a shared_data_t object
 * @param argc NUmber of arguments
 * @param argv Array with the arguments 
 * @return An error code, EXIT_SUCCESS if the code run correctly or 
 * one of all errors declared int line 18 if the code fails in this method
*/
int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);

/**
 * @brief Subroutine that creates threads, in this case
 * we create 2 threads, producer and consumer to simulete the 
 * algorithm "productor-consumidor acotado", we see if there is 
 * an error and report that and finally we wait for the threads and
 * free all of them
 * @param shared_data_t shared_data
 * 
 * @return error code
 */
int create_threads(shared_data_t* shared_data);

/**
 * @brief Subroutine that simulates producing elements in the buffer at 
a "X" speed to later be consumed by a consumer, within this method there 
is a semaphore that allows communication between produce and consume and in
consume happens the same thing as in producing
 * @param shared_data_t shared_data (parsed)
 * 
 * @return NULL
 */
void* produce(void* data);

/**
 * @brief Subroutine that simulates consuming elements in the buffer at 
a "X" speed while the producer produces new elements in "X" rounds, within this method there 
is a semaphore that allows communication between produce and consume and in
consume happens the same thing as in consuming
 * @param shared_data_t shared_data (parsed)
 * 
 * @return NULL
 */
void* consume(void* data);

/**
 * @brief Subroutine that takes a min number and a max number and
 * give us a random number between that 2 numbers that we passed in the
 * argument
 * @param min  Minimal element in randomness
 * @param max  Maximal element in randomness
 * @return NULL
 */
useconds_t random_between(useconds_t min, useconds_t max);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;

  // Dinamic memory of shared data
  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));

  if (shared_data) {
    // Call to analyze the argumets
    error = analyze_arguments(argc, argv, shared_data);
    if (error == EXIT_SUCCESS) {
      // Dinamic memory of the buffer
      shared_data->buffer = (double*)
        calloc(shared_data->buffer_capacity, sizeof(double));
      if (shared_data->buffer) {
        // Inicialize the sem of can produce
        sem_init(&shared_data->can_produce, /*pshared*/ 0,
          shared_data->buffer_capacity);
        // Inicialize the sem of can consume
        sem_init(&shared_data->can_consume, /*pshared*/ 0, /*value*/ 0);

        // Semilla
        unsigned int seed = 0u;
        // Call the random method
        getrandom(&seed, sizeof(seed), GRND_NONBLOCK);
        srandom(seed);
        // Execution time of the program
        struct timespec start_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

        error = create_threads(shared_data);

        struct timespec finish_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

        // Print the execution time of the program
        double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        printf("execution time: %.9lfs\n", elapsed);

        // Free the memory of the sem of can_consume
        sem_destroy(&shared_data->can_consume);
        // Free the memory of the sem of can_produce
        sem_destroy(&shared_data->can_produce);
        free(shared_data->buffer);  // Free the buffer
      } else {
        fprintf(stderr, "error: could not create buffer\n");
        error = ERR_NOMEM_BUFFER;
      }
    }

    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = ERR_NOMEM_SHARED;
  }

  return error;
}

int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data) {
  // Analyze all the different types of errors in the arguments line
  int error = EXIT_SUCCESS;
  if (argc == 7) {
    if (sscanf(argv[1], "%zu", &shared_data->buffer_capacity) != 1
      || shared_data->buffer_capacity == 0) {
        fprintf(stderr, "error: invalid buffer capacity\n");
        error = ERR_BUFFER_CAPACITY;
    } else if (sscanf(argv[2], "%zu", &shared_data->rounds) != 1
      || shared_data->rounds == 0) {
        fprintf(stderr, "error: invalid round count\n");
        error = ERR_ROUND_COUNT;
    } else if (sscanf(argv[3], "%u", &shared_data->producer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min producer delay\n");
        error = ERR_MIN_PROD_DELAY;
    } else if (sscanf(argv[4], "%u", &shared_data->producer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max producer delay\n");
        error = ERR_MAX_PROD_DELAY;
    } else if (sscanf(argv[5], "%u", &shared_data->consumer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min consumer delay\n");
        error = ERR_MIN_CONS_DELAY;
    } else if (sscanf(argv[6], "%u", &shared_data->consumer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max consumer delay\n");
        error = ERR_MAX_CONS_DELAY;
    }
  } else {
    fprintf(stderr, "usage: prod_cons_bound buffer_capacity rounds"
      " producer_min_delay producer_max_delay"
      " consumer_min_delay consumer_max_delay\n");
      error = ERR_NO_ARGS;
  }
  return error;
}

int create_threads(shared_data_t* shared_data) {
  assert(shared_data);
  int error = EXIT_SUCCESS;

  pthread_t producer, consumer;
  // Create the producer
  error = pthread_create(&producer, /*attr*/ NULL, produce, shared_data);
  if (error == EXIT_SUCCESS) {
    // Create the consumer
    error = pthread_create(&consumer, /*attr*/ NULL, consume, shared_data);
    if (error != EXIT_SUCCESS) {
      fprintf(stderr, "error: could not create consumer\n");
      error = ERR_CREATE_THREAD;
    }
  } else {
    fprintf(stderr, "error: could not create producer\n");
    error = ERR_CREATE_THREAD;
  }

  if (error == EXIT_SUCCESS) {
    // Wait for the threads and free them
    pthread_join(producer, /*value_ptr*/ NULL);
    pthread_join(consumer, /*value_ptr*/ NULL);
  }

  return error;
}

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)data;
  size_t count = 0;
  // Iterate the rounds that the user puts in the arguments
  for (size_t round = 0; round < shared_data->rounds; ++round) {
    for (size_t index = 0; index < shared_data->buffer_capacity; ++index) {
      // wait(can_produce)
      sem_wait(&shared_data->can_produce);  // Sees if we can produce
      // Code to produce
      usleep(1000 * random_between(shared_data->producer_min_delay
        , shared_data->producer_max_delay));
      shared_data->buffer[index] = ++count;
      printf("Produced %lg\n", shared_data->buffer[index]);

      // signal(can_consume)
      sem_post(&shared_data->can_consume);  // Consume can take elements
    }
  }

  return NULL;
}

void* consume(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)data;
  for (size_t round = 0; round < shared_data->rounds; ++round) {
    for (size_t index = 0; index < shared_data->buffer_capacity; ++index) {
      // wait(can_consume)
      sem_wait(&shared_data->can_consume);  // Sees if we can consume
      // Code to consume
      double value = shared_data->buffer[index];
      usleep(1000 * random_between(shared_data->consumer_min_delay
        , shared_data->consumer_max_delay));
      printf("\tConsumed %lg\n", value);

      // signal(can_produce)
      // Send a signal to can produce to continue
      sem_post(&shared_data->can_produce);
      // their process
    }
  }

  return NULL;
}

useconds_t random_between(useconds_t min, useconds_t max) {
  // Send a random unsecond_t
  return min + (max > min ? (random() % (max - min)) : 0);
}
