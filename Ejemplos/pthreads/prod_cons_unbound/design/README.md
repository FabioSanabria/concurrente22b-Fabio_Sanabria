### Ejemplo de prod_cons_unbound

## Codigo del tema 10, 11 y 12

Modifique la simulación del problema **productor-consumidor** generalizando las siguientes restricciones.

1. El buffer no es acotado, sino de tamaño arbitrario o dinámico, definido por los límites de memoria del sistema.

2. No existe el concepto de ronda. El primer argumento de línea de comandos indica la cantidad de productos a simular. Identifique los productos en orden secuencial.

3. Varios productores, indicados por el usuario en el segundo argumento de línea de comandos.

4. Varios consumidores, indicados por el usuario en el tercer argumento de línea de comandos.

5. Cada productor y consumidor indica su número en la salida estándar.

El siguiente ejemplo de ejecución simula la construcción de 5 unidades de trabajo por 2 productores que tardan entre 0 y 100 milisegundos en producir, y 3 consumidores que tardan entre 0 y 750 milisegundos en consumir.

`$ ./producer_consumer 5 2 3 0 100 0 750`
`0 produced 1`
`1 produced 3`
    `1 consuming 1`
`0 produced 2`
    `2 consuming 3`
`0 produced 5`
    `0 consuming 5`
    `1 consuming 2`
`1 produced 4`
    `2 consuming 4`

**Este programa crea varios productores y consumidores que trabajan paralelamente unos de otros por lo que se necesita de un trabajo**
**cuidadoso a la hora del control de concurrencia**
