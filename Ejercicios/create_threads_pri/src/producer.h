// Copyright 2022 Fabio Andres Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr> CC-BY-4

#ifndef PRODUCER_H
#define PRODUCER_H

/**
 * @brief Subroutine that simulates producing elements in the buffer at 
    a "X" speed while the consumer consums the elements 
    previously created by this method in "X" rounds, in this case
    we use all the tools that we have to control the concurrence like the mutex
    and the semaaphores
 * @param data simulation object that has all the arguments added by the user
 * 
 * @return NULL
 */
void* produce(void* data);

#endif  // PRODUCER_H
