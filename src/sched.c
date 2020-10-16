/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

info_juego game_info;

void sched_init() {
	uint32_t indice_gdt = GDT_IDX_TAREA_IDLE + 1; // IDLE es la última

	//1. Inicializar arreglo de tareas en game_info.tareas[]
	// TAREAS RICK Y MORTY
	game_info.tareas[0].selector_tss = (indice_gdt << 3);
	game_info.tareas[0].tipo = 1;
	game_info.tareas[0].viva = 1; 
	tss_init_task(0, DIR_TAREA_RICK_C137, get_dir_fisica(0, &game_info.tareas[0].pos_x, &game_info.tareas[0].pos_y), indice_gdt);
	indice_gdt++;
	
	game_info.tareas[1].selector_tss = (indice_gdt << 3);
	game_info.tareas[1].tipo = 2;
	game_info.tareas[1].viva = 1;
	tss_init_task(1, DIR_TAREA_MORTY_C137, 	get_dir_fisica(1, &game_info.tareas[1].pos_x, &game_info.tareas[1].pos_y), indice_gdt);
	indice_gdt++;

	game_info.tareas[2].selector_tss = (indice_gdt << 3);
	game_info.tareas[2].tipo = 3;
	game_info.tareas[2].viva = 1;
	tss_init_task(2, DIR_TAREA_RICK_D248, 	get_dir_fisica(2, &game_info.tareas[2].pos_x, &game_info.tareas[2].pos_y), indice_gdt);
	indice_gdt++;

	game_info.tareas[3].selector_tss = (indice_gdt << 3);
	game_info.tareas[3].tipo = 4;
	game_info.tareas[3].viva = 1;
	tss_init_task(3, DIR_TAREA_MORTY_D248, 	get_dir_fisica(3, &game_info.tareas[3].pos_x, &game_info.tareas[3].pos_y), indice_gdt);
	indice_gdt++;

	// TAREAS CRONENBERG
	for (int i = 4; i < CANT_MAX_TAREAS; i++) {
		game_info.tareas[i].selector_tss = (indice_gdt << 3);
		game_info.tareas[i].tipo = 5;
		game_info.tareas[i].viva = 1;
		tss_init_task(i, DIR_TAREA_CRONENBERG, get_dir_fisica(i, &game_info.tareas[i].pos_x, &game_info.tareas[i].pos_y), indice_gdt);
		indice_gdt++;
	}

	//Indicar tarea actual
	game_info.idx_tarea_actual = 0;
	
}

int16_t sched_nextTask() {
	uint8_t i = game_info.idx_tarea_actual + 1;

	// Busco en la lista de tareas la siguiente tarea que no haya sido desalojada
	while ( i < CANT_MAX_TAREAS && !game_info.tareas[i].viva)
	{
		i++;
	}

	//Si llegue al final, sigo buscando 
	if(i == CANT_MAX_TAREAS){
		i = 0;
		while(i < game_info.idx_tarea_actual && !game_info.tareas[i].viva){
			i++;
		}
	}

	if (game_info.tareas[i].viva) {
		game_info.idx_tarea_actual = i;
		return game_info.tareas[i].selector_tss;
	} else {
		return (GDT_IDX_TAREA_IDLE << 3);
	}
}

void sched_desalojar_tarea_actual(){
	game_info.tareas[game_info.idx_tarea_actual].viva = 0;
}