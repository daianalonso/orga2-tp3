/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"
#include "sched.h"
#include "idt.h"

infoDebug info_debug;
infoPantalla info_pantalla;

void print_students_data(){
    print("Daiana Alonso LU:682/15", (VIDEO_COLS/2)-12, VIDEO_FILS-2, C_FG_YELLOW|C_BG_BLACK);
    print("Tomas Caballero LU:628/15", (VIDEO_COLS/2)-13, VIDEO_FILS-1, C_FG_YELLOW|C_BG_BLACK);
}

void printScanCode(uint8_t scanCode){
    if(!(scanCode & 0x80)){
        if((1 < scanCode) && (scanCode < 12)){
            if(scanCode == 11) print_dec(0, 1, VIDEO_COLS-1, 0, C_FG_YELLOW|C_BG_BLACK);     
            print_dec(scanCode-1 , 1, VIDEO_COLS-1, 0, C_FG_YELLOW|C_BG_BLACK);          
        }else{
            print_dec(0, 1, VIDEO_COLS-1, 0, C_FG_BLACK|C_BG_BLACK);
        } 
    }
}

void print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (uint8_t) text[i];
        p[y][x].a = (uint8_t) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    uint32_t i;
    uint8_t letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        uint32_t resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    uint8_t hexa[8];
    uint8_t letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void screen_drawBox(uint32_t fInit,
                    uint32_t cInit,
                    uint32_t fSize,
                    uint32_t cSize,
                    uint8_t character,
                    uint8_t attr ) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    uint32_t f;
    uint32_t c;
    for (f = fInit; f < fInit+fSize; f++) {
    for (c = cInit; c < cInit+cSize; c++) {
          p[f][c].c = character;
          p[f][c].a = attr;
    }}
}


void imprimir_pantalla_debug()
{
    // Guardo la pantalla actual 
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    ca (*buffer_pantalla)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) &info_pantalla.buffer;

    for (int i = 1; i < 45; ++i)
    {
        for (int j = 1; j < 79; ++j)
        {
            buffer_pantalla[i][j].c = p[i][j].c;
            buffer_pantalla[i][j].a = p[i][j].a;
        }
    }

    // Pinto la pantalla de debug
    for (int i = 1; i < 42; ++i)
    {
        for (int j = 25; j < 55; ++j)
        {
            print( "-", j, i, C_FG_BLACK | C_BG_BLACK );
        }
    }

    for (int i = 4; i < 40; ++i)
    {
        for (int j = 26; j < 54; ++j)
        {
            print( "-", j, i, C_FG_LIGHT_GREY | C_BG_LIGHT_GREY );
        }
    }

    for (int i = 26; i < 54; ++i)
    {
        print( "-", i, 2, C_FG_RED | C_BG_RED );
    }

    const char* registros[16] = {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp",
                                 "eip", "cs", "ds", "es", "fs", "gs", "ss", "eflags"};

    // Imprimo  columna izquierda
    int fila = 6;

    for (int i = 0; i < 16; ++i)
    {   
        if(i == 15) fila +=2;
        print( registros[i], 27, fila, C_FG_BLACK | C_BG_LIGHT_GREY );
        fila += 2;
    }

    //Para que no imprima divide error por equivocacion
    if(info_debug.cr3 != 0) imprimirExcepcion(info_debug.excepcion);

    print_hex( info_debug.eax, 8, 31, 6, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.ebx, 8, 31, 8, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.ecx, 8, 31, 10, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.edx, 8, 31, 12, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.esi, 8, 31, 14, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.edi, 8, 31, 16, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.ebp, 8, 31, 18, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.esp, 8, 31, 20, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.eip, 8, 31, 22, C_FG_WHITE | C_BG_LIGHT_GREY );

    print_hex( info_debug.cs, 4, 31, 24, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.ds, 4, 31, 26, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.es, 4, 31, 28, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.fs, 4, 31, 30, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.gs, 4, 31, 32, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.ss, 4, 31, 34, C_FG_WHITE | C_BG_LIGHT_GREY );

    print_hex( info_debug.eflags, 8, 34, 38, C_FG_WHITE | C_BG_LIGHT_GREY );


    //Imprimo lo de la columna derecha 
    print( "cr0", 41, 8, C_FG_BLACK | C_BG_LIGHT_GREY );
    print( "cr2", 41, 10, C_FG_BLACK | C_BG_LIGHT_GREY );
    print( "cr3", 41, 12, C_FG_BLACK | C_BG_LIGHT_GREY );
    print( "cr4", 41, 14, C_FG_BLACK | C_BG_LIGHT_GREY );
    print( "stack", 41, 27, C_FG_BLACK | C_BG_LIGHT_GREY );

    print_hex( info_debug.cr0, 8, 45, 8, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.cr2, 8, 45, 10, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.cr3, 8, 45, 12, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.cr4, 8, 45, 14, C_FG_WHITE | C_BG_LIGHT_GREY );

    print_hex( info_debug.stack1, 8, 41, 29, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.stack2, 8, 41, 30, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.stack3, 8, 41, 31, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.stack4, 8, 41, 32, C_FG_WHITE | C_BG_LIGHT_GREY );
    print_hex( info_debug.stack5, 8, 41, 33, C_FG_WHITE | C_BG_LIGHT_GREY );
}



void restaurar_pantalla()
{
    // Vuelvo a pintar la pantalla
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    ca (*buffer_pantalla)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) &info_pantalla.buffer;

    for (int i = 1; i < 45; ++i)
    {
        for (int j = 1; j < 79; ++j)
        {
            p[i][j].c = buffer_pantalla[i][j].c;
            p[i][j].a = buffer_pantalla[i][j].a;
        }
    }
}
