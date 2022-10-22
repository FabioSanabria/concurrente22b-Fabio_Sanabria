// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>

#include "entrada.h"

int goldbach_recibir_datos_entrada(array_int64_t *array, FILE *file, int argc,
char* argv[]) {
  assert(array);
  assert(file);
  if (argc < 0 && argv[1]) {
    printf("no hay argumentos");
  }
  
  int64_t value = 0;
  int contador = 0;
  while (fscanf(file, "%" PRId64, &value) == 1) {
    ++contador;
    array_int64_append(array, value);
  }
  return contador;
}
