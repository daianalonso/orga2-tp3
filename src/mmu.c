/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

#define DIR_VIRTUAL_TAREA 0x08000000

uint32_t proxima_pagina_libre;

void mmu_init() {
	proxima_pagina_libre = DIR_INICIO_AREA_LIBRE;
}

uint32_t mmu_nextFreeKernelPage() {
    uint32_t free_page = proxima_pagina_libre;
	proxima_pagina_libre += PAGE_SIZE;
	return free_page;
}

void mmu_mapPage(uint32_t cr3, uint32_t virtual, uint32_t phy, uint32_t attr_us, uint32_t attr_rw) {
	// Virtual = |directory|table|offset|
	//           |31-----22|21-12|11---0|

	uint32_t directory_index = virtual >> 22;				
	uint32_t table_index = (virtual >> 12) & 0x000003FF;

	pd_entry* page_directory = (pd_entry*) cr3;

	//Si la tabla no estaba presente la creamos
	if (!page_directory[directory_index].present) { // Si present = 0
		uint32_t* nueva_pagina = (uint32_t*) mmu_nextFreeKernelPage();
		for (int i = 0; i < 1024; ++i) nueva_pagina[i] = 0;
		page_directory[directory_index].present = PRESENT;
		page_directory[directory_index].read_write = READWRITE; // Permiso menos restrictivo
		page_directory[directory_index].user_supervisor = attr_us;
		page_directory[directory_index].page_table_base = (uint32_t) nueva_pagina >> 12;
	} 

	pt_entry* page_table = (pt_entry*) (page_directory[directory_index].page_table_base << 12);
	page_table[table_index].present = PRESENT;
	page_table[table_index].read_write = attr_rw;
	page_table[table_index].user_supervisor = attr_us;
	page_table[table_index].physical_address_base = phy >> 12;

	tlbflush();
}

uint32_t mmu_unmapPage(uint32_t cr3, uint32_t virtual) {
	// Virtual = |directory|table|offset|
	//           |31-----22|21-12|11---0|

	uint32_t directory_index = virtual >> 22;				
	uint32_t table_index = (virtual >> 12) & 0x000003FF;

	pd_entry* page_directory = (pd_entry*) cr3;

	if (page_directory[directory_index].present) { 
		pt_entry* page_table = (pt_entry*)(page_directory[directory_index].page_table_base << 12);
		page_table[table_index].present = 0;
		tlbflush();
		return 1;
	} else {
    	return 0;
	}
}

uint32_t mmu_initKernelDir() {
	// ** PAGE DIRECTORY **
	pd_entry* page_directory = (pd_entry*) KERNEL_PAGE_DIR;  // 0x27000
	page_directory[0].present 		  = PRESENT;
	page_directory[0].read_write 	  = READWRITE;
	page_directory[0].user_supervisor = SUPERVISOR;
	page_directory[0].page_table_base = ((uint32_t) KERNEL_PAGE_TABLE_0) >> 12; // 0x28000

	// Pongo todo el resto de las posiciones en cero
	for (int i = 1; i < 1024; ++i) page_directory[i] = (pd_entry){0};

	// ** PAGE TABLE **
	pt_entry* page_table_0 = (pt_entry*) KERNEL_PAGE_TABLE_0;
	for (int i = 0; i < 1024; ++i) {
		page_table_0[i].present 			  = PRESENT;
		page_table_0[i].read_write 			  = READWRITE;
		page_table_0[i].user_supervisor 	  = SUPERVISOR;
		page_table_0[i].physical_address_base = i;	 
	}

	return (uint32_t) page_directory;
}

// dir_fisica: 0x400000 - 0x1CFFFFF
uint32_t mmu_initTaskDir(uint32_t codigo_original_tarea, uint32_t dir_fisica){

	// ** PAGE DIRECTORY **
	// Obtenemos nuevo page directory en el área libre del kernel
	pd_entry* page_directory = (pd_entry*) mmu_nextFreeKernelPage();

	for (int i = 0; i < 1024; ++i) page_directory[i] = (pd_entry){0};

	// Identity mapping primeros 4Mb (kernel)
	for (int i = 0; i < 1024; ++i)
		mmu_mapPage((uint32_t) page_directory, PAGE_SIZE * i, PAGE_SIZE * i, SUPERVISOR, READWRITE);

	// Mapeamos el código de la tarea (8KB) en el directorio de la tarea 
	mmu_mapPage((uint32_t) page_directory, DIR_VIRTUAL_TAREA, dir_fisica, USER, READWRITE);
	mmu_mapPage((uint32_t) page_directory, DIR_VIRTUAL_TAREA + PAGE_SIZE, dir_fisica + PAGE_SIZE, USER, READWRITE);

	uint32_t dir_virtual = dir_fisica; // Tomamos la dirección física como virtual

	mmu_mapPage(rcr3(), dir_virtual, dir_fisica, SUPERVISOR, READWRITE);
	mmu_mapPage(rcr3(), dir_virtual + PAGE_SIZE, dir_fisica + PAGE_SIZE, SUPERVISOR, READWRITE);

	tlbflush();

	// Copiamos el código de la tarea (8k)
	char* src = (char*) codigo_original_tarea;
	char* dst = (char*) dir_virtual;
	
	for (int i = 0; i < 2*PAGE_SIZE; ++i) dst[i] = src[i];

	mmu_unmapPage(rcr3(), dir_virtual);
	mmu_unmapPage(rcr3(), dir_virtual + PAGE_SIZE);

	tlbflush();

	return (uint32_t) page_directory;
}