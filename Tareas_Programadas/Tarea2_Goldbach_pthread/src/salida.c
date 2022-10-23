// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>

#include "salida.h"

void print_cant_sumas_numeros(const array_int64_t* array,
const uint64_t array_size) {
  // Imprime la cantidad de numeros y sumas que hay en el
  // array
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

void goldbach_print_salida(const array_int64_t* array,
const uint64_t array_size) {
  // Se encarga de imprimir todo lo relacionado con las sumas y su cantidad
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
  // Este metodo es cuando uno de los numeros es par
  // por lo que debe de agarrar 2 de los elementos en
  // en array de las sumas
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
  // Este metodo es cuando uno de los numeros es impar
  // por lo que debe de agarrar 3 de los elementos en
  // en array de las sumas
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
