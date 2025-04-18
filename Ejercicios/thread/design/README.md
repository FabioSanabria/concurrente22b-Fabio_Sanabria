### Thread.h

## Codigo del Ejercicio 10

Una desventaja de Pthreads es que no está disponible en sistemas operativos que no sean `POSIX`, como ocurre en `Windows`. Por eso, en la versión de 2011 de C se agregó a la biblioteca estándar el encabezado <threads.h> que permite programar hilos de ejecución agnósticos al sistema operativo. El código que use hilos de ejecución estándar será naturalmente más portable a nuevas plataformas.

Implemente la misma funcionalidad del ejemplo hello_iw_shr usando hilos estándar de `C`. Cambie el encabezado #include <pthread> por #include <threads.h>.

 **Indague en la documentación y cree hilos estándar en lugar de hilos POSIX y espere por ellos (join). Modifique la rutina concurrente de saludo para cumplir con la nueva interfaz. Asegúrese de que su solución tenga indeterminismo y no haga mal uso de memoria o concurrencia. Recuerde apegarse a la convención de estilo (linter).**
