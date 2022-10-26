# Average_Reduction

## Codigo del tema 22 de OPENMP

Haga un programa que lea un conjunto arbitrario de números flotantes de la entrada estándar, calcule concurrentemente el promedio y lo imprima en la salida estándar. La cantidad de números no se conoce de antemano. Para evitar la condición de carrera, use control de concurrencia (exclusión mutua). Sugerencia: cree primero una versión serial. Mida el rendimiento de una ejecución serial y la concurrente. El siguiente ejemplo de ejecución calcula el promedio de los primeros 10 millones de números.


seq 1 10000000 | perf stat bin/average_mutex

**En esta version lo que se utiliza es la directiva reduction que lo que hace es decirle a algun metodo de OMP que realice el proceso de sumar todos los valores en una variable para luego sacarle el promedio, eso incrementa un poco la velocidad del programa pero no lo suficiente para ganarle a la serial, ademas de que se apega al modelo de la serial**