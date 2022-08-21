#include <stdio.h>
#include <stdlib.h>
#include "statistics.h"

int main(int argc, char* argv[])
{
    int error = EXIT_SUCCESS;
    statistics_t* statistics = statistics_create();
    if(statistics)
    {
        error = statistics_run(statistics, argc, argv);
        statistics_destroy(statistics);
    }
    else
    {
        fprintf(stderr, "error: could not allocate statistics object\n");
        error = 1;
    }
    return error;
}