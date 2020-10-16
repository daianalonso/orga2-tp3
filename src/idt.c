/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"
#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (uint32_t) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);
        ...
    }
*/
                        //     P DPL D
#define INTR_KERNEL 0x8E00  // 1000 1110 0000 0000  D = 1, DPL = 0
#define INTR_USER   0xEE00  // 1110 1110 0000 0000  D = 1, DPL = 3  

#define IDT_ENTRY(numero, tipo_puerta)                                                             \
    idt[numero].offset_0_15 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF);        \
    idt[numero].segsel = (uint16_t) CS_RING_0;                                                     \
    idt[numero].attr = (uint16_t) tipo_puerta;                                                     \
    idt[numero].offset_16_31 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);

char* msjExcepcion[20] = {"DIVIDE ERROR", "RESERVED", "NMI INTERRUPT", "BREAKPOINT", "OVERFLOW", 
                        "BOUND RANGE EXCEEDED", "INVALID OPCODE", "DEVICE NOT AVAILABLE", 
                        "DOUBLE FAULT", "COPROCESSOR SEGMENT OVERRUN", "INVALID TSS", "SEGMENT NOT PRESENT", "STACK-SEGMENT FAULT",
                        "GENERAL PROTECTION", "PAGE FAULT", "..", "X87 FPU FLOATING-POINT ERROR", "ALIGMENT CHECK",
                        "MACHINE CHECK", "SIMD FLOATING-POINT EXCEPTION"};

void idt_init() {
    // Excepciones
    IDT_ENTRY(0, INTR_KERNEL);
    IDT_ENTRY(1, INTR_KERNEL);
    IDT_ENTRY(2, INTR_KERNEL);
    IDT_ENTRY(3, INTR_KERNEL);
    IDT_ENTRY(4, INTR_KERNEL);
    IDT_ENTRY(5, INTR_KERNEL);
    IDT_ENTRY(6, INTR_KERNEL);
    IDT_ENTRY(7, INTR_KERNEL);
    IDT_ENTRY(8, INTR_KERNEL);
    IDT_ENTRY(9, INTR_KERNEL);
    IDT_ENTRY(10, INTR_KERNEL);
    IDT_ENTRY(11, INTR_KERNEL);
    IDT_ENTRY(12, INTR_KERNEL);
    IDT_ENTRY(13, INTR_KERNEL);
    IDT_ENTRY(14, INTR_KERNEL);
    IDT_ENTRY(16, INTR_KERNEL);
    IDT_ENTRY(17, INTR_KERNEL);
    IDT_ENTRY(18, INTR_KERNEL);
    IDT_ENTRY(19, INTR_KERNEL);
    IDT_ENTRY(32, INTR_KERNEL);
    IDT_ENTRY(33, INTR_KERNEL);
    IDT_ENTRY(137, INTR_USER);
    IDT_ENTRY(138, INTR_USER);
    IDT_ENTRY(139, INTR_USER);
}


void imprimirExcepcion(int num) {
    //Imprime el mensaje de la excepcion dado en el vector de mensajes
    int tam_print = sizeof(msjExcepcion[num]);
    print_dec(num, 2,  27 , 2, C_FG_WHITE|C_BG_RED);
    print(msjExcepcion[num], 40 - tam_print, 2, C_FG_WHITE|C_BG_RED);
} 