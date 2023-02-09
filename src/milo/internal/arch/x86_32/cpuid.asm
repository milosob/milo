; asmsyntax=nasm

bits 32

%include "abi.asm"

global milo_internal_arch_x86_32_cpuid

; BEG arch_x86_32_cpuid

section .text
milo_internal_arch_x86_32_cpuid:
; Args:
;   0 -> ptr to array of 4 double-words
;   1 -> leaf
;   2 -> sub-leaf
    push    edi
    push    ebx
    mov     edi,    [esp + 12]
    mov     eax,    [esp + 16]
    mov     ecx,    [esp + 20]
    cpuid
    mov     [edi + 0],   eax
    mov     [edi + 4],   ebx
    mov     [edi + 8],   ecx
    mov     [edi + 12],  edx
    pop     ebx
    pop     edi
    ret

; END arch_x86_32_cpuid
