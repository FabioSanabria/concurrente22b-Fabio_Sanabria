#ifndef STATISTICS_H
#define STATISTICS_H

typedef struct statistics statistics_t;

statistics_t* statistics_create(void); // Constructor
void statistics_destroy(statistics_t* statistics); // Destructor
int statistics_run(statistics_t* statistics,int argc,char* argv[]); // Run a mathematic process 
int statistics_analyze_arguments(statistics_t* statistics, int argc, char* argv[]);
int statistics_print_help();
int statistics_enqueue_file(const char* filename);
int statistics_analyze_file(statistics_t* statistics, FILE* file);
#endif