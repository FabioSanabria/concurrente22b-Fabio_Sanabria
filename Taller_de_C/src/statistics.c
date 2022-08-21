#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <array_double.h>

#include "statistics.h"

const char* const statistics_help = 
"stats v1.0.0 Jeisson Hidalgo <jeisson.hidalgo@ucer.ac.cr>\n"
"usage: stats [-b] [FILES]\n"
"\n"
"Options:\n"
"   -b  Data is binary\n"
;

typedef struct statistics_file
{
    char* filename;
    struct statistics_file* next

}statistics_file_t;

void statistics_print(const statistics_t* statistics);

typedef struct statistics // This makes private attributes
{
    bool help_asked;
    bool is_binary;
    statistics_file_t *files_head;
    statistics_file_t *files_tail;
    size_t file_count;
    array_double_t values;
} statistics_t;


statistics_t* statistics_create(void)
{
    statistics_t* statistics = (statistics_t*)
    calloc(1, sizeof(statistics_t));
    if(statistics)
    {
        statistics->help_asked = false;
        statistics->is_binary = false;
        statistics->files_head = NULL;
        statistics->files_tail = NULL;
        statistics->file_count = 0;
        array_double_init(&statistics->values);
    }
    return statistics;
}

void statistics_destroy(statistics_t* statistics)
{
    assert(statistics);
    for(statistics_file_t* current = statistics->files_head; current; current = statistics->files_head)
    {
        statistics->files_head = current->next;
        free(current);
    }
    array_double_destroy(&statistics->values);
    free(statistics);
}

int statistics_print_help()
{
    printf("%s", statistics_help); 
}

int statistics_run(statistics_t* statistics,int argc,char* argv[])
{
    int error = statistics_analyze_arguments(statistics,argc, argv);
    if(error == EXIT_SUCCESS)
    {
      if(statistics->help_asked)
      {
        return statistics_print_help();
      }

    if(statistics->file_count > 0)
    {
        for(statistics_file_t* current = statistics->files_head; current; current = current->next)
      {
        const char* filename = current->filename;
        const char* open_mode = statistics->is_binary ? "rb" : "rt";
        FILE* file = fopen(filename, open_mode);
        if(file)
        {
            printf("processing %s...\n", filename);
            statistics_analyze_file(statistics, file);
            fclose(file);
        }
        else
        {
            fprintf(stderr, "error: could not open %s\n", filename);
            error = 20; 
            //break; ??
        }
      }  
    }
    else
    {
        if(statistics->is_binary)
        {
            freopen("", "rb", stdin); 
        }
        statistics_analyze_file(statistics, stdin);
        printf("processing stdin...\n");
    }
   }
    if(error == EXIT_SUCCESS)
    {
        statistics_print(statistics);
    }    
    return error;
}


int statistics_analyze_arguments(statistics_t* statistics, int argc, char* argv[])
{
    assert(statistics); 
    
    for(int index = 1 ; index < argc; ++index)
    {
        //printf("argv[%d] == [%s]\n", index, argv[index]);
        if(strcmp(argv[index], "--help") == 0)
        {
            statistics->help_asked = true;
            break;
        }

        else if(strcmp(argv[index], "-b") == 0)
        {
            statistics->is_binary = true;
        }
        else if(*argv[index] == '-') //argv[index][0]
        {
            fprintf(stderr, "error: unknown option: %s\n", argv[index]);
            return EXIT_FAILURE;
        }
        else
        {
            statistics_enqueue_file(statistics, argv[index]);
        }
    }

    return EXIT_SUCCESS;
}

int statistics_enqueue_file(statistics_t* statistics, const char* filename)
{
    assert(statistics);

    statistics_file_t* node = (statistics_file_t*)
    calloc(1, sizeof(statistics_file_t));
    
    if(node)
    {
        node->filename = filename;

        if(statistics->files_tail == NULL)
        {
            statistics->files_head = statistics->files_tail = node;
        }
        else
        {
            statistics->files_tail = statistics->files_tail->next = node;
        }

        ++(statistics->file_count);
    return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_FAILURE;        
    }
}
int statistics_analyze_file(statistics_t* statistics, FILE* file)
    {
        assert(statistics);
        assert(file);
        int error = EXIT_SUCCESS;

        double value = 0.0;
        if(statistics->is_binary)
        {
            while(fread(&value, sizeof(double), 1, file) == 1)
            {
                error = array_double_append(&statistics->values ,value);
                if(error)
                {
                    break;
                }
                array_double_append(&statistics->values, value);
                //printf("value = %lf\n", value);
            }
        }
        else
        {
            //FILE* output = fopen("output001.bin", "wb");
            while(fscanf(file, "%lg", &value) == 1)
            {
                error = array_double_append(&statistics->values ,value);
                if(error)
                {
                    break;
                }
                
                array_double_append(&statistics->values, value);
                //printf("value = %31.12lg\n", value);
                //fwrite(&value, sizeof(value), 1, output);
            }
            //fclose(output);
        }

        return error;
    }

    void statistics_print(const statistics_t* statistics)
    {
        assert(statistics);
    
        for(size_t index = 0; index < statistics->values.count; ++index)
        {
            const double value = statistics->values.elements[index];
            printf("value = %31.12lg\n", index, value);
        }
    }