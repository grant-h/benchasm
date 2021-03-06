BITS 32
section .text

%macro cglobal 1
  %ifdef LEADING_UNDERSCORES
    global  _%1
    %define %1 _%1
    %1:
  %elif
    global  %1
    %1:
  %endif
%endmacro 

; eax, ecx, edx are caller-saved, rest are callee saved
cglobal memcpy_mov
  mov eax, [esp+0x4]  ; dst
  mov edx, [esp+0x8]  ; src
  mov ecx, [esp+0xc] ; size

  push ebx

MOVLOOP:
    mov bl, byte [edx]
    mov byte [eax], bl
    inc eax
    inc edx

    dec ecx
    jne MOVLOOP
  pop ebx

  mov eax, [esp+0x4]  ; return dst
  ret

cglobal memcpy_rep_movsb
  push ebp
  mov ebp, esp

  push esi
  push edi

  mov edi, [ebp+0x8]  ; dst
  mov esi, [ebp+0xc]  ; src
  mov ecx, [ebp+0x10] ; size

  rep movsb

  pop edi
  pop esi

  mov eax, [ebp+0x8]  ; return dst

  leave
  ret

cglobal memcpy_rep_movsd
  push ebp
  mov ebp, esp

  push esi
  push edi

  mov edi, [ebp+0x8]  ; dst
  mov esi, [ebp+0xc]  ; src
  mov ecx, [ebp+0x10] ; size

  shr ecx, 2
  rep movsd

  pop edi
  pop esi

  mov eax, [ebp+0x8]  ; return dst

  leave
  ret

cglobal memcpy_sse_movntdq
  push ebp
  mov ebp, esp

  push esi
  push edi

  mov edi, [ebp+0x8]  ; dst
  mov esi, [ebp+0xc]  ; src
  mov ecx, [ebp+0x10] ; size

  shr ecx, 4

L1:
      movdqa xmm0, [esi]   ; load aligned double-qword from memory
      movntdq [edi], xmm0  ; store aligned double-qword to memory (non-temporal hint)
      add esi, 16
      add edi, 16
      prefetcht0 [esi+64]
      dec ecx
      jne L1

  pop edi
  pop esi

  mov eax, [ebp+0x8]  ; return dst

  leave
  ret

cglobal memcpy_sse_movdqa
  push ebp
  mov ebp, esp

  push esi
  push edi

  mov edi, [ebp+0x8]  ; dst
  mov esi, [ebp+0xc]  ; src
  mov ecx, [ebp+0x10] ; size

  shr ecx, 4

L2:
      movdqa xmm0, [esi]   ; load aligned double-qword from memory
      movdqa [edi], xmm0   ; store aligned double-qword to memory
      add esi, 16
      add edi, 16
      dec ecx
      jne L2

  pop edi
  pop esi

  mov eax, [ebp+0x8]  ; return dst

  leave
  ret

cglobal memcpy_avx2_vmovdqa
  push ebp
  mov ebp, esp

  push esi
  push edi

  mov edi, [ebp+0x8]  ; dst
  mov esi, [ebp+0xc]  ; src
  mov ecx, [ebp+0x10] ; size

  shr ecx, 5

L3:
      vmovdqa ymm0, [esi]   ; load aligned double-qword from memory
      vmovntdq [edi], ymm0   ; store aligned double-qword to memory
      add esi, 32
      add edi, 32
      dec ecx
      jne L3

  pop edi
  pop esi

  mov eax, [ebp+0x8]  ; return dst

  leave
  ret
