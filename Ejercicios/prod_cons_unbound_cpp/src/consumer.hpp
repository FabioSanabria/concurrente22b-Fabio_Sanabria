// Copyright 2022 Fabio Andres Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr> CC-BY-4

#ifndef CONSUMER_HPP
#define CONSUMER_HPP

#include <hilos.hpp>
class consumer: public hilos {
public:
   /**
 * @brief Subroutine that simulates consuming elements in the buffer at 
    a "X" speed while the producer produces new elements in "X" rounds
    and consums the elemets that the produce subroutine produces, in this case
    we use all the tools that we have to control the concurrence like the mutex
    and the semaaphores
 * @param data simulation object that has all the arguments added by the user
 * 
 * @return NULL
 */
  virtual void* consume(void* data);
private:
};

#endif  // CONSUMER_H
