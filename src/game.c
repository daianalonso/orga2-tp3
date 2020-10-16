/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "task.h"

// Dado un (x, y) devuelve la dirección física en el Mundo Cronenberg
uint32_t posicion2Fisica(int x, int y) {
	return (DIR_MUNDO_CRONENBERG + TASK_SIZE * ((uint32_t)y * VIDEO_COLS + (uint32_t)x)); 
}

// Dado un (x, y) obtiene una dirección aleatorio en el Mundo Cronenberg 
uint32_t get_dir_fisica(uint8_t index, int *x, int *y) {
	getRandomPosition(index, x, y);
	return posicion2Fisica(*x, *y); 
}

int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

void game_init() {
	//Inicializar jugadores
	// JUGADOR ROJO (RICK C137)
	game_info.jugador_rojo.portal_activo.pos_x = -1;
	game_info.jugador_rojo.portal_activo.pos_y = -1;
	game_info.jugador_rojo.contador_puntaje = 0;
	game_info.jugador_rojo.contador_portales = 0;

	// JUGADOR AZUL (RICK D248)
	game_info.jugador_azul.portal_activo.pos_x = -1;
	game_info.jugador_azul.portal_activo.pos_y = -1;
	game_info.jugador_azul.contador_puntaje = 0;
	game_info.jugador_azul.contador_portales = 0;

	//Imprimir info en pantalla
	print("0000000", 4, 45, C_FG_WHITE|C_BG_BLUE);
	print("0000000", 69, 45, C_FG_WHITE|C_BG_RED);

	game_actualizar_mundo();

	//Imprimimos los contadores de cronenberg 
	for (int i = 0 ; i < 10; ++i) print_dec(i, 2, 21+i*4, 43, C_FG_LIGHT_GREY|C_BG_BLACK);
	for (int i = 10; i < 20; ++i) print_dec(i, 2, 21+(i-10)*4, 46, C_FG_LIGHT_GREY|C_BG_BLACK);
}

void game_sumar_punto(uint32_t cod_jugador){
	//breakpoint();
	int i = game_info.idx_tarea_actual;
	if(game_info.tareas[i].tipo == Cronenberg){
		if (cod_jugador == 0xC137) {
			game_info.jugador_rojo.contador_puntaje++;
			game_info.tareas[i].tipo = CronenbergC137;
			print_dec(game_info.jugador_rojo.contador_puntaje, 7, 69, 45, C_FG_WHITE|C_BG_RED);
		} else if (cod_jugador == 0xD248) {
			game_info.jugador_azul.contador_puntaje++;
			game_info.tareas[i].tipo = CronenbergD248;
			print_dec(game_info.jugador_azul.contador_puntaje, 7, 4, 45, C_FG_WHITE|C_BG_BLUE);
		}
		//Desalojar tarea 
		//sched_desalojar_tarea_actual();
	}
}


//Guarda el estado de la CPU  
void game_guardar_estado(uint32_t excepcion, uint32_t edi, uint32_t esi, uint32_t ebp, uint32_t original_esp, uint32_t ebx, uint32_t edx, 
                        uint32_t ecx, uint32_t eax, uint32_t errorCode, uint32_t eip, uint32_t cs, uint32_t eflags, uint32_t esp, uint32_t ss) {
    
	info_debug.excepcion = excepcion;

    info_debug.edi = edi;
    info_debug.esi = esi;
    info_debug.ebp = ebp;
    info_debug.ebx = ebx;
    info_debug.edx = edx;
    info_debug.ecx = ecx;
    info_debug.eax = eax;
    info_debug.ss  = ss;
    info_debug.esp = esp;
    info_debug.eip = eip;
    info_debug.eflags = eflags;

    info_debug.cs = cs;
    info_debug.ds = ss;
    info_debug.es = ss;
    info_debug.fs = ss;
    info_debug.gs = ss;

    info_debug.cr0 = rcr0();
    info_debug.cr2 = rcr2();
    info_debug.cr3 = rcr3();
    info_debug.cr4 = rcr4();
	
    info_debug.stack1 = *((uint32_t*)(info_debug.esp));
    info_debug.stack2 = *((uint32_t*)(info_debug.esp + 4));
    info_debug.stack3 = *((uint32_t*)(info_debug.esp + 8));
    info_debug.stack4 = *((uint32_t*)(info_debug.esp + 12));
    info_debug.stack5 = *((uint32_t*)(info_debug.esp + 16));
}

