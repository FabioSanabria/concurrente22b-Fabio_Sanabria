// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>

#include "calculadora.h"

bool esPrimo(int64_t element) {
  bool resultado = false;
  if (element >= 2) {
    resultado = true;
    double limite = sqrt((double)(element));
    for (int i = 2; i <= limite; i++) {
      if (element % i == 0) {
        resultado = false;
      }
    }
  }
  return resultado;
}

int calcular_primos_calculadora(array_primos_t* array_primos,
array_booleans_t* array_booleans, int64_t num) {
  int cont2 = 0;
  array_booleans_append(array_booleans, 0);  // 0
  array_booleans_append(array_booleans, 0);  // 1

  for (int64_t i = 2; i < num; i++) {
    array_booleans_append(array_booleans, 1);
  }
  // 0 significa que es primo, 1 significa que no es primo 
	//Recorrer los números y para cada uno
	for (int64_t i = 2; i < num; i++){
  //Si es primo recorrer los múltiplos y marcarlos como no primo
		if (array_booleans->boolean[i] == 1) {
			for (int64_t j = i * i; j < num; j += i){
				array_booleans->boolean[j] = 0;
			}
		}
	}
    for (int64_t i = 2; i < num; i++) {
      if (array_booleans->boolean[i] == 1) {
        array_primos_append(array_primos, i);
        cont2++; 
      }
  }
  return cont2;
}

void calcular_pares_calculadora
  (goldbach_t *elements, int64_t num, int prime_count) {
   bool array_sum_required = false;
   if (elements->value < 0) {
     array_sum_required = true;
  }

  int64_t possible_prime = 0;
  for (int i = 0; i < prime_count; i++) {
    possible_prime = num - elements->array_primos.primo[i];
    if (esPrimo(possible_prime)) {
      if (possible_prime >= elements->array_primos.primo[i]) {
        elements->cant_sum++;
        if (array_sum_required) {
          sumas_value_append(&elements->array_sum,
          elements->array_primos.primo[i],
          possible_prime, 0);
        }
      }
    }
  }
}

void calcular_impares_calculadora(goldbach_t *elements, int64_t
num, int prime_count) {
  bool array_sum_required = false;
  if (elements->value < 0) {
    array_sum_required = true;
  }

  int64_t possible_prime = 0;

  for (int i = 0; i < prime_count; i++) {
    for (int j = i; j < prime_count; j++) {
    possible_prime = num - elements->array_primos.primo[i] -
    elements->array_primos.primo[j];
    if (esPrimo(possible_prime)) {
      if (possible_prime >= elements->array_primos.primo[i] && 
          possible_prime >= elements->array_primos.primo[j]) {
        elements->cant_sum++;
        if (array_sum_required) {
          sumas_value_append(&elements->array_sum,
          elements->array_primos.primo[i],
          elements->array_primos.primo[j],
          possible_prime);
        }
      }
    }
    }
  }  
}

void calcular_sumas_calculadora(goldbach_t* elements) {
  int64_t num = labs(elements->value);

  int prime_count = calcular_primos_calculadora
    (&elements->array_primos,
     &elements->array_booleans, num);
  if (num > 5) {
    if (elements->value % 2 == 0) {
      calcular_pares_calculadora(elements, num, prime_count);
    } else {
      calcular_impares_calculadora(elements, num, prime_count);
    }
  }
}