// Copyright 2022 Fabio Andres Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr> CC-BY-4

#ifndef SIMULATION_HPP
#define SIMULATION_HPP


class simulacion : public producer, public consumer {
public:

/**
 * @brief Subroutine simulate a contructor of
 * simulation_t , we inicialize all the arguments
 * that the user provides with the line command in 0
 * and init the 2 mutex, 1 semaphore and the queue
 * @return simulation_t object
 */
simulation_t* simulation_create();

/**
 * @brief Principal Subroutine initializes all
 *  the processes necessary for the program to work,
 *  calls analyze arguments and contains the timer of the 
 * execution time
 * @param simulation Object that contains all, we
 * // create this object in the main
 * @param argc Number of the arguments provided
 * @param argv arguments that provides the user
 * @return Error code
 */
int simulation_run(simulation_t* simulation, int argc, char* argv[]);

/**
 * @brief Subroutine that simulates
 *a destructor in c++, we pass the
 simulation_T object to the subroutine
 and this will destroy and free all the
 dinamic memory to avoid the memory leaks 
 * @return simulation_t object
 */
void simulation_destroy(simulation_t* simulation);
private:
    // Nothing
};

#endif  // SIMULATION_HPP
