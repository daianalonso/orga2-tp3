Para ejecutar los tests:
------------------------

* Modificar el Makefile agregando la siguiente regla:

task%.tsko: task%.asm
	$(NASM) $(NASMFLAGS) -o $@ $^

* Eliminar los archivos `taskRick*.c` y `taskMorty*.c`
* Copiar el .tar.gz en la carpeta del TP.
* Ejecutar tar -xvzf caso-N.tar.gz
* Compilar y correr el test.

Descripción de los tests:

```
(Caso 1)
 Todas las tareas hacen NOP, salvo una MORTY que trata de moverse. No pasa nada.
 
(Caso 2)
 Todas las tareas hacen NOP, salvo una RICK que trata de moverse recorriendo toda la pantalla. Eventualmente gana.

(Caso 3)
 Todas las tareas hacen NOP, salvo una RICK que crea portales para conquistar el mundo. Eventualmente gana.
 
(Caso 4)
 Todas las tareas hacen NOP, salvo una RICK que crea portales para matar a todos. ¿Empate? PROBAR MODO DEBUG ACA.
 
(Caso 5)
 Rick se mueve a la derecha de Morty, y luego se mueven ambos barriendo el mapa de 2 en 2.

(Caso 6)
 Rick mapea un portal con with_morty=1. Luego mapea la pagina de morty y le escribe 1 en una dirección. Morty espera a que esa dirección esté en 1, y a partir de ahí empieza a copiar su código al portal. Rick crea portales con with_morty=1 recorriendo el mundo. Eventualmente ganan.
```
