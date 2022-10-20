// Goldbach_optimization program v1.7 Fabio Sanabria
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "array_int64.h"
#include "array_num64.h"
#include "goldbach.h"


// thread_share_data_t
typedef struct shared {
  array_int64_t* array;
  uint64_t thread_count;
  int64_t pos;  // posicion del hilo en el arreglo
  pthread_mutex_t can_acces;  // verifica si el hilo puede accesar
} shared_data_t;

// thread_private_data_t
typedef struct private {
  uint64_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Subrutina que calcula los numeros primos
 * para cada numero ingresado
 * @param goldbach puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @return int cantidad de primos
*/
int calcular_primos(array_primos_t* array_primos, int64_t num);

/**
 * @brief Lee los datos ingresados en entrada estandar
 * @param goldbach puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @param file un puntero a un archivo de texto
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int goldbach_recibir_datos(array_int64_t *array, FILE *file, int argc,
char* argv[]);

/**
 * @brief Subrutina que calcula las sumas de goldbach
 * @param goldbach puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @return void
*/
void calcular_sumas(goldbach_t* elements);

/**
 * @brief Subrutina que calcula las sumas de goldbach pares
 * @param elements puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @param num numero par a calcular
 * @param prime_count tamanio del arreglo de numeros primos
 * @return void
*/
void calcular_pares 
(goldbach_t* elements, int64_t num, int prime_count);

/**
 * @brief Subrutina que calcula las sumas de goldbach impares
 * @param elements puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @param num numero impar a calcular
 * @param prime_count tamanio del arreglo de numeros primos
 * @return void
*/
void calcular_impares(goldbach_t* elements, int64_t
num, int prime_count);


int create_threads(shared_data_t *shared_data);
void* asignar_thread(void *data);
size_t formula_bloque(int i, int D, int w);

/** @brief Subrutina que imprime las sumas de goldbach
 * ya sea que soliciten las sumas o solo la cantidad de sumas
 * @param goldbach puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @return void
*/
void goldbach_print(const array_int64_t* array, const uint64_t array_size);
void print_par(const array_int64_t* array, int i);
void print_impar(const array_int64_t* array, int i);
void print_cant_sumas_numeros(const array_int64_t* array,
const uint64_t array_size);

int calcular_primos(array_primos_t* array_primos, int64_t num) {
	int i,j,n;
	int numeros[num];
  int cant_primes = 0;  
	//Obtener las lista de números a evaluar
	
  for(i=2;i<num;i++){
		numeros[i]=1;
	}
	
	//Hacer 2 el primer número primo
	numeros[2]=1;
 
	//Recorrer los números y para cada uno
	for (n=2;n<num;n++){
//Si es primo recorrer los múltiplos y marcarlos como no primo
		if (numeros[n]){
      array_primos_append(array_primos, num);
      cant_primes++;
			for (i=n*n;i<num;i+=n){
				numeros[i] = 0;
			}
		}
	}
	  
	//Muestro la lista de los primos	
	printf("Primos: ");
	for (n = 2; n < num; n++){
		if (numeros[n]){
			printf("%d  ",n);
		}
	}
    
	return cant_primes;
}


void calcular_sumas(goldbach_t* elements) {
  int64_t num = labs(elements->value);

  int prime_count = calcular_primos
    (&elements->array_primos, num);
  if (num > 5) {
    if (elements->value % 2 == 0) {
      calcular_pares(elements, num, prime_count);
    } else {
      calcular_impares(elements, num, prime_count);
    }
  }
}

void calcular_pares
  (goldbach_t *elements, int64_t num, int prime_count) {
  bool array_sum_required = false;
  if (elements->value < 0) {
    array_sum_required = true;
  }
  for (int i = 0; i < prime_count; i++) {
    for (int j = i; j < prime_count; j++) {
      if (elements->array_primos.primo[i] +
      elements->array_primos.primo[j] == num) {
        elements->cant_sum++;
        if (array_sum_required) {
          sumas_value_append(&elements->array_sum,
          elements->array_primos.primo[i],
          elements->array_primos.primo[j], 0);
        }
      }
    }
  }
}

void calcular_impares(goldbach_t *elements, int64_t
num, int prime_count) {
  bool array_sum_required = false;
  if (elements->value < 0) {
    array_sum_required = true;
  }
  for (int i = 0; i < prime_count; i++) {
    for (int j = i; j < prime_count; j++) {
      for (int k = j; k < prime_count; k++) {
        if (elements->array_primos.primo[i] +
        elements->array_primos.primo[j] +
        elements->array_primos.primo[k] == num) {
          elements->cant_sum++;
          if (array_sum_required) {
            sumas_value_append(&elements->array_sum,
            elements->array_primos.primo[i],
            elements->array_primos.primo[j],
            elements->array_primos.primo[k]);
          }
        }
      }
    }
  }
}

void goldbach_run(array_int64_t* goldbach, size_t thread_count, int argc,
 char* argv[]) {
  size_t task_amount = goldbach_recibir_datos(goldbach, stdin, argc, argv);
  double time_spent = 0;
  clock_t begin = clock();  // para medir tiempo
  shared_data_t *shared_data = (shared_data_t *)
  calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->thread_count = thread_count;
    shared_data->array = goldbach;
    shared_data->pos = 0;  // se inicializa la posicion

    pthread_mutex_init(&shared_data->can_acces, NULL);
    create_threads(shared_data);

    pthread_mutex_destroy(&shared_data->can_acces);
    free(shared_data);
  }
  goldbach_print(goldbach, task_amount);
  clock_t end = clock();  // fin de la medicion
  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Tiempo: %f segundos\n", time_spent);

}

