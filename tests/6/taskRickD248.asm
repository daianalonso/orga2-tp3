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
  .next_to_morty:
  int 139
  ; Move next to morty.
  ; x = morty.x
  add ebx, 1 ; y = morty.y+1
  mov ecx, 1 ; cross
  mov edx, 0 ; !with_morty
  int 137
  cmp eax, 1
  jne .next_to_morty

  int 139
  ; Map morty's page
  .map_morty
  mov ecx, 0 ; !cross
  mov edx, 1 ; with_morty
  int 137
  cmp eax, 1
  jne .map_morty

  ; Morty can now start copying code.
  mov BYTE [0x08003FFF], 0x1

  mov esi, 2
  mov edi, 0
  .loop:
  .loop_filas:
    .portal:
    mov eax, esi
    mov ebx, edi
    mov ecx, 0 ; !cross
    mov edx, 1 ; with_morty
    int 137
    cmp eax, 1
    jne .portal
    inc edi
    cmp edi, 40
  jl .loop_filas
  mov edi, 0
  inc esi
  jmp .loop
