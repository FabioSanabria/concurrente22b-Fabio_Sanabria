# Relay_race_order_effect

## Codigo del Ejercicio 32

¿Afecta el orden de creación de los equipos el resultado de la carrera de relevos? Modifique su solución de la **Ejemplo 21** para crear los equipos en orden 
inverso y compare los resultados de las ejecuciones. Sugerencia: no duplique el código, sino que utilice metaprogramación (#ifdef…​) para escoger en tiempo de compilación el orden de creación de los equipos.

**R/ La respuesta es que si afecta a la carrera, esto es confirmado por el profesor Jeisson a la hora de explicar este tema, cuando por ejemplo, el elemento 1000 es creado, este va realizando varias de las tareas que le tocan, puede que se atrase pero tiene una ventaja bastante grande y es que empezo antes que cualquier numero menor como es el 0 o el 1, puede ocurrir que 1000 llegue en las primeras posiciones y el 0 apenas va siendo creado, claro hay excepciones ya que el indeterminismo nunca se va y puede ocurrir que el elemento 1000 sea creado pero va muy lento a la hora de ejecutar sus tareas mientras que cuando el hilo 0 es creado, este va muy rapido y puede alcanzar al hilo 1000, pero son casos muy dificiles de que puedan ocurrir debido a la ventaja que obtiene el hilo al crearse de primero**