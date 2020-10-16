section .text
global task
task:
  jmp task_code
  times 200 nop
  ; Fill everything with nops.
.loop:
  ; Claim ourselves as universe D248
  mov ax, 0xD248
  int 138
  jmp .loop
  times 20 nop
  jmp .loop

task_code:
  mov eax, 0x90909090    ;parcheo mi codigo con nops
  mov [0x8000000], eax  ;(como un hdp)
  mov [0x8000004],eax
  xor edi, edi

.ciclo_columnas:
  xor esi, esi
  .ciclo_filas:
    .portal_y:
      mov eax, 0  ; x = 0
      mov ebx, -1 ; y = -1
      mov ecx, 1  ; cross
      mov edx, 0  ; !with_morty
      int 137
      cmp eax, 1
      jne .portal_y
    inc esi
    cmp esi, 40
    jl .ciclo_filas
    .portal_x:
      mov eax, 1 ; x = 1
      mov ebx, 0 ; y = 0
      mov ecx, 1 ; cross
      mov edx, 0 ; !with_morty
      int 137
      cmp eax, 1
      jne .portal_x
  inc edi
  cmp edi, 80
  jl .ciclo_columnas

jmp task_code
times 90 nop
