/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

/* Atributos paginas */
/* -------------------------------------------------------------------------- */
#define PAG_P                   0x00000001
#define PAG_R                   0x00000000
#define PAG_RW                  0x00000002
#define PAG_S                   0x00000000
#define PAG_US                  0x00000004

/* MMU */
/* -------------------------------------------------------------------------- */
#define PRESENT				1
#define READWRITE			1
#define SUPERVISOR			0
#define USER				1

#define PAGE_SIZE				0x00001000 // 4Kb

/* Misc */
/* -------------------------------------------------------------------------- */
#define SIZE_N                  40 // X
#define SIZE_M                  80 // Y

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 39 

#define GDT_IDX_NULL_DESC           0
/* Agregado por nosotros */
#define GDT_IDX_CS_RING_0			8
#define GDT_IDX_DS_RING_0			9
#define GDT_IDX_CS_RING_3			10
#define GDT_IDX_DS_RING_3			11
#define GDT_IDX_VM_RING_0			12
#define GDT_IDX_TAREA_INICIAL		13
#define GDT_IDX_TAREA_IDLE			14

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC << 3)


/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */
#define CS_RING_0 		0x40
#define DS_RING_0 		0x48
#define CS_RING_3 		0x53  // 0x50 | 0x3
#define DS_RING_3 		0x5B  // 0x58 | 0x3 
#define VM_RING_0 		0x60
	
/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR               0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                   0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                    0x000B8000 /* direccion fisica del buffer de video */

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
#define TASK_CODE             0x08000000 /* direccion virtual del codigo */
#define EBP_INIT 			  0x00027000
#define ESP_INIT			  0x00027000
#define KERNEL_STACK_INIT 	  0x27000
/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */
#define DIR_CODIGO_TAREA_IDLE  	0x0001A000
#define DIR_MUNDO_CRONENBERG	0x00400000

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR          0x00027000
#define KERNEL_PAGE_TABLE_0      0x00028000
#define CRYSTALS_MAP             0x00029000
#define DIR_INICIO_AREA_LIBRE	 0x00100000

/* Otras variables */
/* -------------------------------------------------------------------------- */
#define EFLAGS_DEFAULT 0x00000202
#define IOMAP_PORTS_DISABLED 0xFFFF
#define CANT_MAX_TAREAS  		24
#define DIR_TAREA_RICK_C137		0x10000
#define DIR_TAREA_MORTY_C137	0x12000
#define DIR_TAREA_RICK_D248		0x14000
#define DIR_TAREA_MORTY_D248	0x16000
#define DIR_TAREA_CRONENBERG	0x18000
#define TASK_SIZE				0x02000
#endif  /* !__DEFINES_H__ */
