// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0
#include <iostream>  // Libreria para la E/S de datos

int main() {
  // Este es el mismo ejemplo que se habia creado al inicio
  // del curso, la de hello en diferentes hilos, pero hay una
  // enorme diferencia y es que ahora se usa OpenMP y disminuyo
  // una gran cantidad de lineas, originalmente, la cantidad de lineas
  // usando pthreads era de 120 para algo tan basico como esto y ahora
  // solamente se usan 3 lineas

  // El pragma realiza una inyeccion de codigo que hace que por debajo
  // cree los hilos usando pthreads y dentro de la region que engloba,
  // les va haciendo a todos join, perdemos el control de los hilos pero
  // disminuimos una enorme cantidad de lineas y trabajo  que puede ser
  // peor que el ya implementado por el OpenMP
  #pragma omp parallel
  {
    // Este omp critical simula un mutex de pthreads que lo que hace es
    // proteger la concurrencia de los hilos, aqui esto es utilizado ya que
    // hay problemas a la hora de utilizar << std::endl; ya que se realiza
    // una operecion aparte de la de cout << por lo que no significa que
    // exista condiciones de carrera, si no que hay indeterminismo por
    // parte del salto de linea y el cout
    #pragma omp critical
    // Esta es la zona protegida que lo que hace es imprimir hola
    // desde los hilos que la llamaron
    std::cout << "Hello from secondary thread" << std::endl;
  }
}
