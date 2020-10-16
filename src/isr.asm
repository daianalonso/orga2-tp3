; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "print.mac"

BITS 32

msg_int_Y db 'MODO DEBUG'
len_msg_Y equ $ - msg_int_Y

sched_task_offset:     dd 0x00
sched_task_selector:   dw 0x00

portal_creado: db 0x00

;extern imprimirExcepcion
;; PIC
extern pic_finish1

;; Sched
extern sched_nextTask
extern sched_desalojar_tarea_actual

;; Game
extern game_sumar_punto
extern game_guardar_estado
extern game_whereIsMorty
extern game_usePortalGun
extern game_end
extern print_estado_cronenbergs
extern game_finalizar
extern game_actualizar_mundo

;; Screen
extern imprimir_pantalla_debug
extern restaurar_pantalla
extern printScanCode
;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    pushad
    
    call pic_finish1

    ; Guarda el estado de los registros para el debug
    push %1 ; Codigo de excepcion
    call game_guardar_estado
    add esp, 4
    
    call sched_desalojar_tarea_actual

    mov ax, 0x70                    ; (idx_idle = 14) << 3;
    mov [sched_task_selector], ax   ; Cargo el selector de tss de la tarea idle
    jmp far [sched_task_offset]

    popad
    iret

%endmacro


;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 16
ISR 17
ISR 18
ISR 19

;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
    pushad

    call pic_finish1 ; Indica que la interrupcion fue atendida

    ; Finaliza el juego?
    call game_end
    cmp ax, 1
    jne .continuar_juego

        call game_finalizar
        call relojes
        hlt

    .continuar_juego:
        call nextClock   ; Imprimir el reloj del sistema
        call relojes     ; Imprimir relojes de las tareas

        cmp byte [modo_debug_activo], 0
        je .salto_prox_tarea

        .salto_tarea_idle: 
            mov ax, 0x70
            str cx
            cmp ax, cx 
            je .fin 
                mov [sched_task_selector], ax
                jmp far [sched_task_offset]
                jmp .fin

        .salto_prox_tarea:
            call game_actualizar_mundo
            mov BYTE [portal_creado], 0 ; Control para no crear más de un portal por tick de reloj
            call sched_nextTask
            str cx
            cmp ax, cx 
            je .fin 
                mov [sched_task_selector], ax
                jmp far [sched_task_offset]

        .fin:
            popad
            iret 

;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33
_isr33:
    pushad

    call pic_finish1

    in al, 0x60
    cmp al, 0x15 ; tecla y 
    je .tecla_y 
        ;Me fijo si estoy presionando tecla del 0 al 9 
        push eax
        call printScanCode
        add esp, 4
        jmp .fin 

    .tecla_y:
        cmp byte [modo_debug_activo], 1
        jne .activo_modo_debug

        call restaurar_pantalla
        print_text_pm  msg_int_Y, len_msg_Y, 0x00, 0, 35
        mov byte [modo_debug_activo], 0
        jmp .fin

        .activo_modo_debug:
            print_text_pm  msg_int_Y, len_msg_Y, 0x0F, 0, 35
            call imprimir_pantalla_debug
            mov byte [modo_debug_activo], 1

    .fin:
        popad
        iret

;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
syscall_ret: dd 0x00000000

syscall_x:  dd 0x00000000
syscall_y:  dd 0x00000000

global _isr137
_isr137:
    cmp BYTE [portal_creado], 1
    je .fin

    pushad
    push edx ; withMorty
    push ecx ; cross
    push ebx ; y
    push eax ; x
    call game_usePortalGun
    mov [syscall_ret], eax
    mov BYTE [portal_creado], 1
    add esp, 16
    popad
    mov eax, [syscall_ret] ; worked
    iret

    .fin: 
        mov eax, 0
        iret 

global _isr138
_isr138:
    pushad
    push eax
    call game_sumar_punto
    add esp, 4
    popad
    iret 

global _isr139
_isr139:
    pushad
    push syscall_y
    push syscall_x
    call game_whereIsMorty
    add esp, 8
    popad
    mov eax, [syscall_x]
    mov ebx, [syscall_y]
    iret 

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
nextClock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret
modo_debug_activo:      db 0
mostrando_debug: db 0

relojes:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
            mov DWORD [isrNumber], 0x0
            mov ebx, 0
        .ok:
            add ebx, isrClock
            push ebx
            call print_estado_cronenbergs
            add esp, 4
            popad
            ret