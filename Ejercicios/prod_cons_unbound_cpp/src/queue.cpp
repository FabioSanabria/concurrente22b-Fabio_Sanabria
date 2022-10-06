// Copyright 2022 Fabio Andres Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr> CC-BY-4

#include <assert.h>
#include <stdlib.h>
#include <thread>
#include <mutex> 
#include "queue.hpp"

/**
 * @brief This private subroutine eliminates the first
 * node of the queue, at first the subroutine sees if
 * the queue is empty, if the queue has at least
 * one element, it will erase the first element there
 * @remarks This subroutine is thread-not-safe
 * @param queue The queue that we want to eliminate
 * the first element
 * @return NULL
 */
void queue_remove_first_unsafe(queue_t* queue);

/**
 * @brief This subroutine sees if the queue is
 * empty, if is empty sends a true else send a
 * false, the difference between this subroutine
 * and the public one is that this subroutine isn´t
 * safe by a mutex or something
 * @remarks This subroutine is thread-not-safe
 * @param queue The queue that we want to know if is
 * empty
 * @return boolean that says if the queue is NULL or
 * not 
 */
bool queue_is_empty_unsafe(queue_t* queue);

// Method of queue_init
void queue_init(queue_t* queue) {
  assert(queue);
  // Inicialize the mutex of the queue
  // int error = pthread_mutex_init(&queue->can_access_queue, /*attr*/ NULL);
  // Put the nodes in NULL
  queue->head = NULL;
  queue->tail = NULL;
}

// Method of queue_init
void queue_destroy(queue_t* queue) {
  // Call clear
  queue_clear(queue);
  // Return the destructor of mutex
}

bool queue_is_empty(queue_t* queue) {
  assert(queue);
  // Thread_Safe
  // Lost the reference of the first node
  
  queue->can_access_queue.lock();
  bool result = queue->head == NULL;
  queue->can_access_queue.unlock();
  return result;
}

bool queue_is_empty_unsafe(queue_t* queue) {
  // Thread_no_safe
  assert(queue);
  // Lost the reference of the first node
  return queue->head == NULL;
}

void queue_enqueue(queue_t* queue, const size_t data) {
  assert(queue);
  // Create a new node
  queue_node_t* new_node = (queue_node_t*) calloc(1, sizeof(queue_node_t));
  if (new_node) {
    // Put the new data inside of node
    new_node->data = data;
    // Thread_Safe
    queue->can_access_queue.lock();
    // Sees if there is a tail
    if (queue->tail) {
      // Put the node in the tail
      queue->tail = queue->tail->next = new_node;
    } else {
      // Put the node in the head
      queue->head = queue->tail = new_node;
    }
    // Unlock the mutex
    queue->can_access_queue.unlock();
  } else {
  }
}

void queue_dequeue(queue_t* queue, size_t* data) {
  assert(queue);
  // Thread_Safe
  queue->can_access_queue.lock();
  // Sees if the queue is empty
  // We use the unsafe version because
  // this method can create a "bello durmiente"
  // (sleepy beauty)
  if (!queue_is_empty_unsafe(queue)) {
    if (data) {
      *data = queue->head->data;
    }
    // Here again we use the unsafe to avoid problems
    queue_remove_first_unsafe(queue);
  } else {
  }
  // End of the critical zone
  queue->can_access_queue.unlock();
}

void queue_remove_first_unsafe(queue_t* queue) {
  assert(queue);
  // The unsafe method only can use unsafe methods
  assert(!queue_is_empty_unsafe(queue));
  // At this part we copy the head in the new
  // node and the we free the node
  queue_node_t* node = queue->head;
  queue->head = queue->head->next;
  free(node);
  if (queue->head == NULL) {
    queue->tail = NULL;
  }
}

void queue_clear(queue_t* queue) {
  assert(queue);
  // Thread_safe
  queue->can_access_queue.lock();
  // In this cycle we are eliminating all
  // the elements 1 by 1
  while (!queue_is_empty_unsafe(queue)) {
    queue_remove_first_unsafe(queue);
  }
  queue->can_access_queue.unlock();
}