// Pre: la tarea actual es una Rick 
void game_whereIsMorty(int32_t* x, int32_t* y) {
	if (game_info.tareas[game_info.idx_tarea_actual].tipo == RickC137) {
		*x = game_info.tareas[TASK_MORTY_C137].pos_x - game_info.tareas[game_info.idx_tarea_actual].pos_x;
		*y = game_info.tareas[TASK_MORTY_C137].pos_y - game_info.tareas[game_info.idx_tarea_actual].pos_y; 
	} else if(game_info.tareas[game_info.idx_tarea_actual].tipo == RickD248) {
		*x = game_info.tareas[TASK_MORTY_D248].pos_x - game_info.tareas[game_info.idx_tarea_actual].pos_x;
		*y = game_info.tareas[TASK_MORTY_D248].pos_y - game_info.tareas[game_info.idx_tarea_actual].pos_y;
	}
} 

// Copia el contenido de dir_fisica_src en dir_fisica_dst
void copiarMemoria(uint32_t cr3, uint32_t dir_virtual_src, uint32_t dir_fisica_src, uint32_t dir_virtual_dst, uint32_t dir_fisica_dst) {

	// Mapear la dirección física de origen
	mmu_mapPage(cr3, dir_virtual_src, dir_fisica_src, SUPERVISOR, READWRITE);
	mmu_mapPage(cr3, dir_virtual_src + PAGE_SIZE, dir_fisica_src + PAGE_SIZE, SUPERVISOR, READWRITE);

	// Mapear la dirección física de destino
	mmu_mapPage(cr3, dir_virtual_dst, dir_fisica_dst, SUPERVISOR, READWRITE);
	mmu_mapPage(cr3, dir_virtual_dst + PAGE_SIZE, dir_fisica_dst + PAGE_SIZE, SUPERVISOR, READWRITE);

	tlbflush(); 

	// Copiamos el contenido
	char* src  = (char*) dir_fisica_src;
	char* dest = (char*) dir_fisica_dst;
	for(int i = 0; i < 2 * PAGE_SIZE; i++) dest[i] = src[i];

	mmu_unmapPage(cr3, dir_virtual_src);
	mmu_unmapPage(cr3, dir_virtual_src + PAGE_SIZE);
	mmu_unmapPage(cr3, dir_virtual_dst);
	mmu_unmapPage(cr3, dir_virtual_dst + PAGE_SIZE);

	tlbflush();
}

void game_cruzarPortal(uint32_t cr3, uint32_t dir_fisica_src, uint32_t dir_fisica_dst) {
	//Copiar el codigo a la dirección donde se moverá el Rick
	copiarMemoria(rcr3(), dir_fisica_src, dir_fisica_src, dir_fisica_dst, dir_fisica_dst);
	// Mapeamos la nueva posición
	mmu_mapPage(cr3, TASK_CODE, dir_fisica_dst, USER, READWRITE);
	mmu_mapPage(cr3, TASK_CODE + PAGE_SIZE, dir_fisica_dst + PAGE_SIZE, USER, READWRITE);
}

