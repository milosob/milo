; asmsyntax=nasm

bits 64

%include "abi.asm"

global milo_internal_arch_x86_64_cpuid

; BEG arch_x86_64_cpuid

section .text
milo_internal_arch_x86_64_cpuid:
; Args:
;   0 -> ptr to array of 4 double-words
;   1 -> leaf
;   2 -> sub-leaf
    xchg    rbx,    r10
    mov     r9,     abi_arg_0
    mov     rax,    abi_arg_1
    mov     rcx,    abi_arg_2
    cpuid
    mov     [r9 + 0],   eax
    mov     [r9 + 4],   ebx
    mov     [r9 + 8],   ecx
    mov     [r9 + 12],  edx
    xchg    rbx,        r10
    ret

; BEG arch_x86_64_cpuid
