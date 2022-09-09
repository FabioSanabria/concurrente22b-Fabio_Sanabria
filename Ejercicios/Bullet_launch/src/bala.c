
// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0
#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
// Library that provide us everything related
// to random numbers
#include <sys/random.h>

// #include <unistd.h>

/**
 * @brief Subroutine that simulate a player throwing
 * a bullet 3 times in a row and take the best shot
 * of the player, also this subroutine is ony for the
 * players of the team 1,later the best times will be seen
 * by the main to chose the winner
 * @param data the number of the atlets that will fight for win
 * the first place
 * @return The best shot of the 3 shot done by the player
 * it will be returned into a false memory address 
 */
void* bulletLauch1(void* data);

/**
 * @brief Subroutine that simulate a player throwing
 * a bullet 3 times in a row and take the best shot
 * of the player, also this subroutine is ony for the
 * players of the team 2, later the best times will be seen
 * by the main to chose the winner
 * @param data the number of the atlets that will fight for win
 * the first place
 * @return The best shot of the 3 shot done by the player
 * it will be returned into a false memory address 
 */
void* bulletLauch2(void* data);

/**
 * @brief Subroutine that takes a min number and a max number and
 * give us a random number between that 2 numbers that we passed in the
 * argument
 * @param min  Minimal element in randomness
 * @param max  Maximal element in randomness
 * @return the random number
 */
double random_between(double min, double max);

int main(int argc, char* argv[]) {
    uint64_t atletas;  // Variable of atletas
    uint64_t points_team1 = 0;  // points of the team 1
    uint64_t points_team2 = 0;  // points of the team 2

    // In this part we see if the number put in the command
    // line is odd or pair, if is pair the process finish
    // if is odd the process continues normaly
    if (argc < 2)  // check number of arguments
        printf("No argument entered\n");
    else if (!isdigit(argv[1][0]))  // check first char of user arg
        printf("Not a number\n");
    else if (atoi(argv[1]) % 2 == 0)  // convert to int and test
        printf("Even\n");
    else
    // Pass the argument to the variable atletas
        sscanf(argv[1], "%" SCNu64, &atletas);
    pthread_t* threads = (pthread_t*) malloc(atletas * sizeof(pthread_t));
    // Create all the threads
    for (uint64_t thread_number = 0; thread_number < atletas
        ; ++thread_number) {
      // create_thread(greet, thread_number)
      pthread_create(&threads[thread_number], /*attr*/ NULL, bulletLauch1
        , /*arg*/ (void*)thread_number);

      pthread_create(&threads[thread_number], /*attr*/ NULL, bulletLauch2
        , /*arg*/ (void*)thread_number);
    }

    // Malloc of the team 1
    double* player1 = (double*)malloc(1 * sizeof(double));
    // Malloc of the team 2
    double* player2 = (double*)malloc(1 * sizeof(double));

    // Cycle that sees all the atletas and compare them to see who win
    // and adds points to the scoreboard
    // Cycle of joins that gives us all the random nums
    for (uint64_t thread_number = 0; thread_number < atletas
        ; ++thread_number) {
      // Join of the player of the team 1
      // Jugador del equipo 1
      pthread_join(threads[thread_number], (void*)&player1);

      // Join of the player of the team 2
      // Jugador del equipo 2
      pthread_join(threads[thread_number], (void*)&player2);

        // Compare who wins to add points into the points_team
      if (*player1 > *player2) {
          points_team1 += 1;
      } else {
        points_team2 += 1;
      }
    }
    // Observa que equipo tiene mas puntos y gana
    if (points_team1 > points_team2) {
        printf("result %"PRId64 ":%"PRId64 ", team 1 wins\n",
        points_team1, points_team2);
    } else {
        printf("result %"PRId64 ":%"PRId64 ", team 2 wins\n",
        points_team1, points_team2);
    }
    // Free all the memory used in this program
    free(player1);
    free(player2);
    free(threads);
}

// Subroutine of the team 1
void* bulletLauch1(void* data) {
    // The number of the player
    uint64_t atleta = (uint64_t) data;
    // This variable will take the biggest shot
    // of the player
    double numero_grande = 0;
    // Random shot of the player
    double number_rand;
    // Cycle that simulates the shots of the
    // player
    for (int i = 0; i < 3; i++) {
        // Method that give me a random in double
       number_rand = random_between(0.0, 25.0);
       // Compare the biggest number with the random
       // number
        if (numero_grande < number_rand) {
            numero_grande = number_rand;
        }
    }
    // Print the shot of the player
    printf("1.%"PRId64 ": best shot put %f m\n", atleta, numero_grande);
    // False memory address
    size_t* mejor_tiro = calloc(1, sizeof(size_t));  // Calloc to
    // troll the subroutine

    mejor_tiro[0] = numero_grande;  // False memory address

    // Sending the number like a memory address
    return (void*)mejor_tiro;  // Return the best shot
}

// Subroutine of the team 2
void* bulletLauch2(void* data) {
    // The number of the player
    uint64_t atleta = (uint64_t) data;
    // This variable will take the biggest shot
    // of the player
    double numero_grande = 0;
    // Random shot of the player
    double number_rand;
    // Cycle that simulates the shots of the
    // player
    for (int i = 0; i < 3; i++) {
        number_rand = random_between(0.0, 25.0);
       // Compare the biggest number with the random
       // number
        if (numero_grande < number_rand) {
            numero_grande = number_rand;
        }
    }
    // Print the shot of the player
    printf("2.%"PRId64 ": best shot put %f m\n", atleta, numero_grande);
    // False memory address
    size_t* mejor_tiro = calloc(1, sizeof(size_t));  // Troll the subroutine

    mejor_tiro[0] = numero_grande;  // False memory address

    // Sending the number like a memory address
    return (void*)mejor_tiro;  // Return the best shot
}

double random_between(double min, double max) {
  // Send a random unsecond_t
    double range = (max - min);  // takes the range
    double div = RAND_MAX / range;  // takes the divisor
    // the linter will hate this instruction
    return min + (rand() / div);  // And return the num
}
