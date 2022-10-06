// Copyright 2022 Fabio Andres Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr> CC-BY-4

#include <assert.h>
#include <errno.h>
#include <thread>
#include <pthread.h>
#include <stdlib.h>
#include <sys/random.h>
#include <stdio.h>
#include <vector>
#include "common.hpp"
#include "consumer.hpp"
#include "producer.hpp"
#include "simulation.hpp"

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
 * frees all the memory that consume all the std::threads
 * created in the method of creation
 * @param count Cant of std::thread that we will join
 * @param std::threads Part of the memory that the std::threads
 * are safed, they will join with a cycle
 * @return Error code
 */
int join_threads(size_t count, std::vector<std::thread>threads);

simulation_t* simulacion:: simulation_create() {
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
    simulation->next_unit = 0;
    sem_init(&simulation->can_consume, /* pshared */ 0, /* value */ 0);
    simulation->consumed_count = 0;
  }
  return simulation;
}

void simulacion::simulation_destroy(simulation_t* simulation) {
  assert(simulation);
  // Destroying all the mutex used and freeing
  // all the dinamic memory
  sem_destroy(&simulation->can_consume);
  queue_destroy(&simulation->queue);
  free(simulation);
}

int simulacion::simulation_run(simulation_t* simulation, int argc, char* argv[]) {
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

std::vector<std::thread> create_threads(size_t count, void*(*subroutine)(void*), void* data) {
  // Create the threads that we need to do all the process
  // We allocate memory in the heap
  std::vector<std::thread> threads;
  std::thread thread(subroutine, data);
    for (size_t index = 0; index < count; ++index) {
      threads.push_back(thread);
      // Wait for all the threads to finish their jobs
    }
  return threads;
}

int join_threads(size_t count, std::vector<std::thread>threads) {
  // Wait for each thread and then we eliminate all of
  // them
  for (size_t index = 0; index < count; ++index) {
    // todo: sum could not be right
    threads[index].join();
  }
  return 1;
}
int create_consumers_producers(simulation_t* simulation) {
  assert(simulation);
  int error = EXIT_SUCCESS;

  // Here we create the producers and do all the process inside
  // produce method
  std::vector<std::thread> producers = create_threads(simulation->producer_count, produce
    , simulation);
  // Here we create the consumers and do all the process inside
  // consume method
  std::vector<std::thread> consumers = create_threads(simulation->consumer_count, consume
    , simulation);

  // Join all the std::threads
    join_threads(simulation->producer_count, producers);
    join_threads(simulation->consumer_count, consumers);

  return error;

