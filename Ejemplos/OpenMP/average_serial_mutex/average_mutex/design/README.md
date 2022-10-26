# Average_Mutex

## Codigo del tema 22 de OPENMP

Haga un programa que lea un conjunto arbitrario de números flotantes de la entrada estándar, calcule concurrentemente el promedio y lo imprima en la salida estándar. La cantidad de números no se conoce de antemano. Para evitar la condición de carrera, use control de concurrencia (exclusión mutua). Sugerencia: cree primero una versión serial. Mida el rendimiento de una ejecución serial y la concurrente. El siguiente ejemplo de ejecución calcula el promedio de los primeros 10 millones de números.

seq 1 10000000 | perf stat bin/average_mutex

**Esta segunda version ya utiliza concurrencua pero habia una condicion de carrera por lo que hay que utilizar un mutex para controlarla, no obstante, al usar mutex, se esta utilizando una herramienta que consume tiempo y no hace al programa mas rapido que la serial, al contrario, mas bien lo hace mas lento por lo que mas adelante se veran mas mecanismos para poder incrementar el desempeño**