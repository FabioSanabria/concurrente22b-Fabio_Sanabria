// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0

// All the includes was seen before
#include <assert.h>
#include <limits.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "array_int64.h"

/** @struct shared_data_t
 *  @brief Este struct tiene la funcionalidad
 *  de guardar los datos que se requieran compartir
 * entre los hilos como lo es la cantidad de hilos
 *  @b array guarda el arreglo que contiene el los
 * tiempos ingresados por el usuario en la entrada
 * estandar
 *  @b thread_count cantidad de hilos que puso el usuario
 */
typedef struct shared_block {
  array_int64_t* array;
  uint64_t thread_count;
} shared_data_block;

/** @struct private_block
 *  @brief Este struct tiene la funcionalidad
 *  de guardar los datos privados de los hilos, como
 * lo es el inicio y final del mapeo de cada hilo, su 
 * shared data y el numero de hilo
 *  @b thread_number Numero de hilo que tiene alguno en
 * especifico
 *  @b shared_Data registro compartido de todos los hilos
 * 
 *  @b start Indica el inicio de los elementos con los que
 * tiene que trabajar el hilo (mapeo). Ej [0, 3[
 * 
 * @b finish Indica el final de los elementos con los que 
 * tiene que trabajar un hilo (mapeo). Ej [0, 3[
 */
typedef struct private_block {
  uint64_t thread_number;
  shared_data_block* shared_data;
  size_t start;
  size_t finish;
} private_data_block;

void create_threads_block(shared_data_block *shared_data, size_t task_amount);
// void create_threads_dinamic(shared_data_dinamic *shared_data);

/** 
 *  @brief Tiene como funcion obtener la velocidad maxima que
 * logro alcanzar el mapeo que se analiza
 *  @b time_map tiempo maximo que dura un mapeo en especifico
 *  @b serial_time Tiempo que dura la serial para ejecutar
 *  @return La velocidad maxima que logro el mapeo
 * una tarea
 */
double getSpeedUp(size_t time_map, size_t serial_time);

/** 
 *  @brief Tiene como principal funcion dar la eficiencia de
 * los mapeos
 *  @b time_map tiempo maximo que dura un mapeo en especifico
 *  @b serial_time Tiempo que dura la serial para ejecutar
 * una tarea
 *  @return la eficiencia maxima que logro obtener el algoritmo
 * probablemente, el maximo es 1
 */
double getEfficiency(size_t time_map, size_t serial_time,
size_t thread_count);

/**
 * @brief Subrutina encargada de asignarle a cada thread
 * una tarea en especifico, en este caso se va a usar para
 * el caso de mapeo por bloque
 * @param data Datos privados que tiene cada hilo
 * (Dentro del metodo se hace parse)
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
void* asignar_thread(void* data);

/** 
 *  @brief Formula utilizada para lograr distribuir de manera
 * correcta los hilos para lograr un mapeo de bloque
 *  @b i Numero de hilo
 *  @b d Cantidad de tareas a realizar
 *  @b w Cantidad de hilos
 */
size_t formula_bloque(int i, int D, int w);

/** 
 *  @brief Saca el minimo entre 2 numeros, es utilizado por
 * el mapeo de bloque
 *  @b a Numero1 a comparar
 *  @b b Numero2 a comparar
 */
uint64_t minimo(uint64_t a, uint64_t b);

double getSpeedUp(size_t time_map, size_t serial_time) {
  return (double)serial_time / time_map;
}

