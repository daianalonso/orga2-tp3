section .text
global task
task:
  jmp task_code
  times 50 nop
  ; Invalid instruction.
  times 300 ud2

task_code:
  mov eax, 0x90909090    ;parcheo mi codigo con nops
  mov [0x8000000], eax  ;(como un hdp)
  mov [0x8000004],eax
  xor edi, edi
.ciclo_columnas:
  xor esi, esi
  .ciclo_filas:
    .portal_y:
      mov eax, edi
      inc eax      ; x = col + 1
      mov ebx, esi ; y = fila
      mov ecx, 0  ; !cross
      mov edx, 0  ; !with_morty
      int 137
      cmp eax, 1
      jne .portal_y
    call copiar_codigo
    inc esi
    cmp esi, 40
    jl .ciclo_filas
  inc edi
  cmp edi, 80
  jl .ciclo_columnas

jmp task_code
times 90 nop

; Must preserve esi and edi.
copiar_codigo:
  mov ecx, 0x1000
  mov ebx, 0x08002000
  mov edx, 0x08000000
  .loop:
    mov eax, [edx + ecx - 0x4]
    mov [ebx + ecx - 0x4], eax
    sub ecx, 0x4
    cmp ecx, 0
    jg .loop
  ret
