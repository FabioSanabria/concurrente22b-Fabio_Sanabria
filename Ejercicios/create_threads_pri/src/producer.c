// Copyright 2022 Fabio Andres Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr> CC-BY-4

#include <stdio.h>

#include "common.h"
#include "producer.h"

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  private_data_t* private_data = (private_data_t*)data;
  simulation_t* simulation = private_data->simulation;

  while (true) {
    // declare my_unit := 0
    size_t my_unit = 0;
    // lock(can_access_consumed_count)
    // We lock this part of the code to avoid the data race of
    // the "if" and the increment of the next_unit
    pthread_mutex_lock(&simulation->can_access_next_unit);
    // If is there pending work, take a unit for producing
    if (simulation->next_unit < simulation->unit_count) {
      // Posible data race
      my_unit = ++simulation->next_unit;
    } else {
      // unlock(can_access_next_unit) we dont want to create
      // a "bello durmiente" here we unlock beacause there is a
      // posibility of enter in this else
      pthread_mutex_unlock(&simulation->can_access_next_unit);
      // break while
      break;
    }
      // unlock(can_access_next_unit) we dont want to create
      // a "bello durmiente"
    pthread_mutex_unlock(&simulation->can_access_next_unit);

    // Delay of the producers to create products
    usleep(1000 * random_between(simulation->producer_min_delay
      , simulation->producer_max_delay));
    // Here dont need to protect this part of the code
    // with a mutex because this is already protected
    queue_enqueue(&simulation->queue, my_unit);
    printf("Producer thread %"PRId64" of %zu Produced %zu\n",
    private_data->thread_number ,
    private_data->simulation->producer_count, my_unit);

    // signal(can_consume)
    // Send the signal to the consumer that he
    // can consume again
    sem_post(&simulation->can_consume);
  }

  return NULL;
}
