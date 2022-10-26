// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  // Optiene la cantidad maxima de hilos
  // si el usuario no ingresa ningun numero
  // int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    // Agarra el segundo argumento
    // que es el de los hilos, si
    // no envia nada entonces se
    // toman la cantidad de CPUs
    // que tenga la maquina
    thread_count = atoi(argv[1]);
  }
  // Vector de valores que guarda los valores
  // para sacar el promedio de estos
  std::vector<double> values;

  // Valor que va a ser cambiado constantemente
  // ya que va a ser modificado por el cin para
  // guardarlo en el vector
  double value = 0.0;
  while (std::cin >> value) {
    values.push_back(value);
  }

  // Variable que va a almacenar el promedio de los
  // numeros ingresados por el usuario
  double sum = 0.0;

  // Aqui va sumando todos los valores en la variable para
  // luego dividirlo por la cantidad total de los valores
  // y obtener el promedio
  for (size_t index = 0; index < values.size(); ++index) {
    sum += values[index];
  }

  // En esta parte divide la variable de sumas para poder sacar
  // el promedio, no obstante, habria problemas si el denominador
  // es 0 por lo que se revisa que este no sea 0 para no dividirlo
  const double average = values.size() ? sum / values.size() : 0.0;
  // Imprime el promedio
  std::cout << average << std::endl;
}
