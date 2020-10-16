; Inicialización de la pantalla utilizando el segmento de video guardado en GS

BITS 32

init_screen:
	pushad
	; BORDE SUPERIOR NEGRO
	; Inicio: 	(0, 0)
	; Fin: 		(80, 0)
	mov ecx, dword 80
	mov edx, dword 0				; Offset
	.bordeSuperior:
	 	mov word [gs:edx], 0x0000	; |Negro|Null|
	 	add edx, 2

	 	loop .bordeSuperior

	; TABLERO VERDE
	; Inicio: 	(0, 1)
	; Fin:		(80, 40)
	mov ecx, dword 3200	; 80 * 40
	.tablero:
	 	mov word [gs:edx], 0x2000	; |Verde|Null|
	 	add edx, 2

	 	loop .tablero

	; RECTANGULO INFERIOR NEGRO
	; Inicio:	(0, 41)
	; Fin:		(80, 50)
	mov ecx, dword 720 ; 80 * 9	
	.rectanguloInferior:
	 	mov word [gs:edx], 0x0000	; |Negro|Null|
	 	add edx, 2

	 	loop .rectanguloInferior

	; RECUADRO AZUL DE PUNTAJE
	; Inicio:	(3, 44)
	; Fin: 		(11, 47)
	mov ebx, dword 44 		; y = 44
	.puntajeAzulFilas:

		mov eax, ebx		; eax = y
		mov edx, dword 80
    	imul edx          	; eax = y * 80
    	mov edx, dword 3	; edx = x
    	add eax, edx   		; eax = y * 80 + x

    	shl eax, 1
    	mov edx, eax

		mov ecx, dword 9 	; Largo de recuadro
		.puntajeAzulColumnas:
	 		mov word [gs:edx], 0x1000	; |Azul|Null|
	 		add edx, 2
		loop .puntajeAzulColumnas

		inc ebx

		cmp ebx, 47
		jl .puntajeAzulFilas

	; RECUADRO ROJO DE PUNTAJE
	; Inicio:	(68, 44)
	; Fin: 		(77, 47)
	mov ebx, dword 44	; y = 44 
	.puntajeRojoFilas:

		mov eax, ebx		; eax = y
		mov edx, dword 80
    	imul edx          	; eax = y * 80
    	mov edx, dword 68	; edx = x
    	add eax, edx   		; eax = y * 80 + x

    	shl eax, 1
    	mov edx, eax

		mov ecx, dword 9 	; Largo de recuadro
		.puntajeRojoColumnas:
	 		mov word [gs:edx], 0x4000	; |Rojo|Null|
	 		add edx, 2
		loop .puntajeRojoColumnas

		inc ebx

		cmp ebx, 47
		jl .puntajeRojoFilas
		
	popad
	ret

