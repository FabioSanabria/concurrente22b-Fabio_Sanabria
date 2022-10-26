# Average_Atomic

## Codigo del tema 22 de OPENMP

Haga un programa que lea un conjunto arbitrario de números flotantes de la entrada estándar, calcule concurrentemente el promedio y lo imprima en la salida estándar. La cantidad de números no se conoce de antemano. Para evitar la condición de carrera, use control de concurrencia (exclusión mutua). Sugerencia: cree primero una versión serial. Mida el rendimiento de una ejecución serial y la concurrente. El siguiente ejemplo de ejecución calcula el promedio de los primeros 10 millones de números.

seq 1 10000000 | perf stat bin/average_mutex

**Esta cuarta version ya utiliza concurrencia pero habia una condicion de carrera por lo que hay que utilizar un mutex para controlarla, la diferencia entre esta implemetacion y la del mutex es que esta se coloca el mecanismo de control de concurrencia en un lugar donde los hilos no entran tantas veces haciendo el codigo mas rapido en comparacion a versiones anteriores**