double getEfficiency(size_t time_map, size_t serial_time,
size_t thread_count) {
  return (double)serial_time / (time_map * thread_count);
}
int main(int argc, char* argv[]) {
  array_int64_t arreglo;
  array_int64_t arregloDinamico;
  array_int64_t arregloCiclico;
  array_int64_init(&arreglo);
  array_int64_init(&arregloDinamico);
  array_int64_init(&arregloCiclico);
  int error = EXIT_SUCCESS;
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {  // Scan de number
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }
  int64_t value = 0;  // data entered by the user
  // If the data isn't a number,
  while (fscanf(stdin, "%"SCNd64, &value) == 1) {
    // add all the data to an array of values
    error = array_int64_append(&arreglo, value);
  }

  size_t serial_time = 0;
  for (size_t i = 0; i < arreglo.count; i++) {
    serial_time += arreglo.elements[i];
  }

  for (size_t i = 0; i < thread_count; i++) {
    array_int64_append(&arregloDinamico, arreglo.elements[i]);
  }

// PARTE DEL MAPEO DINAMICO-----------------------------
  size_t index_temp = 0;
  int64_t time_temp = INT64_MAX;
  // arreglo = tiempos
  // arregloDinamico = Simula los hilos
  for (uint64_t i = thread_count; i < arreglo.count; i++) {  //
    time_temp = INT64_MAX;
    for (uint64_t z = 0; z < arregloDinamico.count; z++) {
      if (arregloDinamico.elements[z] <= time_temp) {
        index_temp = z;
        time_temp = arregloDinamico.elements[z];
        // printf("Hilos tiempos %"PRId64 "\n", time_temp);
      }
    }
    arregloDinamico.elements[index_temp] += arreglo.elements[i];
  }

  int64_t max_time = 0;
  for (uint64_t i = 0; i < arregloDinamico.count; i++) {
    if (max_time < arregloDinamico.elements[i]) {
      max_time = arregloDinamico.elements[i];
    }
  }
// FIN DE MAPEO DINAMICO---------------------------------

// PARTE DEL MEPEO CICLICO
  for (uint64_t i = 0; i < thread_count; ++i) {
    // Guarda hilos(indices)
    array_int64_append(&arregloCiclico, 0);
  }

  size_t indice = 0;
  for (uint64_t i = 0; i < arreglo.count; ++i) {
    if (thread_count < indice + 1) {
      indice = 0;
      i--;
    } else {
      arregloCiclico.elements[indice] += arreglo.elements[i];
      ++indice;
    }
  }

  int64_t max_time_Ciclic = 0;
  for (uint64_t i = 0; i < arregloCiclico.count; i++) {
    if (max_time_Ciclic < arregloCiclico.elements[i]) {
      max_time_Ciclic = arregloCiclico.elements[i];
    }
  }

printf("--------------------------------------------------");
printf("------------------------------------\n");
// FIN DEL MAPEO CICLICO
  printf("%zu \t", serial_time);
  for (size_t i = 0; i < thread_count; i++) {
    printf("%zu \t", i);
  }

  printf("Duration \t");
  printf("Speedup \t\t");
  printf("Efficiency\n");

  printf("Block   ");
// PARTE DEL MAPEO ESTATICO POR BLOQUE
  shared_data_block *shared_data = (shared_data_block *)
  calloc(1, sizeof(shared_data_block));
    shared_data->thread_count = thread_count;
    shared_data->array = &arreglo;

  create_threads_block(shared_data, arreglo.count);
  // FIN DEL MAPEO ESTATICO POR BLOQUE
  double speed = getSpeedUp(arreglo.max_time, serial_time);
  double efficience = getEfficiency(arreglo.max_time, serial_time,
  (size_t)thread_count);

  // for (size_t i = 0; i < thread_count; ++i) {
  //   printf("Hilo %zu %"PRId64 "\n",i,arreglo.timesBloque[i]);
  // }
  printf("%zu \t\t", arreglo.max_time);
  printf("%f\t\t", speed);
  printf("%f\t\t\n", efficience);

  arreglo.max_time = 0;

  speed = getSpeedUp(max_time_Ciclic, serial_time);
  efficience = getEfficiency(max_time_Ciclic, serial_time,
  (size_t)thread_count);

  printf("Cyclic  ");
  for (uint64_t i = 0; i < arregloCiclico.count; i++) {
    printf("%"PRId64 "\t", arregloCiclico.elements[i]);
  }

  printf("%zu \t\t", max_time_Ciclic);
  printf("%f\t\t", speed);
  printf("%f\t\t\n", efficience);

  speed = getSpeedUp(max_time, serial_time);
  efficience = getEfficiency(max_time, serial_time,
  (size_t)thread_count);

  printf("Dynamic ");
  for (uint64_t i = 0; i < arregloDinamico.count; i++) {
    printf("%"PRId64 "\t", arregloDinamico.elements[i]);
  }

  printf("%zu \t\t", max_time);
  printf("%f\t\t", speed);
  printf("%f\t\t\n", efficience);

  array_int64_destroy(&arregloCiclico);  // se llama al destructor
  array_int64_destroy(&arregloDinamico);  // se llama al destructor
  array_int64_destroy(&arreglo);  // se llama al destructor

  return error;
}

void create_threads_block(shared_data_block *shared_data,
uint64_t task_amount) {
  pthread_t *threads = (pthread_t *)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_block *private_data = (private_data_block *)
    calloc(shared_data->thread_count, sizeof(private_data_block));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
    ++thread_number) {
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;
      private_data[thread_number].start = formula_bloque(thread_number,
      task_amount, shared_data->thread_count);
      private_data[thread_number].finish = formula_bloque(thread_number+1,
      task_amount, shared_data->thread_count);
      pthread_create(&threads[thread_number], /*attr*/ NULL,
      asignar_thread, /*arg*/ &private_data[thread_number]);
    }

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
    ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    free(private_data);
    free(threads);
  }
}

void* asignar_thread(void* data) {
  private_data_block* private_data = (private_data_block*) data;
  shared_data_block* shared_data = private_data->shared_data;
  size_t time = 0;
  for ( size_t i = private_data->start; i < private_data->finish; i++ ) {
    time += shared_data->array->elements[i];
    if (shared_data->array->max_time < time) {
      shared_data->array->max_time = time;
    }
  }
  printf("%zu \t", time);
  return NULL;
}

uint64_t minimo(uint64_t a, uint64_t b) {
  return ( ( ( a) < ( b)) ? ( a) : ( b));
}

size_t formula_bloque(int i, int D, int w) {
  // i = thread_number, D = cant_sums, w = thread_count
  return i*(int)floor(D/w)+minimo(i, D%w);
}
