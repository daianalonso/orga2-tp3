; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

global start
extern GDT_DESC
extern IDT_DESC
extern idt_init
extern pic_reset
extern pic_enable
extern mmu_init
extern mmu_initKernelDir
extern mmu_initTaskDir
extern print_students_data
extern tss_init
extern tss_init_idle
extern sched_init
extern game_init

%define CS_RING_0       0x40
%define DS_RING_0       0x48
%define VM_RING_0       0x60
%define KERNEL_STACK_INIT  0x27000
%define SELECTOR_TSS_INICIAL 0x68
%define SELECTOR_TSS_IDLE  0x70

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0
    

    ; Habilitar A20
    call A20_disable
    call A20_check
    call A20_enable
    call A20_check

    ; Cargar la GDT
    lgdt [GDT_DESC]
    
    ; Setear el bit PE del registro CR0
    mov eax, CR0
    or eax, 0x1
    mov CR0, eax

    ; Saltar a modo protegido
    jmp CS_RING_0:modo_protegido

BITS 32
modo_protegido:
    ; Establecer selectores de segmentos
    mov ax, DS_RING_0
    mov ds, ax
    mov ss, ax
    mov fs, ax
    mov es, ax
    mov gs, ax

    ; Establecer la base de la pila
    mov ebp, KERNEL_STACK_INIT
    mov esp, KERNEL_STACK_INIT   

    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 0,0

    ; Inicializar pantalla
    mov ax, VM_RING_0
    mov gs, ax

    call init_screen 
    
    ; Inicializar el manejador de memoria
    call mmu_init

    ; Inicializar el directorio de paginas
    call mmu_initKernelDir

    ; Cargar directorio de paginas
    mov eax, KERNEL_STACK_INIT 
    mov CR3, eax ; Esto ya limpia PCD y PWT

    ; Habilitar paginacion
    mov eax, CR0
    or eax, 0x80000000
    mov CR0, eax

    ; Inicializar tss
    call tss_init

    ; Inicializar tss de la tarea Idle
    call tss_init_idle

    ; Inicializar el scheduler
    call sched_init

    ; Inicializar juego
    call game_init
    
    ; Inicializar la IDT
    call idt_init 

    ; Cargar IDT
 	lidt [IDT_DESC] 

    ; Configurar controlador de interrupciones
    call pic_reset
    call pic_enable
    
    ; Cargar tarea inicial
    mov ax, SELECTOR_TSS_INICIAL
    ltr ax
	
    ; Habilitar interrupciones
    sti 

    ;xchg bx, bx
    ; Saltar a la primera tarea: Idle
    jmp SELECTOR_TSS_IDLE:0

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
%include "screen.asm"
