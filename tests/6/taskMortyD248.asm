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

.wait:
  cmp BYTE [0x08001FFF], 0x1
jne .wait

xchg bx, bx

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
  jmp copiar_codigo
