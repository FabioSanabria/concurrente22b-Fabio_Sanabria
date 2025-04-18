# Tema 22 OpenMP parte 2

## Ejemplo de paralell_for

Modifique el programa de Ejemplo 34 para repartir iteraciones entre hilos de ejecución. El programa debe permitir al usuario indicar en el segundo argumento opcional de la línea de comandos la cantidad de iteraciones. Si el usuario omite este argumento, tome la cantidad de hilos a crear, de tal forma que cada hilo tenga al menos una iteración con que trabajar.

El programa debe imprimir en la salida estándar el número de hilo y las iteraciones que realizó. Por ejemplo, para repartir 10 iteraciones entre 3 hilos, una salida podría ser:

`$ bin/parallel_for 3 10`
`0/3: iteration 0/10`
`0/3: iteration 1/10`
`1/3: iteration 4/10`
`1/3: iteration 5/10`
`1/3: iteration 6/10`
`0/3: iteration 2/10`
`0/3: iteration 3/10`
`2/3: iteration 7/10`
`2/3: iteration 8/10`
`2/3: iteration 9/10`

**¿Qué tipo de mapeo aplicó por defecto OpenMP? Como buena práctica de programación, impida que OpenMP escoja automáticamente el tipo de memoria entre datos privados o compartidos (default(none)).**
