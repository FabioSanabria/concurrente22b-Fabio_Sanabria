### Shared memory

## Codigo del tema 5

Cree una nueva versión de su programa "hola mundo numerado" donde cada hilo de ejecución imprime `"Hello from secondary thread i of w\n"`, donde `i` es el número de hilo y `w` la cantidad total de hilos que componen el equipo, pero con los siguientes cambios:

1) Haga que los datos comunes para todos los hilos de ejecución (el número `w`) estén en un registro (estructura) compartido para cada hilo. 
2) Mantenga los datos exclusivos para cada hilo (`i`) se mantengan en el registro privado.

Reporte la duración en segundos que los hilos toman en saludar. Puede usar la función POSIX `clock_gettime()` de <time.h>.

***De momento no se ha reportado la duracion de los segundos en el codigo ya que en el tema 5 no se vio***

