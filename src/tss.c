/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_initial;
tss tss_idle;

tss tss_directory[CANT_MAX_TAREAS];

void tss_init() {
	//Seteo la TSS inicial en 0
    tss_initial = (tss) {0};
	// Actualizo el comienzo de la TSS en la GDT
	gdt[GDT_IDX_TAREA_INICIAL].base_0_15 =  (uint32_t) &tss_initial & 0x0000FFFF;
	gdt[GDT_IDX_TAREA_INICIAL].base_23_16 = ((uint32_t) &tss_initial & 0x00FF0000) >> 16;
	gdt[GDT_IDX_TAREA_INICIAL].base_31_24 = (uint32_t) &tss_initial >> 24;
}

void tss_init_idle(){
	// Inicializo la TSS de la tarea IDLE
	tss_idle.cr3    = KERNEL_PAGE_DIR;
	tss_idle.eip    = DIR_CODIGO_TAREA_IDLE;
	tss_idle.ebp    = KERNEL_STACK_INIT;
	tss_idle.esp    = KERNEL_STACK_INIT;
	tss_idle.eflags = EFLAGS_DEFAULT; //Interrupciones activas
	tss_idle.cs     = CS_RING_0;
	tss_idle.ds     = DS_RING_0;
	tss_idle.es     = DS_RING_0;
	tss_idle.gs     = DS_RING_0;
	tss_idle.fs     = DS_RING_0;
	tss_idle.ss     = DS_RING_0;
	tss_idle.eax    = 0;
	tss_idle.ebx    = 0;
	tss_idle.ecx    = 0;
	tss_idle.edx    = 0;
	tss_idle.iomap  = IOMAP_PORTS_DISABLED;

	// Actualizo el comienzo de la TSS en la GDT
	gdt[GDT_IDX_TAREA_IDLE].base_0_15 =  (uint32_t) &tss_idle & 0x0000FFFF;
	gdt[GDT_IDX_TAREA_IDLE].base_23_16 = ((uint32_t) &tss_idle & 0x00FF0000) >> 16;
	gdt[GDT_IDX_TAREA_IDLE].base_31_24 = (uint32_t) &tss_idle >> 24;
}


void tss_cargar_gdt(tss* tss_p, int gdt_idx){

	uint32_t tss = (uint32_t) tss_p;
	
	gdt[gdt_idx].base_0_15 = tss & 0x0000FFFF;
	gdt[gdt_idx].base_23_16 = (tss & 0x00FF0000) >> 16;
	gdt[gdt_idx].base_31_24 = (tss >> 24);
	gdt[gdt_idx].limit_0_15 = 0x0067;
	gdt[gdt_idx].limit_16_19 = 0x0;
	gdt[gdt_idx].type = 0x9;
	gdt[gdt_idx].s = 0x0;
	gdt[gdt_idx].dpl = 0x0;
	gdt[gdt_idx].p = 0x1;
	gdt[gdt_idx].avl = 0x0;
	gdt[gdt_idx].db = 0x0;
	gdt[gdt_idx].l = 0x0;
	gdt[gdt_idx].g = 0x0;
}


void tss_init_task(uint8_t id, uint32_t codigo_tarea, uint32_t dir_fisica, uint32_t indice_gdt){

		tss* tss_new_task = &tss_directory[id];
		
		// Cargo en la GDT la tarea
		tss_cargar_gdt(tss_new_task, indice_gdt);

		// Inicializo el TSS de la tarea 
		tss_new_task->cr3    = mmu_initTaskDir(codigo_tarea, dir_fisica);
		tss_new_task->eip    = TASK_CODE;
		tss_new_task->ebp    = TASK_CODE + PAGE_SIZE;
		tss_new_task->esp    = TASK_CODE + PAGE_SIZE;
		tss_new_task->eflags = EFLAGS_DEFAULT;
		tss_new_task->cs     = CS_RING_3;
		tss_new_task->ds     = DS_RING_3;
		tss_new_task->es     = DS_RING_3;
		tss_new_task->gs     = DS_RING_3;
		tss_new_task->fs     = DS_RING_3;
		tss_new_task->ss     = DS_RING_3;
		tss_new_task->esp0   = mmu_nextFreeKernelPage() + PAGE_SIZE;
		tss_new_task->ss0    = DS_RING_0;

		tss_new_task->eax    = 0;
		tss_new_task->ebx    = 0;
		tss_new_task->ecx    = 0;
		tss_new_task->edx    = 0;

		tss_new_task->iomap  = IOMAP_PORTS_DISABLED;
}