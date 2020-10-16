/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "stdint.h"
#include "screen.h"
#include "tss.h"

#define TASK_RICK_C137 	0
#define TASK_MORTY_C137 1
#define TASK_RICK_D248 	2
#define TASK_MORTY_D248 3

typedef enum e_taskType {
    RickC137       = 1,
    MortyC137      = 2,
    RickD248       = 3,
    MortyD248      = 4,
    Cronenberg     = 5,
    CronenbergC137 = 6,
    CronenbergD248 = 7
} e_taskType_t;

typedef struct str_portal_info{
	int pos_x;
	int pos_y;
} __attribute__((__packed__)) portal;

typedef struct str_jugador {
	portal portal_activo;
	uint8_t contador_portales;
	uint32_t contador_puntaje;
} __attribute__((__packed__)) jugador;


typedef struct str_task_info {
	uint8_t viva;  
	int16_t selector_tss;
	e_taskType_t tipo;
	int pos_x;
	int pos_y;
} __attribute__((__packed__)) task_info;

typedef struct str_info_juego {
	uint8_t		  idx_tarea_actual;
	jugador       jugador_rojo;
	jugador       jugador_azul;
	task_info     tareas[CANT_MAX_TAREAS];
} __attribute__((__packed__)) info_juego;

void sched_init();

int16_t sched_nextTask();

void sched_desalojar_tarea_actual();

extern info_juego game_info;

#endif	/* !__SCHED_H__ */
