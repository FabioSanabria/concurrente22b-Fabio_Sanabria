# Ejeecicio 29 Signaling_3

## Pseudocodigo del ejercicio dado por el profe Alberto

Modifique los códigos de los hilos para que la instrucción a1 se ejecute siempre antes que la instrucción b1 y ésta siempre se ejecute antes que la instrucción c1. Este orden de ejecución puede abreviarse como **a1 < b1 < c1**.

`procedure main:`
  `create_thread(thread_a)`
  `create_thread(thread_b)`
  `create_thread(thread_c)`
`end procedure`

`procedure thread_a:`
  `statement a1`
`end procedure`

`procedure thread_b:`
  `statement b1`
`end procedure`

`procedure thread_c:`
  `statement c1`
`end procedure`