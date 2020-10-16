/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (uint16_t)    0x0000,         /* limit[0:15]  */
        (uint16_t)    0x0000,         /* base[0:15]   */
        (uint8_t)     0x00,           /* base[23:16]  */
        (uint8_t)     0x00,           /* type         */
        (uint8_t)     0x00,           /* s            */
        (uint8_t)     0x00,           /* dpl          */
        (uint8_t)     0x00,           /* p            */
        (uint8_t)     0x00,           /* limit[16:19] */
        (uint8_t)     0x00,           /* avl          */
        (uint8_t)     0x00,           /* l            */
        (uint8_t)     0x00,           /* db           */
        (uint8_t)     0x00,           /* g            */
        (uint8_t)     0x00,           /* base[31:24]  */
    },
    /* SEGMENTO DE CODIGO NIVEL 0 */
    [GDT_IDX_CS_RING_0] = (gdt_entry){
        .base_0_15 = 0x0000,
        .base_23_16 = 0x00,
        .base_31_24 = 0x00,
        .limit_0_15 = 0x88FF,
        .limit_16_19 = 0x00,
        .type = 8, //Execute-only
        .s = 1,
        .dpl = 0, //Privilegio 0
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 1,//Granularidad en kb 
    },
    /* SEGMENTO DE CODIGO NIVEL 3*/
    [GDT_IDX_CS_RING_3] = (gdt_entry){
        .base_0_15 = 0x0000,
        .base_23_16 = 0x00,
        .base_31_24 = 0x00,
        .limit_0_15 = 0x88FF,
        .limit_16_19 = 0x00,
        .type = 8, //Execute-only
        .s = 1,
        .dpl = 3, //Privilegio 3
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 1, //Granularidad en kb 
    },
    /* SEGMENTO DE DATOS NIVEL 0 */
    [GDT_IDX_DS_RING_0] = (gdt_entry){
        .base_0_15 = 0x0000,
        .base_23_16 = 0x00,
        .base_31_24 = 0x00,
        .limit_0_15 = 0x88FF,
        .limit_16_19 = 0x00,
        .type = 2, //Read/write
        .s = 1,
        .dpl = 0, //Privilegio 0
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 1, //Granularidad en kb 
    },
    /* SEGMENTO DE DATOS NIVEL 3 */
    [GDT_IDX_DS_RING_3] = (gdt_entry){
        .base_0_15 = 0x0000,
        .base_23_16 = 0x00,
        .base_31_24 = 0x00,
        .limit_0_15 = 0x88FF,
        .limit_16_19 = 0x00,
        .type = 2, //Read/write
        .s = 1,
        .dpl = 3, //Privilegio 3
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 1, //Granularidad en kb 
    },
    /* SEGMENTO DE VIDEO NIVEL 0*/
    [GDT_IDX_VM_RING_0] = (gdt_entry){
        .base_0_15 = 0x8000,
        .base_23_16 = 0x0B,
        .base_31_24 = 0x00,
        .limit_0_15 = 0x1F3F,
        .limit_16_19 = 0x00,
        .type = 2, //Read/write
        .s = 1,
        .dpl = 0, //Privilegio 0
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 0, //Granularidad en byte 
    },
    /* TSS TAREA INICIAL */
   [GDT_IDX_TAREA_INICIAL] = (gdt_entry){
        .base_0_15 = 0x0000,
        .base_23_16 = 0x00,
        .base_31_24 = 0x00,
        .limit_0_15 = 0x0067,
        .limit_16_19 = 0x00,
        .type = 9, // 10B1 B = 0 -> 1001 = 9
        .s = 0,
        .dpl = 0, //Privilegio 0
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 0, //Granularidad en byte 
    },
    /* TSS TAREA IDLE */
   [GDT_IDX_TAREA_IDLE] = (gdt_entry){
        .base_0_15 = 0x0000,
        .base_23_16 = 0x00,
        .base_31_24 = 0x00,
        .limit_0_15 = 0x0067,
        .limit_16_19 = 0x00,
        .type = 9,
        .s = 0,
        .dpl = 0, //Privilegio 0
        .p = 1,
        .avl = 0,
        .l = 0,
        .db = 1,
        .g = 0, //Granularidad en byte 
    }
};



gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (uint32_t) &gdt
};
