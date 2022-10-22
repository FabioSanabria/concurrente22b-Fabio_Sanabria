[comment]: <> (Goldbach_pthread readme v1.4 Fabio Sanabria Valerin <fabio.sanabria@ucr.ac.cr>)

# Diagramas del programa

## Diagrama de flujo pthread

En el diagrama mostrado a continuación se muestran las distintas decisiones que debe tomar el programa respecto a los números que ingrese el usuario y los hilos que se creen.  
Se presenta en el diagrama *un número compuesto de al menos dos potencias, un número primo y un número inválido*, esto para ejemplificar con mayor claridad los resultados de las decisiones tomadas de acuerdo al número que reciba el programa:

![Marcado 1](../images/Diagrama_flujo_goldbach.png)

## Diagrama UML secuencial pthread

En el siguiente diagrama se pueden observar los procesos secuenciales ocurridos dentro del programa de manera general, con el fin de brindar mayor claridad a la hora de plantear las etapas necesarias para brindar el resultado deseado al finalizar el programa.  
Se observa al *actor* , quien se encarga de insertar los datos a ser procesados. Luego se muestran los cuatro procesos principales por los que pasan los datos:  

* Entrada de datos o input
* Creacion y division de los hilos
* Realización de las *sumas de Goldbach*
* Salida de datos u output

En caso de que el número digitado no sea un entero el programa se detendrá. En caso contrario se continuará el proceso hasta mostrar la salida de los datos al usuario.

Si el usuario no provee el número de hilos, el programa usará por default la cantidad de CPU's que posea el equipo.  


![Marcado 1](../images/Secuencial_UML_goldbach.png)

## Diagrama de clases

El diagrama es el mismo que el de la tarea pasada ya que sigue la misma estructura, no obstante, ahora la tarea esta bien modularizada haciendo que goldbach.c no tenga tanta responsabilidad, en el diagrama aparece que goldbach hace muchas tareas y en parte tiene razon ya que goldbach.c es el que llama a todas las demas clases que hacen los calculos de las sumas y las impresiones de los numeros, no obstante, ya no lo hace de manera directa. Goldbach.c lo que hace de manera directa es la creaciond de los hilos y la division de los trabajos utilizando mapeo dinamico para aumentar la velocidad


![Marcado 1](../images/Diagrama_de_clases.jpg)

## Pseudo Código

[goldbach_optimization pseudo codigo](https://github.com/FabioSanabria/concurrente22b-Fabio_Sanabria/blob/main/Tareas_Programadas/Tarea3_Optimizacion/Tarea3_Goldbach_optimization/design/goldbach_dinamic.pseudo)


## Reporte

[Documento de reporte]( https://github.com/FabioSanabria/concurrente22b-Fabio_Sanabria/tree/main/Tareas_Programadas/Tarea3_Optimizacion/Tarea3_Goldbach_optimization/report)

## perf_measure_tarea03
[Excel con los tiempos]( https://github.com/FabioSanabria/concurrente22b-Fabio_Sanabria/blob/main/Tareas_Programadas/Tarea3_Optimizacion/Tarea3_Goldbach_optimization/design/perf_measure_tarea03.xlsx)
