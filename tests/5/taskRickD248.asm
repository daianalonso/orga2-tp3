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
  add eax, 1 ; x =  morty.x + 1
  ; ebx: morty.y
  mov ecx, 1 ; cross
  mov edx, 0 ; !with_morty
  int 137
  cmp eax, 1
  jne .next_to_morty

  .loop:
  xor edi, edi
  .loop_filas:
    .mover_vertical:
    mov eax, 0
    mov ebx, -1
    mov ecx, 1
    mov edx, 1
    int 137
    cmp eax, 1
    jne .mover_vertical
    inc edi
    cmp edi, 40
  jl .loop_filas


  .mover_horizontal:
    mov eax, -2
    mov ebx, 0
    mov ecx, 1   ; cross
    mov edx, 1   ; with_morty
    int 137
    cmp eax, 1
    jne .mover_horizontal
  jmp .loop