uint8_t game_crearPortalRick(int32_t x, int32_t y, uint32_t cross, uint32_t withMorty) {
	uint8_t worked = 1;
	task_info* tarea_actual = &game_info.tareas[game_info.idx_tarea_actual];
	jugador* jugador_actual = (tarea_actual->tipo == RickC137)? &game_info.jugador_rojo : &game_info.jugador_azul;
	uint8_t	idx_morty = (tarea_actual->tipo == RickC137)? TASK_MORTY_C137 : TASK_MORTY_D248;	

	jugador_actual->contador_portales++;

	uint8_t nuevo_x = mod((tarea_actual->pos_x + x), SIZE_M);
	uint8_t nuevo_y = mod((tarea_actual->pos_y + y), SIZE_N);

	uint32_t dir_fisica_src = posicion2Fisica(tarea_actual->pos_x, tarea_actual->pos_y);
	uint32_t dir_fisica_dst = posicion2Fisica(nuevo_x, nuevo_y);

	uint32_t cr3_tarea = tss_directory[game_info.idx_tarea_actual].cr3;

	if (cross) {

		// Actualizamos la posición en el mundo
		tarea_actual->pos_x = nuevo_x;
		tarea_actual->pos_y = nuevo_y;

		game_cruzarPortal(cr3_tarea, dir_fisica_src, dir_fisica_dst);
		
		if (withMorty) {
			int8_t pos_x_morty;
			int8_t pos_y_morty;
			uint32_t dir_fisica_src_morty;
			uint32_t dir_fisica_dst_morty;

			pos_x_morty = game_info.tareas[idx_morty].pos_x;
			pos_y_morty = game_info.tareas[idx_morty].pos_y;

			nuevo_x = mod((pos_x_morty + x), SIZE_M);
			nuevo_y = mod((pos_y_morty + y), SIZE_N);

			dir_fisica_src_morty = posicion2Fisica(pos_x_morty, pos_y_morty);
			dir_fisica_dst_morty = posicion2Fisica(nuevo_x, nuevo_y);

			game_info.tareas[idx_morty].pos_x = nuevo_x;
			game_info.tareas[idx_morty].pos_y = nuevo_y;

			game_cruzarPortal(tss_directory[idx_morty].cr3, dir_fisica_src_morty, dir_fisica_dst_morty);
		}
	} else {
		if (jugador_actual->portal_activo.pos_x == -1 && jugador_actual->portal_activo.pos_y == -1) {
			jugador_actual->portal_activo.pos_x = tarea_actual->pos_x;
			jugador_actual->portal_activo.pos_y = tarea_actual->pos_y;
		}

		jugador_actual->portal_activo.pos_x = mod((jugador_actual->portal_activo.pos_x + x), SIZE_M);
		jugador_actual->portal_activo.pos_y = mod((jugador_actual->portal_activo.pos_y + y), SIZE_N);
		dir_fisica_dst = posicion2Fisica(jugador_actual->portal_activo.pos_x, jugador_actual->portal_activo.pos_y);

		mmu_mapPage(cr3_tarea, 0x08002000, dir_fisica_dst, USER, READWRITE);
		mmu_mapPage(cr3_tarea, 0x08002000 + PAGE_SIZE, dir_fisica_dst + PAGE_SIZE, USER, READWRITE);

		if (withMorty) {
			mmu_mapPage(tss_directory[idx_morty].cr3, 0x08002000, dir_fisica_dst, USER, READWRITE);
			mmu_mapPage(tss_directory[idx_morty].cr3, 0x08002000 + PAGE_SIZE, dir_fisica_dst + PAGE_SIZE, USER, READWRITE);
		}
	}
	
	return worked;
}

uint8_t game_crearPortalMorty(int32_t x, int32_t y, uint32_t cross, uint32_t withMorty) {
	uint8_t worked = 0;
	task_info* tarea_actual = &game_info.tareas[game_info.idx_tarea_actual];
	jugador* jugador_actual = (tarea_actual->tipo == RickC137)? &game_info.jugador_rojo : &game_info.jugador_azul;

	uint8_t nuevo_x = mod((tarea_actual->pos_x + x), SIZE_M);
	uint8_t nuevo_y = mod((tarea_actual->pos_y + y), SIZE_N);

	uint32_t dir_fisica_src = posicion2Fisica(tarea_actual->pos_x, tarea_actual->pos_y);
	uint32_t dir_fisica_dst = posicion2Fisica(nuevo_x, nuevo_y);

	uint32_t cr3_tarea = tss_directory[game_info.idx_tarea_actual].cr3;
	//Abre el portal solo si su Rick ya llamó 10 veces a usePortalGun
	if (jugador_actual->contador_portales == 10) {
		jugador_actual->contador_portales = 0; //Reseteamos el contador 

		if (cross) {
			// Actualizamos la posición en el mundo
			tarea_actual->pos_x = nuevo_x;
			tarea_actual->pos_y = nuevo_y;

			game_cruzarPortal(cr3_tarea, dir_fisica_src, dir_fisica_dst);
		} else {
			mmu_mapPage(cr3_tarea, 0x08002000, dir_fisica_dst, USER, READWRITE);
			mmu_mapPage(cr3_tarea, 0x08002000 + PAGE_SIZE, dir_fisica_dst + PAGE_SIZE, USER, READWRITE);
		}
		worked = 1;
	}

	return worked;
}

