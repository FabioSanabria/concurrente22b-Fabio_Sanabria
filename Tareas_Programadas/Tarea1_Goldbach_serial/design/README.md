[comment]: <> (Goldbach_serial diagramas v1.1 Fabio Andres Sanabria Valerin <fabio.sanabria@ucr.ac.cr>)

# Diagramas del programa

## Diagrama de flujo

En el diagrama mostrado a continuación se muestran las distintas decisiones que debe tomar el programa respecto a los números que ingrese el usuario.  
Se presenta en el diagrama *un número compuesto de al menos dos potencias, un número primo y un número inválido*, esto para ejemplificar con mayor claridad los resultados de las decisiones tomadas de acuerdo al número que reciba el programa:

![Marcado 1](/design/Diagrama_flujo.png)

## Diagrama UML secuencial

En el siguiente diagrama se pueden observar los procesos secuenciales ocurridos dentro del programa de manera general, con el fin de brindar mayor claridad a la hora de plantear las etapas necesarias para brindar el resultado deseado al finalizar el programa.  
Se observa al *actor* , quien se encarga de insertar los datos a ser procesados. Luego se muestran los tres procesos principales por los que pasan los datos:  

* Entrada de datos o input
* Realización de las *sumas de Goldbach*
* Salida de datos u output

En caso de que el número digitado no sea un entero el programa se detendrá. En caso contrario se continuará el proceso hasta mostrar la salida de los datos al usuario.

![Marcado 1](/design/Diagrama_secuencial_UML.png)
