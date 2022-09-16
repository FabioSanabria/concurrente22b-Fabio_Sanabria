// Copyright 2022 Fabio Andres Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr> CC-BY-4

#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>

// Struct of the node of the
// queue
typedef struct queue_node {
  size_t data;
  struct queue_node* next;
} queue_node_t;

// Struct of the queue that
// has the node of the head
// and the tail, also has a
// mutex to control the access
// of the queue
typedef struct {
  pthread_mutex_t can_access_queue;
  queue_node_t* head;
  queue_node_t* tail;
} queue_t;

/**
 * @brief Contructor of the queue that inicialize the mutex and put
 * the nodes in NULL in the begin
 * @param queue queue that we want to init
 * 
 * @return Code of error, Success if we can init the queue and
 * Failure if we cannot init the queue 
 */
int queue_init(queue_t* queue);

/**
 * @brief Destructor of the queue that clear all the queue
 * and destroy the mutex that we create in the init
 * To do a destroy we must do a init first
 * @param queue queue that we want to destroy
 * 
 * @return Code of error, Success if we can destroy the queue and
 * Failure if we cannot destroy the queue 
 */
int queue_destroy(queue_t* queue);

/**
 * @brief This subroutine sees if the queue is
 * empty, if is empty sends a true else send a
 * false
 * @remarks This subroutine is thread-safe
 * @param queue The queue that we want to know if is
 * empty
 * @return boolean that says if the queue is NULL or
 * not 
 */
bool queue_is_empty(queue_t* queue);

/**
 * @brief This subroutine enqueue elements into
 * the queue, allocate all the necesary memory to do
 * this process
 * @remarks This subroutine is thread-safe
 * @param queue The queue that we want to add an element
 * @param data The element that we want to add into the
 * queue, this data is a size_t type
 * @return Code of error, Success if we were able to
 *  add an element into the queue and Failure if we cannot 
 * add it
 */
int queue_enqueue(queue_t* queue, const size_t data);

/**
 * @brief This subroutine dequeue elements of
 * the queue, call remove first to do the hard
 * work
 * @remarks This subroutine is thread-safe
 * @param queue The queue that we want to eliminate an element
 * @param data The element that we want to eliminate of the
 * queue, this data is a size_t type
 * @return Code of error, Success if we were able
 * to eliminate the element of the queue
 * and Failure if we cannot eliminate it
 */
int queue_dequeue(queue_t* queue, size_t* data);

/**
 * @brief This subroutine clear all the queue, i.e
 * there is a while that goes into the queue and 
 * eliminates all the elements in it
 * @remarks This subroutine is thread-safe
 * @param queue The queue that we want to clear
 * @return NULL
 */
void queue_clear(queue_t* queue);

#endif  // QUEUE_H
