#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <unistd.h>

// Children returning the lottery numbers
/*
    @brief
*/
void* lottery_grandson(void* data);


int main() {
    srand(time(NULL));
    pthread_t childson1;
    pthread_t childson2;
    int error = pthread_create(&childson1, /*attr*/ NULL, lottery_grandson, /*arg*/ NULL);
    error = pthread_create(&childson2, /*attr*/ NULL, lottery_grandson, /*arg*/ NULL);
    if(error == EXIT_SUCCESS) {
        printf("Grandma: Give me the numbers \n");
        size_t* random_number1; 
        size_t* random_number2; 
        pthread_join(childson1, /*value_ptr*/ (void*)&random_number1);
        pthread_join(childson2, /*value_ptr*/ (void*)&random_number2);
        printf("%zu\n", *random_number1);
        printf("%zu\n", *random_number2);
    }
    else{
        fprintf(stderr, "Error: could not create secondary thread\n");
    }

    return error;
}

void* lottery_grandson(void* data) {
    (void)data;
    size_t number_rand = (size_t)rand() % 100;   

    size_t* random_number = calloc(1, sizeof(size_t));

    random_number[0] = number_rand;

    return (void*)random_number;
}