// Pre: la tarea actual es Rick o Morty
uint8_t game_usePortalGun(int32_t x, int32_t y, uint32_t cross, uint32_t withMorty) {	
	uint8_t worked = 0;
	task_info* tarea_actual = &game_info.tareas[game_info.idx_tarea_actual];

	//Si quiere crear un portal un Rick 
	if (tarea_actual->tipo == RickC137 || tarea_actual->tipo == RickD248) {
		worked = game_crearPortalRick(x, y, cross, withMorty);
	} else if (tarea_actual->tipo == MortyC137 || tarea_actual->tipo == MortyD248) {
		worked = game_crearPortalMorty(x, y, cross, withMorty);
	}
	
	return worked; 
}

//Chequea las condiciones para que finalice el juego
uint8_t game_end() {
	if ((game_info.jugador_rojo.contador_puntaje == 20 && game_info.jugador_azul.contador_puntaje == 0) || 
		(game_info.jugador_rojo.contador_puntaje == 0 && game_info.jugador_azul.contador_puntaje == 20)){
		return 1;
	}else{
		if(!game_info.tareas[TASK_MORTY_C137].viva || !game_info.tareas[TASK_MORTY_D248].viva ||
			!game_info.tareas[TASK_RICK_C137].viva || !game_info.tareas[TASK_RICK_C137].viva){
			return 1;
		}
	}
	return 0;
}


//Funciones auxiliares para imprimir los datos en pantalla:

void game_actualizar_mundo() {
	screen_drawBox(1, 0, 40, 80, 0, C_FG_WHITE|C_BG_GREEN);

	for (int i = 0; i < CANT_MAX_TAREAS; ++i) {
		char* personaje;
		uint16_t color;
		uint8_t tiene_portal;
		e_taskType_t tipo = game_info.tareas[i].tipo;

		if(game_info.tareas[i].viva){
			if (tipo == RickC137) {
				personaje = "R";
				color = C_FG_WHITE|C_BG_RED;
				tiene_portal = (game_info.jugador_rojo.portal_activo.pos_x != -1 && game_info.jugador_rojo.portal_activo.pos_y != -1);
			} else if (tipo == RickD248) {
				personaje = "R";
				color = C_FG_WHITE|C_BG_BLUE;
				tiene_portal = (game_info.jugador_azul.portal_activo.pos_x != -1 && game_info.jugador_azul.portal_activo.pos_y != -1);
			} else if (tipo == MortyC137) {
				personaje = "M";
				color = C_FG_WHITE|C_BG_RED;
			} else if (tipo == MortyD248) {
				personaje = "M";
				color = C_FG_WHITE|C_BG_BLUE;
			} else if (tipo == Cronenberg) {
				personaje = "+";
				color = C_FG_WHITE|C_BG_BROWN;
			}else if (tipo == CronenbergC137) {
				personaje = "+";
				color = C_FG_WHITE|C_BG_RED;
			} else {
				personaje = "+";
				color = C_FG_WHITE|C_BG_BLUE;
			}
		}else{
			personaje = "x";
			color = C_FG_GREEN|C_BG_GREEN;
		}

		print(personaje, game_info.tareas[i].pos_x, game_info.tareas[i].pos_y + 1, color);

		if (tipo == RickC137 && tiene_portal) {
			print("*", game_info.jugador_rojo.portal_activo.pos_x, game_info.jugador_rojo.portal_activo.pos_y + 1, C_FG_RED|C_BG_GREEN);
		} else if (tipo == RickD248 && tiene_portal) {
			print("*", game_info.jugador_azul.portal_activo.pos_x, game_info.jugador_azul.portal_activo.pos_y + 1, C_FG_BLUE|C_BG_GREEN);
		}
	}
}

