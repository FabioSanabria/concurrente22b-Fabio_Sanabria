# Ejercicio 48 [odd_even_sort_serial]

## Codigo del ejercicio 48

Modifique los códigos de los hilos para que la instrucción a1 se ejecute siempre antes que la instrucción b1 y ésta siempre se ejecute antes que la instrucción c1. Este orden de ejecución puede abreviarse como **a1 < b1 < c1**.


El siguiente código implementa el ordenamiento par-impar (odd-even transposition sort) de un arreglo a con n elementos. Rastree el algoritmo con el arreglo a = [7 4 6 8 3].


    for (int phase = 0; phase < n; ++phase) {
      if (phase % 2 == 0) {
        for (int i = 1; i < n; i += 2) {
        if (a[i - 1] > a[i]) {
          swap(&a[i - 1], &a[i]);
        }
      }
    } else {
      for (int i = 1; i < n - 1; i += 2) {
        if (a[i] > a[i + 1]) {
          swap(&a[i], &a[i + 1]);
        }
      }
    }
  }

Implemente una versión serial del ordenamiento par-impar (odd-even transposition sort) en una subrutina `serial_odd_even_sort(size_t n, double arr[n])`que ordena un arreglo de números flotantes. Escriba un programa de pruebas main() que reciba por argumentos de línea de comandos el tamaño n del arreglo, crea un arreglo de números flotantes de doble precisión en la memoria dinámica, y los inicializa con valores aleatorios. El programa de pruebas ordena el arreglo con su implementación de `serial_odd_even_sort().`

Puede imprimir el arreglo en la salida estándar para comprobar que realmente se haya ordenado, pero luego deshabilite la impresión para realizar las pruebas de rendimiento. Veririque que su programa no provoque accesos inválidos, fugas de memoria, ni condiciones de carrera.
