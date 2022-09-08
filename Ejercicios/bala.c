// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0
#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/random.h>

// #include <unistd.h>

// Children returning the lottery numbers
/**
 * @brief Subroutine that simulates the 2 childs buying
 * lottery, this method uses rand to the random nums and
 * calloc to return the number
 * @return The number as a false memory address
 */


void* bulletLauch1(void* data);
void* bulletLauch2(void* data);

int main(int argc, char* argv[]) {

    uint64_t atletas;
    uint64_t points_team1 = 0;
    uint64_t points_team2 = 0;
    atletas = 3;
    unsigned int seed = 0u;
    getrandom(&seed, sizeof(seed), GRND_NONBLOCK);
    srandom(seed);
    //if (argc == 2) {
        //if((int)argv[1] % 2 != 0) {
          //  atletas = (int)argv[1];
        //} else {   
         //   fprintf(stderr, "Error: athlete count must be odd\n");                  
       // }   
    //} else {
      //fprintf(stderr, "Error: invalid thread count\n");
      //return 11;
    //}

    pthread_t* threads = (pthread_t*) malloc(atletas * sizeof(pthread_t));
    for (uint64_t thread_number = 0; thread_number < atletas // Atletas del equipo 1
        ; ++thread_number) {
      // create_thread(greet, thread_number)
      pthread_create(&threads[thread_number], /*attr*/ NULL, bulletLauch1
        , /*arg*/ (void*)thread_number);

      pthread_create(&threads[thread_number], /*attr*/ NULL, bulletLauch2
        , /*arg*/ (void*)thread_number);  
    }

    size_t* jugador1;
    size_t* jugador2; 

    for (uint64_t thread_number = 0; thread_number < atletas // Atletas del equipo 1
        ; ++thread_number) {
      
      pthread_join(threads[thread_number], (void*)&jugador1);  // Jugador del equipo 1

      pthread_join(threads[thread_number], (void*)&jugador2);  // Jugador del equipo 2

      if(*jugador1 > *jugador2) {
          points_team1 += 1;  
      } else { 
        points_team2 += 1;
      }  
    }

    if(points_team1 > points_team2) {
        printf("result %"PRId64 ":%"PRId64 ", team 1 wins", points_team1, points_team2);
    } else {
        printf("result %"PRId64 ":%"PRId64 ", team 2 wins", points_team1, points_team2);
    }

    free(threads);
}

void* bulletLauch1(void* data) {
    uint64_t atleta = (uint64_t) data;
    size_t numero_grande = 0;
    size_t number_rand;

    for(int i = 0; i < 3; i++) {
        number_rand = (size_t)rand() % 20;
        if(numero_grande < number_rand) {
            numero_grande = number_rand;
        }
    }
    printf("1.%"PRId64 ": best shot put %zu m\n", atleta, numero_grande);
    // False memory address
    size_t* mejor_tiro = calloc(1, sizeof(size_t));

    mejor_tiro[0] = number_rand;

    // Sending the number like a memory address
    return (void*)mejor_tiro;
}

void* bulletLauch2(void* data) {
    uint64_t atleta = (uint64_t) data;
    size_t numero_grande = 0;
    size_t number_rand;

    for(int i = 0; i < 3; i++) {
        number_rand = (size_t)rand() % 20;
        if(numero_grande < number_rand) {
            numero_grande = number_rand;
        }

    }
    printf("2.%"PRId64 ": best shot put %zu m\n", atleta, numero_grande);
    // False memory address
    size_t* mejor_tiro = calloc(1, sizeof(size_t));

    mejor_tiro[0] = number_rand;

    // Sending the number like a memory address
    return (void*)mejor_tiro;
}