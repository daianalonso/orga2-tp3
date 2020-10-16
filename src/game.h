/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "stdint.h"
#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"

void game_init();

uint32_t get_dir_fisica(uint8_t index, int *x, int *y);
uint32_t posicion2Fisica(int x, int y);
void 	game_sumar_punto(uint32_t cod_jugador);
void game_guardar_estado(uint32_t excepcion, uint32_t edi, uint32_t esi, uint32_t ebp, uint32_t ignorar, uint32_t ebx, uint32_t edx, 
                        uint32_t ecx, uint32_t eax, uint32_t errorCode, uint32_t eip, uint32_t cs, uint32_t eflags, uint32_t esp, uint32_t ss);
void game_whereIsMorty(int32_t* x, int32_t* y);
uint8_t game_usePortalGun(int32_t x, int32_t y, uint32_t cross, uint32_t withMorty);
void game_actualizar_mundo();
void print_estado_cronenbergs();
void game_finalizar();
uint8_t game_end();

#endif  /* !__GAME_H__ */
