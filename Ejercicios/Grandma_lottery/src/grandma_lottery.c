// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // Time library that helps with the random numbers
// #include <unistd.h>

// Children returning the lottery numbers
/**
 * @brief Subroutine that simulates the 2 childs buying
 * lottery, this method uses rand to the random nums and
 * calloc to return the number
 * @return The number as a false memory address
 */
void* lottery_grandson(void* data);


int main() {
    srand(time(NULL));
    pthread_t childson1;
    pthread_t childson2;
    // Creates 2 threads of childson
    int error = pthread_create(&childson1, /*attr*/ NULL,
    lottery_grandson, /*arg*/ NULL);
    error = pthread_create(&childson2, /*attr*/ NULL,
    lottery_grandson, /*arg*/ NULL);

    // Grandma asking for the numbers to the childson
    if (error == EXIT_SUCCESS) {
        printf("Grandma: Give me the numbers \n");
        size_t* random_number1;  // Random number
        size_t* random_number2;  // Random number
        // Grandma waiting for the childsons
        pthread_join(childson1, /*value_ptr*/ (void*)&random_number1);
        pthread_join(childson2, /*value_ptr*/ (void*)&random_number2);
        // The childsons saying the numbers that they have
        printf("%zu\n", *random_number1);
        printf("%zu\n", *random_number2);
    } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
    }

    return error;
}

void* lottery_grandson(void* data) {
    (void)data;
    size_t number_rand = (size_t)rand() % 100;

    // False memory address
    size_t* random_number = calloc(1, sizeof(size_t));

    random_number[0] = number_rand;

    // Sending the number like a memory address
    return (void*)random_number;
}