int create_threads(shared_data_t *shared_data) {
  int error = EXIT_SUCCESS;
  pthread_t *threads = (pthread_t *)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t *private_data = (private_data_t *)
    calloc(shared_data->thread_count, sizeof(private_data_t));

  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < 
    shared_data->thread_count; ++thread_number) {

      private_data[thread_number].shared_data = shared_data;
      error = pthread_create(&threads[thread_number], /*attr*/ NULL,
      asignar_thread, /*arg*/ &private_data[thread_number]);
    }

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
    ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    free(private_data);
    free(threads);
  }
  return error;
}

int goldbach_recibir_datos(array_int64_t *array, FILE *file, int argc,
char* argv[]) {
  assert(array);
  assert(file);
  if (argc < 0 && argv[1]) {
    printf("no hay argumentos");
  }
  printf("Escriba los numeros a procesar\n");
  int64_t value = 0;
  int contador = 0;
  while (fscanf(file, "%" PRId64, &value) == 1) {
    ++contador;
    array_int64_append(array, value);
  }
  return contador;
}

void* asignar_thread(void* data) {
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  int64_t pos = 0;

  while (true) {
    pthread_mutex_lock(&shared_data->can_acces);
    pos = shared_data->pos;
    shared_data->pos++;

    if ((size_t)shared_data->pos > shared_data->array->count) {
      pthread_mutex_unlock(&shared_data->can_acces);
      break;
    }
    pthread_mutex_unlock(&shared_data->can_acces);
    calcular_sumas(&shared_data->array->elements[pos]);
  }
  return NULL;
}

void print_cant_sumas_numeros(const array_int64_t* array,
const uint64_t array_size) {
  int64_t cant_sumas = 0;
  int64_t cant_numeros = (int64_t)array_size;
  for (uint64_t i = 0; i < array_size; i++) {
    if (array->elements[i].value == 4 || array->elements[i].value == -4) {
      ++cant_sumas;
    } else {
      cant_sumas += array->elements[i].cant_sum;
    }
  }
  printf("Total " "%"PRId64 " numbers " "%"PRId64 " sums\n\n",
  cant_numeros, cant_sumas);
}

void goldbach_print(const array_int64_t* array, const uint64_t array_size) {
  print_cant_sumas_numeros(array, array_size);
  for (uint64_t i = 0; i < array_size; i++) {
    int64_t num = labs(array->elements[i].value);

    printf("%" PRId64 ": ", array->elements[i].value);
    if (0 <= num && num <= 5) {
      if (array->elements[i].value == 4) {
        printf("1 sums");
      } else {
        if (array->elements[i].value == -4) {
          printf("1 sums: 2 + 2");
        } else {
          printf("NA");
        }
      }
    } else {
      if (array->elements[i].value < 0) {
        printf("%" PRId64 " sums: ", array->elements[i].cant_sum);
        if (array->elements[i].value % 2 == 0) {
          print_par(array, i);
        } else {
          print_impar(array, i);
        }
      } else {
        printf("%" PRId64 " sums ", array->elements[i].cant_sum);
      }
    }
    printf("\n");
  }
}

void print_par(const array_int64_t* array, int i) {
  for (int j = 0; j < array->elements[i].cant_sum; j++) {
    if ( j ==  array->elements[i].cant_sum - 1 ) {
      printf("%" PRId64 " + %" PRId64, array->elements[i].array_sum.
      sumas_value[j].num1, array->elements[i].array_sum.sumas_value
      [j].num2);
    } else {
      printf("%" PRId64 " + %" PRId64 ", ", array->elements[i].
      array_sum.sumas_value[j].num1, array->elements[i].array_sum.
      sumas_value[j].num2);
    }
  }
}

void print_impar(const array_int64_t* array, int i) {
  for (int j = 0; j < array->elements[i].cant_sum; j++) {
    if ( j == array->elements[i].cant_sum - 1 ) {
      printf("%" PRId64 " + %" PRId64 " + %" PRId64, array->elements[i]
      .array_sum.sumas_value[j].num1, array->elements[i].array_sum.
      sumas_value[j].num2, array->elements[i].array_sum.sumas_value
      [j].num3);
    } else {
      printf("%" PRId64 " + %" PRId64 " + %" PRId64 ", ",
      array->elements[i].array_sum.sumas_value[j].num1,
      array->elements[i].array_sum.sumas_value[j].num2,
      array->elements[i].array_sum.sumas_value[j].num3);
    }
  }
}