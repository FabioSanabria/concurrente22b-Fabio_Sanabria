// Copyright 2022 Fabio Andres Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr> CC-BY-4

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/random.h>
#include <stdio.h>

#include "common.h"
#include "consumer.h"
#include "producer.h"
#include "simulation.h"

/**
 * @brief Subroutine that checks if the arguments
 * gived by the user are correct or not, if
 * one of the arguments is incorrect, this method
 * will send a error message
 * @param simulation object of simulation_t
 * @param argc Number of the arguments provided
 * @param argv arguments that provides the user
 * @return Error code
 */
int analyze_arguments(simulation_t* simulation, int argc, char* argv[]);

/**
 * @brief This subroutine create all the consumers
 * and the producers that the user wants to have,
 * when we create all the consumers and producers
 * they will do all the process that the consume
 * and produce method do, then they return and
 * to call the join method
 * @param simulation object of simulation_t
 * @return Error code
 */
int create_consumers_producers(simulation_t* simulation);

/**
 * @brief This subroutine call the join function that
 * frees all the memory that consume all the threads
 * created in the method of creation
 * @param count Cant of thread that we will join
 * @param threads Part of the memory that the threads
 * are safed, they will join with a cycle
 * @return Error code
 */
int join_threads(size_t count, private_data_t* threads);

simulation_t* simulation_create() {
  simulation_t* simulation = (simulation_t*) calloc(1, sizeof(simulation_t));
  if (simulation) {
    // I dont documment that to avoid the
    // reiteration and the obvious
    simulation->unit_count = 0;
    simulation->producer_count = 0;
    simulation->consumer_count = 0;
    simulation->producer_min_delay = 0;
    simulation->producer_max_delay = 0;
    simulation->consumer_min_delay = 0;
    simulation->consumer_max_delay = 0;
    // Inicialize all the control of concurrence
    queue_init(&simulation->queue);
    pthread_mutex_init(&simulation->can_access_next_unit, /* attr */ NULL);
    simulation->next_unit = 0;
    sem_init(&simulation->can_consume, /* pshared */ 0, /* value */ 0);
    pthread_mutex_init(&simulation->can_access_consumed_count, /* attr */ NULL);
    simulation->consumed_count = 0;
  }
  return simulation;
}

void simulation_destroy(simulation_t* simulation) {
  assert(simulation);
  // Destroying all the mutex used and freeing
  // all the dinamic memory
  pthread_mutex_destroy(&simulation->can_access_consumed_count);
  sem_destroy(&simulation->can_consume);
  pthread_mutex_destroy(&simulation->can_access_next_unit);
  queue_destroy(&simulation->queue);
  free(simulation);
}

int simulation_run(simulation_t* simulation, int argc, char* argv[]) {
  // Callc analyze_arguments
  int error = analyze_arguments(simulation, argc, argv);
  if (error == EXIT_SUCCESS) {
    unsigned int seed = 0;
    getrandom(&seed, sizeof(seed), GRND_NONBLOCK);
    srandom(seed);
    // The clock that gives the time
    struct timespec start_time;
    clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

    error = create_consumers_producers(simulation);

    struct timespec finish_time;
    clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

    double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
    printf("execution time: %.9lfs\n", elapsed);
  }
  return error;
}

int analyze_arguments(simulation_t* simulation, int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // The user pass all the arguments by the command line and we
  // safe all of the in the respective variables
  // producer_consumer buffer_capacity rounds"
  //" producer_min_delay producer_max_delay"
  //" consumer_min_delay consumer_max_delay
  if (argc == 8) {
    if (sscanf(argv[1], "%zu", &simulation->unit_count) != 1
      || simulation->unit_count == 0) {
        fprintf(stderr, "error: invalid unit count\n");
        error = ERR_UNIT_COUNT;
    } else if (sscanf(argv[2], "%zu", &simulation->producer_count) != 1
      || simulation->producer_count == 0) {
        fprintf(stderr, "error: invalid producer count\n");
        error = ERR_PRODUCER_COUNT;
    } else if (sscanf(argv[3], "%zu", &simulation->consumer_count) != 1
      || simulation->consumer_count == 0) {
        fprintf(stderr, "error: invalid consumer count\n");
        error = ERR_CONSUMER_COUNT;
    } else if (sscanf(argv[4], "%u", &simulation->producer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min producer delay\n");
        error = ERR_MIN_PROD_DELAY;
    } else if (sscanf(argv[5], "%u", &simulation->producer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max producer delay\n");
        error = ERR_MAX_PROD_DELAY;
    } else if (sscanf(argv[6], "%u", &simulation->consumer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min consumer delay\n");
        error = ERR_MIN_CONS_DELAY;
    } else if (sscanf(argv[7], "%u", &simulation->consumer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max consumer delay\n");
        error = ERR_MAX_CONS_DELAY;
    }
  } else {
    // Error if we do something wrong
    fprintf(stderr, "usage: producer_consumer buffer_capacity rounds"
      " producer_min_delay producer_max_delay"
      " consumer_min_delay consumer_max_delay\n");
      error = ERR_NO_ARGS;
  }
  return error;
}

private_data_t* create_threads(size_t count,
void*(*subroutine)(void*), void* data) {
  // Create the threads that we need to do all the process
  // We allocate memory in the heap
    // Create a private_data array that will contro all the attributes
    pthread_t* threads = (pthread_t*) calloc(count, sizeof(pthread_t));

    private_data_t* private_data = (private_data_t*)
    calloc(count, sizeof(private_data_t));

  if (threads) {
    for (size_t thread_number = 0; thread_number < count; ++thread_number) {
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].simulation = data;
      private_data[thread_number].thread = &threads[thread_number];
      private_data[thread_number].cant_hilos = count;
      if (pthread_create(private_data[thread_number].thread, /*attr*/ NULL,
      subroutine, &private_data[thread_number])
      == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "error: could not create thread %zu\n", thread_number);
        join_threads(thread_number, private_data);
        free(threads);
        return NULL;
      }
    }
  }
  return private_data;
}

int join_threads(size_t count, private_data_t* threads) {
  int error = EXIT_SUCCESS;
  // Wait for each thread and then we eliminate all of
  // them
  for (size_t index = 0; index < count; ++index) {
    // todo: sum could not be right
    error += pthread_join(*(threads[index].thread), /*value_ptr*/ NULL);
  }
  free(threads);
  return error;
}

int create_consumers_producers(simulation_t* simulation) {
  assert(simulation);
  int error = EXIT_SUCCESS;

  // Here we create the producers and do all the process inside
  // produce method
  private_data_t* producers = create_threads(simulation->producer_count, produce
    , simulation);
  // Here we create the consumers and do all the process inside
  // consume method
  private_data_t* consumers = create_threads(simulation->consumer_count, consume
    , simulation);

  // Join all the threads
  if (producers && consumers) {
    join_threads(simulation->producer_count, producers);
    join_threads(simulation->consumer_count, consumers);
  } else {
    fprintf(stderr, "error: could not create threads\n");
    error = ERR_CREATE_THREAD;
  }

  return error;
}