void print_estado_cronenbergs(char* clock) {
	char* estado;
	char  clock_char = clock[0];

	uint32_t x = 21;
    for (int i = 4; i < CANT_MAX_TAREAS - 10; i++) {
        if (game_info.tareas[i].viva) {
        	estado = &clock_char;
        } else {
        	estado = "X";
        }
        if(game_info.tareas[i].tipo == CronenbergC137){
        	print(estado, x, 44, C_FG_RED|C_BG_BLACK);
        }else if(game_info.tareas[i].tipo == CronenbergD248){
        	print(estado, x, 44, C_FG_BLUE|C_BG_BLACK);
        }else{
        	print(estado, x, 44, C_FG_WHITE|C_BG_BLACK);
        }
        print(" ", x+1, 44, C_FG_BLACK|C_BG_BLACK);
        x += 4;
    } 

    x = 21;
    for (int i = CANT_MAX_TAREAS - 10; i < CANT_MAX_TAREAS; i++) {
        if (game_info.tareas[i].viva) {
        	estado = &clock_char;
        } else {
        	estado = "X";
        }
        if(game_info.tareas[i].tipo == CronenbergC137){
        	print(estado, x, 47, C_FG_RED|C_BG_BLACK);
        }else if(game_info.tareas[i].tipo == CronenbergD248){
        	print(estado, x, 47, C_FG_BLUE|C_BG_BLACK);
        }else{
        	print(estado, x, 47, C_FG_WHITE|C_BG_BLACK);
        }
        print(" ", x+1, 47, C_FG_BLACK|C_BG_BLACK);
        x += 4;
    } 
}


int contarPorCapturar(){
	int aCapturar = CANT_MAX_TAREAS - 4;
	for(int i = 4; i < CANT_MAX_TAREAS; i++){
		if(!game_info.tareas[i].viva || game_info.tareas[i].tipo == CronenbergD248 || game_info.tareas[i].tipo == CronenbergC137){
			aCapturar--;
		}
	}
	return aCapturar;
}


void game_finalizar() {
	game_actualizar_mundo();
	print("FIN DEL JUEGO!", 33, 18, C_FG_WHITE | C_BG_BLACK);
	
	if(!game_info.tareas[TASK_MORTY_C137].viva){
		print("GANADOR: ", 31, 20, C_FG_WHITE | C_BG_BLACK);
		print("RICK D248", 40, 20, C_FG_WHITE | C_BG_BLUE);
		print("MURIO MORTY C137", 32, 20, C_FG_WHITE | C_BG_BLACK);
	}else if (!game_info.tareas[TASK_MORTY_D248].viva){
		print("GANADOR: ", 31, 20, C_FG_WHITE | C_BG_BLACK);
		print("RICK C137", 40, 20, C_FG_WHITE | C_BG_RED);
		print("MURIO MORTY D248", 32, 20, C_FG_WHITE | C_BG_BLACK);	
	}else if(!game_info.tareas[TASK_RICK_C137].viva){
		print("GANADOR: ", 31, 20, C_FG_WHITE | C_BG_BLACK);
		print("RICK D248", 40, 20, C_FG_WHITE | C_BG_BLUE);
	}else if(!game_info.tareas[TASK_RICK_D248].viva){
		print("GANADOR: ", 31, 20, C_FG_WHITE | C_BG_BLACK);
		print("RICK C137", 32, 20, C_FG_WHITE | C_BG_RED);
	}else{
		int cant_por_capturar = contarPorCapturar();
		if(cant_por_capturar == 0){
			int ganador;
			if(game_info.jugador_rojo.contador_puntaje == game_info.jugador_azul.contador_puntaje){
				print("EMPATE", 34, 20, C_FG_WHITE | C_BG_BLACK);
			}else{
				print("GANADOR: ", 31, 20, C_FG_WHITE | C_BG_BLACK);
				if(game_info.jugador_rojo.contador_puntaje > game_info.jugador_azul.contador_puntaje) {
				ganador = 1;
				} else {
				ganador = 2;
				}
				print(ganador == 1 ? "RICK C137" : "RICK D248", 40, 20, C_FG_WHITE | (ganador == 1 ? C_BG_RED : C_BG_BLUE));
			}
		}
	}
}
