// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> and
// Markus Östberg git: markusos CC-BY 4.0

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<unistd.h>
#include <list>
#include "mpi.h"
#include "UniformRandom.hpp"
/// Signature
void philosopher(int process_number);
/// Signature
void table(int process_number, int process_count);
// Define Macros
#define FORK_REQUEST 1
#define FORK_RESPONSE 2
#define FORK_RELEASE 3
#define DEBUG 1

int main(int argc, char** argv) {
    int process_number, process_count;
    // init MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    // Depending on rand, Filosofo or Table
    if (process_number == 0) {
       table(process_number, process_count);
    } else {
        // Aqui solo permite que sean 5 procesos
        if (process_number < 5) {
            philosopher(process_number);
        }
    }
    MPI_Finalize();
    return 0;
}
/* Filosofo function - only Filosofo processes run this */
void philosopher(int process_number) {
    printf("Hello from Filosofo %d \n", process_number);
    int in_buffer[1];
    int out_buffer[1];
    MPI_Status stat;
    srand(time(NULL) + process_number);
    UniformRandom<int> uniformRandom;
    // Filosofo main loop
    while (true) {
        printf("Filosofo %d esta dormido \n", process_number);
        // "THINK METHOD"
        sleep(uniformRandom.between(4, 10));  // Sleep
        printf("Filosofo %d esta esperando para comer \n", process_number);
        // Request forks
        MPI_Send(out_buffer, 1, MPI_INT, 0, FORK_REQUEST, MPI_COMM_WORLD);
        // Whait for response
        MPI_Recv(in_buffer, 1, MPI_INT, 0, FORK_RESPONSE,
        MPI_COMM_WORLD, &stat);
        printf("Filosofo %d esta comiendo \n", process_number);
        // / "THINK METHOD"
        sleep(uniformRandom.between(4, 10));  // Eat

        printf("Filosofo %d termino de comer \n", process_number);

        MPI_Send(out_buffer, 1, MPI_INT, 0, FORK_RELEASE,
        MPI_COMM_WORLD);  // Release forks
    }
}

/* Table function - only table process run this */
void table(int process_number, int process_count) {
    printf("Hello from table %d \n", process_number);
    int in_buffer[1];
    int out_buffer[1];
    int philosopher;
    MPI_Status stat;

    std::list<int> queue;
    // Aqui da error el linter
    bool fork[process_count-1];
    // Init all forks as free
    for (int i = 0; i < process_count-1; i++) fork[i] = true;
    // Table main loop
    while (true) {
        MPI_Recv(in_buffer, 1, MPI_INT, MPI_ANY_SOURCE,
        MPI_ANY_TAG , MPI_COMM_WORLD, &stat);  // Recive next message
        philosopher = stat.MPI_SOURCE;  // Read source of message

        if (stat.MPI_TAG == FORK_REQUEST) {  // If Request for forks
            printf("EL filosofo %d pidio los palillos\n", philosopher);
            // If both forks are free
            if (fork[philosopher%(process_count-1)] == true
            && fork[philosopher-1] == true) {
                // Set the forks as taken
                fork[philosopher%(process_count-1)] = false;
                fork[philosopher-1] = false;
            // Send Fork response to the right Filosofo
                MPI_Send(out_buffer, 1, MPI_REAL, philosopher,
                FORK_RESPONSE, MPI_COMM_WORLD);
                printf("El filosofo %d logro recibir los palillos\n",
                philosopher);
            } else {  // If not both forks are free
                queue.push_back(philosopher);  // Put in wait queue
            }
        }
        if (stat.MPI_TAG == FORK_RELEASE) {  // If Release of forks
            // Set forks to free again
            fork[philosopher%(process_count-1)] = true;
            fork[philosopher-1] = true;
            printf("El filosofo %d libero los palillos\n", philosopher);

            if (!queue.empty()) {  // If Filosofo whaiting for forks
            // Go through whole list of whaiting Filosofos
                for (std::list<int>::iterator it = queue.begin();
                it != queue.end(); it++) {
                    philosopher = *it;
                    // If one of them can get both forks
                    if (fork[philosopher%(process_count-1)] == true &&
                    fork[philosopher-1] == true) {
                        fork[philosopher%(process_count-1)] = false;
                        fork[philosopher-1] = false;
                        MPI_Send(out_buffer, 1, MPI_INT, philosopher,
                        FORK_RESPONSE, MPI_COMM_WORLD);  // send Fork response

                        printf("El filosofo %d logro recibir los palillos\n",
                        philosopher);
                        it = queue.erase(it);  // Remove from wait list
                    }
                }
            }
        }
    }
}
