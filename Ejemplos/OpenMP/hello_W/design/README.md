# Tema 20 openMP

## Ejemplo de hello_W usando openMP

Modifique el programa de **"hola mundo"** para que cada hilo de ejecución salude indicando su número y la cantidad de hilos en el equipo al que pertenece. Permita al usuario indicar la cantidad de hilos como argumento de la línea de comandos. Si el usuario omite este argumento, suponga la cantidad de CPUs disponibles en el sistema. La siguiente podría ser una salida hipotética.

`Hello from secondary thread 1 of 4`
`Hello from secondary thread 2 of 4`
`Hello from secondary thread 0 of 4`
`Hello from secondary thread 3 of 4`

Indague en la especificación cómo obtener el **número de CPUs** disponibles con **OpenMP** o bien, la cantidad sugerida de hilos a crear en una región paralela.
