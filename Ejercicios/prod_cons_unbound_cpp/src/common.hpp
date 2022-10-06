// Copyright 2022 Fabio Andres Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr> CC-BY-4

#ifndef COMMON_HPP
#define COMMON_HPP

#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <mutex>
#include "queue.hpp"

// Declare the different types of errors
// Increaseing by 1
enum {
  ERR_NOMEM_SHARED = EXIT_FAILURE + 1,
  ERR_NOMEM_BUFFER,
  ERR_NO_ARGS,
  ERR_UNIT_COUNT,
  ERR_PRODUCER_COUNT,
  ERR_CONSUMER_COUNT,
  ERR_MIN_PROD_DELAY,
  ERR_MAX_PROD_DELAY,
  ERR_MIN_CONS_DELAY,
  ERR_MAX_CONS_DELAY,
  ERR_CREATE_THREAD,
};

// This is the struct of all the elements
// that we will use to simulate the proccess of
// consuming and producing
typedef struct simulation {
  size_t unit_count;
  size_t producer_count;
  size_t consumer_count;
  useconds_t producer_min_delay;
  useconds_t producer_max_delay;
  useconds_t consumer_min_delay;
  useconds_t consumer_max_delay;

  // queue that we will use in this example
  queue_t queue;
  // This is the mutex that will control the
  // access of the units that we produce and
  // create
  std::mutex can_access_next_unit;
  // Size of the next unit
  size_t next_unit;
  // Semaphore that will control the communication and concurence
  // of the consumer and producer
  sem_t can_consume;
  // Mutex of the consumed count
  // later i´ll explain it with detail
  std::mutex can_access_consumed_count;
  // Size of the consumed count
  size_t consumed_count;
} simulation_t;

/**
 * @brief Subroutine that takes a min number and a max number and
 * give us a random number between that 2 numbers that we passed in the
 * argument, this is used to know how long does the consumer and the 
 * producer has to wait each other
 * @param min  Minimal element in randomness
 * @param max  Maximal element in randomness
 * @return number between the min and max
 */
useconds_t random_between(useconds_t min, useconds_t max);

#endif  // COMMON_H
