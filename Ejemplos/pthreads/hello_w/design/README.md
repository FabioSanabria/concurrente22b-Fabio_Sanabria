# Hello_W

## Código del tema 4

Modifique el programa de ***"hola mundo"*** para que cada hilo de ejecución salude indicando su número y la cantidad de hilos en el equipo al que pertenece. Permita al usuario indicar la cantidad de hilos como argumento de la línea de comandos. Si el usuario omite este argumento, suponga la cantidad de CPUs disponibles en el sistema. La siguiente podría ser una salida hipotética.

`Hello from secondary thread 1 of 4`
`Hello from secondary thread 2 of 4`
`Hello from secondary thread 0 of 4`
`Hello from secondary thread 3 of 4`

Indague en la ***especificación*** cómo obtener el número de CPUs disponibles con OpenMP o bien, la cantidad sugerida de hilos a crear en una región paralela.

### Ejercicio 9

Utilizando los argumentos en línea de comandos, ubique la cantidad máxima de threads que su sistema operativo le permite crear. Escriba el número obtenido en un archivo readme.ext en su carpeta `ejemplos/pthreads/hello_iw_pri`, junto con el nombre de su sistema operativo ***(por ejemplo, Debian 10 64bits o Fedora 30 64bits)***.

**Cantidad de hilos que puede fabricar mi maquina virtual:**

`[UBUNTU 63048 64bits]`