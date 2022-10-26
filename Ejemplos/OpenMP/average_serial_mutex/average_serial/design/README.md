# Average_Serial

## Codigo del tema 22 de OPENMP

Haga un programa que lea un conjunto arbitrario de números flotantes de la entrada estándar, calcule concurrentemente el promedio y lo imprima en la salida estándar. La cantidad de números no se conoce de antemano. Para evitar la condición de carrera, use control de concurrencia (exclusión mutua). Sugerencia: cree primero una versión serial. Mida el rendimiento de una ejecución serial y la concurrente. El siguiente ejemplo de ejecución calcula el promedio de los primeros 10 millones de números.

seq 1 10000000 | perf stat bin/average_mutex

**Esta primera version es la serial para tener una estructura previa para paralelizar, al realizar una simple operacion matematica como es la suma y la division, la version concurrente le costara bastante superar el tiempo de la serial**