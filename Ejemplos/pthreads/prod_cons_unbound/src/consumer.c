// Copyright 2022 Fabio Andres Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr> CC-BY-4

#include <stdbool.h>
#include <stdio.h>

#include "common.h"
#include "consumer.h"

// Subroutine of consuming products
void* consume(void* data) {
  // Parse the data
  simulation_t* simulation = (simulation_t*)data;
  // Variante 1 (Infinite cycle)
  while (true) {
    // lock(can_access_consumed_count)
    // We lock this part of the code to avoid the data race of
    // the "if" and the increment of consumed count
    pthread_mutex_lock(&simulation->can_access_consumed_count);
    if (simulation->consumed_count < simulation->unit_count) {
      // Reserve the next product to me
      ++simulation->consumed_count;
    } else {
      // unlock(can_access_consumed_count) we dont want to create
      // a "bello durmiente" here we unlock beacause there is a
      // posibility of enter in this else
      pthread_mutex_unlock(&simulation->can_access_consumed_count);
      // break while
      break;
    }
    // unlock(can_access_consumed_count) we dont want to create
    // a "bello durmiente"
    pthread_mutex_unlock(&simulation->can_access_consumed_count);

    // wait(can_consume)
    sem_wait(&simulation->can_consume);

    size_t value = 0;
    // Here dont need to protect this part of the code
    // with a mutex because this is already protected
    queue_dequeue(&simulation->queue, &value);
    printf("\tConsuming %zu\n", value);  // Print of consuming
    // Delay of the consumers
    usleep(1000 * random_between(simulation->consumer_min_delay
      , simulation->consumer_max_delay));
  }

  return NULL;
}
