; asmsyntax=nasm

bits 64

%include "abi.asm"

global milo_primitive_detail_cipher_chacha_impl_hw_x86_64_ssse_3_ietf_transform
global milo_primitive_detail_cipher_chacha_impl_hw_x86_64_ssse_3_ietf_generate

global milo_primitive_detail_cipher_chacha_impl_hw_x86_64_avx_2_ietf_transform
global milo_primitive_detail_cipher_chacha_impl_hw_x86_64_avx_2_ietf_generate

; BEG cipher_chacha_impl_hw_x86_64_ssse_3

; Constants.

; Stack.
%xdefine    stack_size              (0)
%xdefine    s_state_temp(i)         [rsp + stack_size + 16 * (i)]
%xdefine    stack_size              (stack_size + 16 * 6)
%xdefine    s_state_base(i)         [rsp + stack_size + 16 * (i)]
%xdefine    stack_size              (stack_size + 16 * 16)

; Variables.
%define     g_rounds                abi_arg_0
%define     g_rounds_cnt            rbx
%define     g_blocks                abi_arg_1
%define     g_blocks_cnt            rax
%define     g_state                 abi_arg_2
%define     m_state(i)              [g_state + (i)]
%define     g_dst                   abi_arg_3
%define     m_dst(i)                [g_dst + (i)]

%ifdef MILO_INTERNAL_ABI_UNIX
%define     g_src                   abi_arg_4
%endif

%ifdef MILO_INTERNAL_ABI_WIND
%define     g_src                   r10
%endif

%define     m_src(i)                [g_src + (i)]
%define     x_state_eval(i)         xmm %+ %eval((i))

%macro  chacha_ssse_3_rounds 18
; Args:
;   1-16    -> 0 based indexes of state in column top-down order
;   17      -> even round rotations by 16 or 8
;   18      -> odd  round rotations by 12 or 7
    paddd   x_state_eval(%1),   x_state_eval(%5)
    paddd   x_state_eval(%2),   x_state_eval(%6)
    pxor    x_state_eval(%13),  x_state_eval(%1)
    pxor    x_state_eval(%14),  x_state_eval(%2)
    paddd   x_state_eval(%3),   x_state_eval(%7)
    paddd   x_state_eval(%4),   x_state_eval(%8)
    pshufb  x_state_eval(%13),  [rel .constant_state_shuffle_rol_%17]
    pshufb  x_state_eval(%14),  [rel .constant_state_shuffle_rol_%17]
    pxor    x_state_eval(%15),  x_state_eval(%3)
    pxor    x_state_eval(%16),  x_state_eval(%4)
    movdqa  s_state_temp(0),    x_state_eval(%13)
    movdqa  s_state_temp(1),    x_state_eval(%14)
    pshufb  x_state_eval(%15),  [rel .constant_state_shuffle_rol_%17]
    pshufb  x_state_eval(%16),  [rel .constant_state_shuffle_rol_%17]

    paddd   x_state_eval(%9),   x_state_eval(%13)
    paddd   x_state_eval(%10),  x_state_eval(%14)
    pxor    x_state_eval(%5),   x_state_eval(%9)
    pxor    x_state_eval(%6),   x_state_eval(%10)
    paddd   x_state_eval(%11),  x_state_eval(%15)
    paddd   x_state_eval(%12),  x_state_eval(%16)
    movdqa  x_state_eval(%13),  x_state_eval(%5)
    movdqa  x_state_eval(%14),  x_state_eval(%6)
    pslld   x_state_eval(%5),   %18
    psrld   x_state_eval(%13),  32 - %18
    pslld   x_state_eval(%6),   %18
    psrld   x_state_eval(%14),  32 - %18
    pxor    x_state_eval(%7),   x_state_eval(%11)
    pxor    x_state_eval(%8),   x_state_eval(%12)
    pxor    x_state_eval(%5),   x_state_eval(%13)
    pxor    x_state_eval(%6),   x_state_eval(%14)
    movdqa  x_state_eval(%13),  x_state_eval(%7)
    movdqa  x_state_eval(%14),  x_state_eval(%8)
    pslld   x_state_eval(%7),   %18
    psrld   x_state_eval(%13),  32 - %18
    pslld   x_state_eval(%8),   %18
    psrld   x_state_eval(%14),  32 - %18
    pxor    x_state_eval(%7),   x_state_eval(%13)
    pxor    x_state_eval(%8),   x_state_eval(%14)
    movdqa  x_state_eval(%13),  s_state_temp(0)
    movdqa  x_state_eval(%14),  s_state_temp(1)
%endmacro

%macro chacha_ssse_3_transform_args 0
; On unix source pointer is already in abi defined register.
; On wind it's on stack. Need to load it, as it's 5th parameter.

%ifdef MILO_INTERNAL_ABI_WIND
; Frame is rbx, rbp, return address, shadow space, 5th param.
    mov     g_src,  [rbp + 24 + 32]
%endif

%endmacro

%macro chacha_ssse_3_transform_core 12
; Args:
;   1       -> unused
;   2       -> offset
;   2-6     -> consequential columns
;   7-8     -> scratch registers
;   9-12    -> scratch registers for memory loads
    movdqa      x_state_eval(%7),       x_state_eval(%3)
    movdqa      x_state_eval(%8),       x_state_eval(%5)
    movdqu      x_state_eval(%9),       m_src(16 * (%2 + 0))
    movdqu      x_state_eval(%10),      m_src(16 * (%2 + 4))
    punpckhdq   x_state_eval(%3),       x_state_eval(%4)  ; A1 A0 B1 B0
    punpckldq   x_state_eval(%7),       x_state_eval(%4)  ; C1 C0 D1 D0
    movdqu      x_state_eval(%11),      m_src(16 * (%2 + 8))
    movdqu      x_state_eval(%12),      m_src(16 * (%2 + 12))
    punpckhdq   x_state_eval(%5),       x_state_eval(%6)  ; A3 A2 B3 B2
    punpckldq   x_state_eval(%8),       x_state_eval(%6)  ; C3 C2 D3 D2

    movdqa      x_state_eval(%4),       x_state_eval(%3)
    movdqa      x_state_eval(%6),       x_state_eval(%7)
    punpckhqdq  x_state_eval(%3),       x_state_eval(%5)  ; A3 A2 A1 A0
    punpcklqdq  x_state_eval(%4),       x_state_eval(%5)  ; B3 B2 B1 B0
    pxor        x_state_eval(%3),       x_state_eval(%9)
    pxor        x_state_eval(%4),       x_state_eval(%10)
    movdqu      m_dst(16 * (%2 + 0)),   x_state_eval(%3)
    movdqu      m_dst(16 * (%2 + 4)),   x_state_eval(%4)
    punpckhqdq  x_state_eval(%7),       x_state_eval(%8)  ; C3 C2 C1 C0
    punpcklqdq  x_state_eval(%6),       x_state_eval(%8)  ; D3 D2 D1 D0
    pxor        x_state_eval(%7),       x_state_eval(%11)
    pxor        x_state_eval(%6),       x_state_eval(%12)
    movdqu      m_dst(16 * (%2 + 8)),   x_state_eval(%7)
    movdqu      m_dst(16 * (%2 + 12)),  x_state_eval(%6)
%endmacro

%macro chacha_ssse_3_transform_body 0
; States 15 and 12 are already in memory.
; Use them as scratch registers. This avoids 2 moves.
; Need 4 more registers, flush 10, 11, 13 and 14 to the stack.
    paddd   x_state_eval(10),   s_state_base(10)
    paddd   x_state_eval(11),   s_state_base(11)
    paddd   x_state_eval(13),   s_state_base(13)
    paddd   x_state_eval(14),   s_state_base(14)
    movdqa  s_state_temp(2),    x_state_eval(10)
    movdqa  s_state_temp(3),    x_state_eval(11)
    movdqa  s_state_temp(4),    x_state_eval(13)
    movdqa  s_state_temp(5),    x_state_eval(14)

    paddd   x_state_eval(0),    s_state_base(0)
    paddd   x_state_eval(1),    s_state_base(1)
    paddd   x_state_eval(2),    s_state_base(2)
    paddd   x_state_eval(3),    s_state_base(3)
    paddd   x_state_eval(4),    s_state_base(4)
    paddd   x_state_eval(5),    s_state_base(5)
    paddd   x_state_eval(6),    s_state_base(6)
    paddd   x_state_eval(7),    s_state_base(7)

    chacha_ssse_3_transform_core    _ , 0, 0, 1, 2, 3, 10, 11, 12, 13, 14, 15

    movdqa  x_state_eval(2),    s_state_base(12)
    movdqa  x_state_eval(0),    s_state_temp(0)   ; 15
    movdqa  x_state_eval(1),    s_state_temp(1)   ; 12

    chacha_ssse_3_transform_core    _ , 1, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15

    movdqa  x_state_eval(3),    s_state_temp(2)   ; 10
    movdqa  x_state_eval(4),    s_state_temp(3)   ; 11
    movdqa  x_state_eval(6),    s_state_temp(4)   ; 13
    movdqa  x_state_eval(7),    s_state_temp(5)   ; 14

    paddd   x_state_eval(8),    s_state_base(8)
    paddd   x_state_eval(9),    s_state_base(9)

; Need to add state_base to 12 and 15.
    paddd   x_state_eval(1),    x_state_eval(2)
    paddd   x_state_eval(0),    s_state_base(15)
; Advance state.
    paddd   x_state_eval(2),    [rel .constant_state_add_advance]

    chacha_ssse_3_transform_core    _ , 2, 8, 9, 3, 4, 10, 11, 12, 13, 14, 15

    movdqa  s_state_base(12),   x_state_eval(2)

    chacha_ssse_3_transform_core    _ , 3, 1, 6, 7, 0, 10, 11, 12, 13, 14, 15
; Advance pointers.
    add     g_dst,  256
    add     g_src,  256
%endmacro

%macro chacha_ssse_3_generate_args 0
; All parameters are mapped to registers by abi.
%endmacro

%macro chacha_ssse_3_generate_core 8
; Args:
;   1       -> unused
;   2       -> offset
;   2-6     -> consequential columns
;   7-8     -> scratch registers
    movdqa      x_state_eval(%7),       x_state_eval(%3)
    movdqa      x_state_eval(%8),       x_state_eval(%5)
    punpckhdq   x_state_eval(%3),       x_state_eval(%4)  ; A1 A0 B1 B0
    punpckldq   x_state_eval(%7),       x_state_eval(%4)  ; C1 C0 D1 D0
    punpckhdq   x_state_eval(%5),       x_state_eval(%6)  ; A3 A2 B3 B2
    punpckldq   x_state_eval(%8),       x_state_eval(%6)  ; C3 C2 D3 D2

    movdqa      x_state_eval(%4),       x_state_eval(%3)
    movdqa      x_state_eval(%6),       x_state_eval(%7)
    punpckhqdq  x_state_eval(%3),       x_state_eval(%5)  ; A3 A2 A1 A0
    punpcklqdq  x_state_eval(%4),       x_state_eval(%5)  ; B3 B2 B1 B0
    movdqu      m_dst(16 * (%2 + 0)),   x_state_eval(%3)
    movdqu      m_dst(16 * (%2 + 4)),   x_state_eval(%4)
    punpckhqdq  x_state_eval(%7),       x_state_eval(%8)  ; C3 C2 C1 C0
    punpcklqdq  x_state_eval(%6),       x_state_eval(%8)  ; D3 D2 D1 D0
    movdqu      m_dst(16 * (%2 + 8)),   x_state_eval(%7)
    movdqu      m_dst(16 * (%2 + 12)),  x_state_eval(%6)
%endmacro

%macro chacha_ssse_3_generate_body 0
; States 15 and 12 are already in memory.
; Use them as scratch registers. This avoids 2 moves.
    paddd   x_state_eval(0),    s_state_base(0)
    paddd   x_state_eval(1),    s_state_base(1)
    paddd   x_state_eval(2),    s_state_base(2)
    paddd   x_state_eval(3),    s_state_base(3)
    paddd   x_state_eval(4),    s_state_base(4)
    paddd   x_state_eval(5),    s_state_base(5)
    paddd   x_state_eval(6),    s_state_base(6)
    paddd   x_state_eval(7),    s_state_base(7)
    paddd   x_state_eval(8),    s_state_base(8)
    paddd   x_state_eval(9),    s_state_base(9)
    paddd   x_state_eval(10),   s_state_base(10)
    paddd   x_state_eval(11),   s_state_base(11)
    paddd   x_state_eval(13),   s_state_base(13)
    paddd   x_state_eval(14),   s_state_base(14)

    chacha_ssse_3_generate_core     _, 0, 0, 1, 2, 3, 12, 15

    movdqa  x_state_eval(2),    s_state_base(12)
    movdqa  x_state_eval(0),    s_state_temp(0)   ; 15
    movdqa  x_state_eval(1),    s_state_temp(1)   ; 12

    chacha_ssse_3_generate_core     _, 1, 4, 5, 6, 7, 12, 15

; Need to add state_base to 12 and 15.
    paddd   x_state_eval(1),    x_state_eval(2)
    paddd   x_state_eval(0),    s_state_base(15)
; Advance state.
    paddd   x_state_eval(2),    [rel .constant_state_add_advance]

    chacha_ssse_3_generate_core     _, 2, 8, 9, 10, 11, 12, 15

    movdqa  s_state_base(12),   x_state_eval(2)

    chacha_ssse_3_generate_core     _, 3, 1, 13, 14, 0, 12, 15
; Advance pointers.
    add     g_dst,  256
%endmacro

%macro chacha_ssse_3_body 1

section .rodata
    align 16
.constant_state_add_init:
    dd      0x03, 0x02, 0x01, 0x00
.constant_state_add_advance:
    dd      0x04, 0x04, 0x04, 0x04
.constant_state_shuffle_rol_16:
    db      0x02, 0x03, 0x00, 0x01
    db      0x06, 0x07, 0x04, 0x05
    db      0x0a, 0x0b, 0x08, 0x09
    db      0x0e, 0x0f, 0x0c, 0x0d
.constant_state_shuffle_rol_8:
    db      0x03, 0x00, 0x01, 0x02
    db      0x07, 0x04, 0x05, 0x06
    db      0x0b, 0x08, 0x09, 0x0a
    db      0x0f, 0x0c, 0x0d, 0x0e
section .text

.start:

    push    rbp
    push    rbx
    mov     rbp,    rsp

%ifdef MILO_INTERNAL_ABI_UNIX
    sub     rsp,    stack_size + 8
%endif

%ifdef MILO_INTERNAL_ABI_WIND
    sub     rsp,    stack_size + 8 + 16 * 10
    movdqa  [rbp - 8 - 16 * 1],     xmm6
    movdqa  [rbp - 8 - 16 * 2],     xmm7
    movdqa  [rbp - 8 - 16 * 3],     xmm8
    movdqa  [rbp - 8 - 16 * 4],     xmm9
    movdqa  [rbp - 8 - 16 * 5],     xmm10
    movdqa  [rbp - 8 - 16 * 6],     xmm11
    movdqa  [rbp - 8 - 16 * 7],     xmm12
    movdqa  [rbp - 8 - 16 * 8],     xmm13
    movdqa  [rbp - 8 - 16 * 9],     xmm14
    movdqa  [rbp - 8 - 16 * 10],    xmm15
%endif

    chacha_ssse_3_%{1}_args

    test    g_blocks,       g_blocks
    mov     g_blocks_cnt,   g_blocks
    jz      .blocks_loop_end

.blocks_init:

; State representation, columns are registers:
;   A0 A1 A2 A3 | A4 A5 A6 A7 | A8 A9 A10 A11 | A12 A13 A14 A15
;   B0 B1 B2 B3 | B4 B5 B6 B7 | B8 B9 B10 B11 | B12 B13 B14 B15
;   C0 C1 C2 C3 | C4 C5 C6 C7 | C8 C9 C10 C11 | C12 C13 C14 C15
;   D0 D1 D2 D3 | D4 D5 D6 D7 | D8 D9 D10 D11 | D12 D13 D14 D15
;
; This representation allows rows to be processed in parallel.

    movdqu  x_state_eval(12),    m_state(16 * 0)
    movdqu  x_state_eval(13),    m_state(16 * 1)
    movdqu  x_state_eval(14),    m_state(16 * 2)
    movdqu  x_state_eval(15),    m_state(16 * 3)

    pshufd  x_state_eval(0),    x_state_eval(12),   00000000b
    pshufd  x_state_eval(1),    x_state_eval(12),   01010101b
    pshufd  x_state_eval(2),    x_state_eval(12),   10101010b
    pshufd  x_state_eval(3),    x_state_eval(12),   11111111b
    movdqa  s_state_base(0),    x_state_eval(0)
    movdqa  s_state_base(1),    x_state_eval(1)
    movdqa  s_state_base(2),    x_state_eval(2)
    movdqa  s_state_base(3),    x_state_eval(3)

    pshufd  x_state_eval(4),    x_state_eval(13),   00000000b
    pshufd  x_state_eval(5),    x_state_eval(13),   01010101b
    pshufd  x_state_eval(6),    x_state_eval(13),   10101010b
    pshufd  x_state_eval(7),    x_state_eval(13),   11111111b
    movdqa  s_state_base(4),    x_state_eval(4)
    movdqa  s_state_base(5),    x_state_eval(5)
    movdqa  s_state_base(6),    x_state_eval(6)
    movdqa  s_state_base(7),    x_state_eval(7)

    pshufd  x_state_eval(8),    x_state_eval(14),   00000000b
    pshufd  x_state_eval(9),    x_state_eval(14),   01010101b
    pshufd  x_state_eval(10),   x_state_eval(14),   10101010b
    pshufd  x_state_eval(11),   x_state_eval(14),   11111111b
    movdqa  s_state_base(8),    x_state_eval(8)
    movdqa  s_state_base(9),    x_state_eval(9)
    movdqa  s_state_base(10),   x_state_eval(10)
    movdqa  s_state_base(11),   x_state_eval(11)

    pshufd  x_state_eval(12),   x_state_eval(15),   00000000b
    pshufd  x_state_eval(13),   x_state_eval(15),   01010101b
    paddd   x_state_eval(12),   [rel .constant_state_add_init]
    pshufd  x_state_eval(14),   x_state_eval(15),   10101010b
    pshufd  x_state_eval(15),   x_state_eval(15),   11111111b

    movdqa  s_state_base(12),   x_state_eval(12)
    movdqa  s_state_base(13),   x_state_eval(13)
    movdqa  s_state_base(14),   x_state_eval(14)
    movdqa  s_state_base(15),   x_state_eval(15)

; Avoid loading state from stack.
    jmp     .blocks_loop_first

.blocks_loop:

    movdqa  x_state_eval(0),    s_state_base(0)
    movdqa  x_state_eval(1),    s_state_base(1)
    movdqa  x_state_eval(2),    s_state_base(2)
    movdqa  x_state_eval(3),    s_state_base(3)
    movdqa  x_state_eval(4),    s_state_base(4)
    movdqa  x_state_eval(5),    s_state_base(5)
    movdqa  x_state_eval(6),    s_state_base(6)
    movdqa  x_state_eval(7),    s_state_base(7)
    movdqa  x_state_eval(8),    s_state_base(8)
    movdqa  x_state_eval(9),    s_state_base(9)
    movdqa  x_state_eval(10),   s_state_base(10)
    movdqa  x_state_eval(11),   s_state_base(11)
    movdqa  x_state_eval(12),   s_state_base(12)
    movdqa  x_state_eval(13),   s_state_base(13)
    movdqa  x_state_eval(14),   s_state_base(14)
    movdqa  x_state_eval(15),   s_state_base(15)

.blocks_loop_first:

        mov     g_rounds_cnt,       g_rounds

.rounds_loop:

    chacha_ssse_3_rounds    \
         0,  1,  2,  3,     \
         4,  5,  6,  7,     \
         8,  9, 10, 11,     \
        12, 13, 14, 15,     \
        16, 12

    chacha_ssse_3_rounds    \
         0,  1,  2,  3,     \
         4,  5,  6,  7,     \
         8,  9, 10, 11,     \
        12, 13, 14, 15,     \
         8,  7

    chacha_ssse_3_rounds    \
         0,  1,  2,  3,     \
         5,  6,  7,  4,     \
        10, 11,  8,  9,     \
        15, 12, 13, 14,     \
        16, 12

    chacha_ssse_3_rounds    \
         0,  1,  2,  3,     \
         5,  6,  7,  4,     \
        10, 11,  8,  9,     \
        15, 12, 13, 14,     \
         8,  7

    sub     g_rounds_cnt,   1
    jnz     .rounds_loop

.rounds_loop_end:

    chacha_ssse_3_%{1}_body

    sub     g_blocks_cnt,   1
    jnz     .blocks_loop

.blocks_loop_end:

%ifdef MILO_INTERNAL_ABI_WIND
    movdqa  xmm6,   [rbp - 8 - 16 * 1]
    movdqa  xmm7,   [rbp - 8 - 16 * 2]
    movdqa  xmm8,   [rbp - 8 - 16 * 3]
    movdqa  xmm9,   [rbp - 8 - 16 * 4]
    movdqa  xmm10,  [rbp - 8 - 16 * 5]
    movdqa  xmm11,  [rbp - 8 - 16 * 6]
    movdqa  xmm12,  [rbp - 8 - 16 * 7]
    movdqa  xmm13,  [rbp - 8 - 16 * 8]
    movdqa  xmm14,  [rbp - 8 - 16 * 9]
    movdqa  xmm15,  [rbp - 8 - 16 * 10]
%endif

    mov     rsp,    rbp
    pop     rbx
    pop     rbp
; Return blocks done.
; Parallelization factor is 4.
    mov     rax,    g_blocks
    shl     rax,    2
    ret
%endmacro

    section .text
milo_primitive_detail_cipher_chacha_impl_hw_x86_64_ssse_3_ietf_transform:
; Args:
;   0 -> rounds
;   1 -> blocks
;   2 -> state_ptr
;   3 -> dst_ptr
;   4 -> src_ptr
; Return:
;   rax -> blocks
    chacha_ssse_3_body  transform

milo_primitive_detail_cipher_chacha_impl_hw_x86_64_ssse_3_ietf_generate:
; Args:
;   0 -> rounds
;   1 -> blocks
;   2 -> state_ptr
;   3 -> dst_ptr
; Return:
;   rax -> blocks
    chacha_ssse_3_body  generate

; END cipher_chacha_impl_hw_x86_64_ssse_3

; BEG cipher_chacha_impl_hw_x86_64_avx_2

; Constants.

; Stack.
%xdefine    stack_size              (0)
%xdefine    s_state_temp(i)         [rsp + stack_size + 32 * (i)]
%xdefine    stack_size              (stack_size + 32 * 6)
%xdefine    s_state_base(i)         [rsp + stack_size + 32 * (i)]
%xdefine    stack_size              (stack_size + 32 * 16)

; Variables.
%define     g_rounds                abi_arg_0
%define     g_rounds_cnt            rbx
%define     g_blocks                abi_arg_1
%define     g_blocks_cnt            rax
%define     g_state                 abi_arg_2
%define     m_state(i)              [g_state + (i)]
%define     g_dst                   abi_arg_3
%define     m_dst(i)                [g_dst + (i)]

%ifdef MILO_INTERNAL_ABI_UNIX
%define     g_src                   abi_arg_4
%endif

%ifdef MILO_INTERNAL_ABI_WIND
%define     g_src                   r10
%endif

%define     m_src(i)                [g_src + (i)]
%define     y_state_eval(i)         ymm %+ %eval((i))

%macro  chacha_avx_2_rounds 18
; Args:
;   1-16    -> 0 based indexes of state in column top-down order
;   17      -> even round rotations by 16 or 8
;   18      -> odd  round rotations by 12 or 7
    vpaddd  y_state_eval(%1),   y_state_eval(%1),   y_state_eval(%5)
    vpaddd  y_state_eval(%2),   y_state_eval(%2),   y_state_eval(%6)
    vpxor   y_state_eval(%13),  y_state_eval(%13),  y_state_eval(%1)
    vpxor   y_state_eval(%14),  y_state_eval(%14),  y_state_eval(%2)
    vpaddd  y_state_eval(%3),   y_state_eval(%3),   y_state_eval(%7)
    vpaddd  y_state_eval(%4),   y_state_eval(%4),   y_state_eval(%8)
    vpshufb y_state_eval(%13),  y_state_eval(%13),  [rel .constant_state_shuffle_rol_%17]
    vpshufb y_state_eval(%14),  y_state_eval(%14),  [rel .constant_state_shuffle_rol_%17]
    vpxor   y_state_eval(%15),  y_state_eval(%15),  y_state_eval(%3)
    vpxor   y_state_eval(%16),  y_state_eval(%16),  y_state_eval(%4)
    vmovdqa s_state_temp(0),    y_state_eval(%13)
    vmovdqa s_state_temp(1),    y_state_eval(%14)
    vpshufb y_state_eval(%15),  y_state_eval(%15),  [rel .constant_state_shuffle_rol_%17]
    vpshufb y_state_eval(%16),  y_state_eval(%16),  [rel .constant_state_shuffle_rol_%17]

    vpaddd  y_state_eval(%9),   y_state_eval(%9),   y_state_eval(%13)
    vpaddd  y_state_eval(%10),  y_state_eval(%10),  y_state_eval(%14)
    vpxor   y_state_eval(%5),   y_state_eval(%5),   y_state_eval(%9)
    vpxor   y_state_eval(%6),   y_state_eval(%6),   y_state_eval(%10)
    vpaddd  y_state_eval(%11),  y_state_eval(%11),  y_state_eval(%15)
    vpaddd  y_state_eval(%12),  y_state_eval(%12),  y_state_eval(%16)
    vmovdqa y_state_eval(%13),  y_state_eval(%5)
    vmovdqa y_state_eval(%14),  y_state_eval(%6)
    vpslld  y_state_eval(%5),   y_state_eval(%5),   %18
    vpsrld  y_state_eval(%13),  y_state_eval(%13),  32 - %18
    vpslld  y_state_eval(%6),   y_state_eval(%6),   %18
    vpsrld  y_state_eval(%14),  y_state_eval(%14),  32 - %18
    vpxor   y_state_eval(%7),   y_state_eval(%7),   y_state_eval(%11)
    vpxor   y_state_eval(%8),   y_state_eval(%8),   y_state_eval(%12)
    vpxor   y_state_eval(%5),   y_state_eval(%5),   y_state_eval(%13)
    vpxor   y_state_eval(%6),   y_state_eval(%6),   y_state_eval(%14)
    vmovdqa y_state_eval(%13),  y_state_eval(%7)
    vmovdqa y_state_eval(%14),  y_state_eval(%8)
    vpslld  y_state_eval(%7),   y_state_eval(%7),   %18
    vpsrld  y_state_eval(%13),  y_state_eval(%13),  32 - %18
    vpslld  y_state_eval(%8),   y_state_eval(%8),   %18
    vpsrld  y_state_eval(%14),  y_state_eval(%14),  32 - %18
    vpxor   y_state_eval(%7),   y_state_eval(%7),   y_state_eval(%13)
    vpxor   y_state_eval(%8),   y_state_eval(%8),   y_state_eval(%14)
    vmovdqa y_state_eval(%13),  s_state_temp(0)
    vmovdqa y_state_eval(%14),  s_state_temp(1)
%endmacro

%macro chacha_avx_2_transform_args 0
; On unix source pointer is already in abi defined register.
; On wind it's on stack. Need to load it, as it's 5th parameter.

%ifdef MILO_INTERNAL_ABI_WIND
; Frame is rbx, rbp, return address, shadow space, 5th param.
    mov     g_src,  [rbp + 24 + 32]
%endif

%endmacro

%macro chacha_avx_2_merge_32 6
; Args:
;   1-2     -> scratch registers
;   3-6     -> merged  registers
; State:
;   Args [1, 2, 3, 5] are merged.
;   Args [4, 6] are free.
; Merges 32-bit lanes.
    vpunpckhdq  y_state_eval(%1),   y_state_eval(%3),   y_state_eval(%4)    ; A1 A0 B1 B0 E1 E0 F1 F0
    vpunpckhdq  y_state_eval(%2),   y_state_eval(%5),   y_state_eval(%6)    ; A3 A2 B3 B2 E3 E2 F3 F2
    vpunpckldq  y_state_eval(%3),   y_state_eval(%3),   y_state_eval(%4)    ; C1 C0 D1 D0 G1 G0 H1 H0
    vpunpckldq  y_state_eval(%5),   y_state_eval(%5),   y_state_eval(%6)    ; C3 C2 D3 D2 G3 G2 H3 H2
%endmacro

%macro chacha_avx_2_merge_64 6
; Args:
;   1-2     -> scratch registers
;   3-6     -> merged  registers
; State:
;   Args [1, 2, 3, 5] are merged.
;   Args [4, 6] are free.
; Merges 64-bit lanes.
    vpunpckhqdq y_state_eval(%1),   y_state_eval(%3),   y_state_eval(%4)    ; A3 A2 A1 A0 E3 E2 E1 E0
    vpunpckhqdq y_state_eval(%2),   y_state_eval(%5),   y_state_eval(%6)    ; C3 C2 C1 C0 G3 G2 G1 G0
    vpunpcklqdq y_state_eval(%3),   y_state_eval(%3),   y_state_eval(%4)    ; B3 B2 B1 B0 F3 F2 F1 F0
    vpunpcklqdq y_state_eval(%5),   y_state_eval(%5),   y_state_eval(%6)    ; D3 D2 D1 D0 H3 H2 H1 H0
%endmacro

%macro chacha_avx_2_merge_128 4
; Args:
;   1-2     -> scratch registers
;   3-4     -> merged  registers
; State:
;   Args [1, 2] are merged state.
;   Args [3, 4] are free.
;   Args [2, 3] can be same.
; Merges 128-bit lanes.
    vperm2i128  y_state_eval(%1),   y_state_eval(%3),   y_state_eval(%4),   00110001b  ; A7 A6 A5 A4 A3 A2 A1 A0
    vperm2i128  y_state_eval(%2),   y_state_eval(%3),   y_state_eval(%4),   00100000b  ; E7 E6 E5 E4 E3 E2 E1 E0
%endmacro

%macro chacha_avx_2_merge_transform 4
; Args:
;   1-2     -> registers
;   3-4     -> source offsets
    vpxor       y_state_eval(%1),   y_state_eval(%1),   m_src(32 * %3)
    vpxor       y_state_eval(%2),   y_state_eval(%2),   m_src(32 * %4)
%endmacro

%macro chacha_avx_2_merge_generate 4
; Args:
;   1-2     -> registers
;   3-4     -> source 32 byte offsets
; This has no effect.
%endmacro

%macro chacha_avx_2_merge_stor 8
; Args:
;   1-4     -> registers
;   5-8     -> destination offsets
    vmovdqu     m_dst(32 * %5),     y_state_eval(%1)
    vmovdqu     m_dst(32 * %6),     y_state_eval(%2)
    vmovdqu     m_dst(32 * %7),     y_state_eval(%3)
    vmovdqu     m_dst(32 * %8),     y_state_eval(%4)
%endmacro

%macro chacha_avx_2_merge_v_1 14
; Args:
;   1       -> identifier
;   2       -> offset
;   3-4     -> scratch registers
;   5-6     -> unused
;   7-14    -> merge   registers
; There are 16 x 256 bit memory locations controlled by offset.
; Vex encoding removes necessity to prepare registers for unaligned loads.
; For now decoding and ports are biggest bottleneck.
    chacha_avx_2_merge_32        %3,  %4,  %7,  %8,  %9, %10
    chacha_avx_2_merge_64        %8, %10,  %3,  %4,  %7,  %9
    chacha_avx_2_merge_32        %4,  %9, %11, %12, %13, %14
    chacha_avx_2_merge_64       %12, %14,  %4,  %9, %11, %13
;  %9   ->  A7 A6 A5 A4 A3 A2 A1 A0
; %13   ->  E7 E6 E5 E4 E3 E2 E1 E0
    chacha_avx_2_merge_128       %9, %13,  %8, %12
    chacha_avx_2_merge_%{1}      %9, %13,           \
                                %eval(%2 + 0),      \
                                %eval(%2 + 8)
;  %8   ->  C7 C6 C5 C4 C3 C2 C1 C0
; %12   ->  G7 G6 G5 G4 G3 G2 G1 G0
    chacha_avx_2_merge_128       %8, %12, %10, %14
    chacha_avx_2_merge_%{1}      %8, %12,           \
                                %eval(%2 + 4),      \
                                %eval(%2 + 12)
    chacha_avx_2_merge_stor      %9, %13,  %8, %12, \
                                %eval(%2 + 0),      \
                                %eval(%2 + 8),      \
                                %eval(%2 + 4),      \
                                %eval(%2 + 12)
; %10   ->  B7 B6 B5 B4 B3 B2 B1 B0
; %14   ->  F7 F6 F5 F4 F3 F2 F1 F0
    chacha_avx_2_merge_128      %10, %14,  %3,  %4
    chacha_avx_2_merge_%{1}     %10, %14,           \
                                %eval(%2 + 2),      \
                                %eval(%2 + 10)
;  %3   ->  D7 D6 D5 D4 D3 D2 D1 D0
;  %4   ->  H7 H6 H5 H4 H3 H2 H1 H0
    chacha_avx_2_merge_128       %3,  %4,  %7, %11
    chacha_avx_2_merge_%{1}      %3,  %4,           \
                                %eval(%2 + 6),      \
                                %eval(%2 + 14)
    chacha_avx_2_merge_stor     %10, %14,  %3,  %4, \
                                %eval(%2 + 2),      \
                                %eval(%2 + 10),     \
                                %eval(%2 + 6),      \
                                %eval(%2 + 14)
%endmacro

%macro chacha_avx_2_merge 1
; States 15 and 12 are already in memory.
; Use them as scratch registers. This avoids 2 moves.
    vpaddd  y_state_eval(0),    y_state_eval(0),    s_state_base(0)
    vpaddd  y_state_eval(1),    y_state_eval(1),    s_state_base(1)
    vpaddd  y_state_eval(2),    y_state_eval(2),    s_state_base(2)
    vpaddd  y_state_eval(3),    y_state_eval(3),    s_state_base(3)
    vpaddd  y_state_eval(4),    y_state_eval(4),    s_state_base(4)
    vpaddd  y_state_eval(5),    y_state_eval(5),    s_state_base(5)
    vpaddd  y_state_eval(6),    y_state_eval(6),    s_state_base(6)
    vpaddd  y_state_eval(7),    y_state_eval(7),    s_state_base(7)

    chacha_avx_2_merge_v_1  \
                %1,  0,     \
        12, 15,  0,  0,     \
         0,  1,  2,  3,     \
         4,  5,  6,  7

; Preferred register use order is [2, 6, 1, 5, 0, 4].
    vmovdqa y_state_eval(2),    s_state_base(12)
    vmovdqa y_state_eval(6),    s_state_temp(1)     ; 12
    vmovdqa y_state_eval(1),    s_state_temp(0)     ; 15
    vpaddd  y_state_eval(8),    y_state_eval(8),    s_state_base(8)
    vpaddd  y_state_eval(9),    y_state_eval(9),    s_state_base(9)
    vpaddd  y_state_eval(10),   y_state_eval(10),   s_state_base(10)
    vpaddd  y_state_eval(11),   y_state_eval(11),   s_state_base(11)
    vpaddd  y_state_eval(13),   y_state_eval(13),   s_state_base(13)
    vpaddd  y_state_eval(14),   y_state_eval(14),   s_state_base(14)
    vpaddd  y_state_eval(6),    y_state_eval(6),    y_state_eval(2)
    vpaddd  y_state_eval(1),    y_state_eval(1),    s_state_base(15)
; Advance state.
    vpaddd  y_state_eval(2),    y_state_eval(2),    [rel .constant_state_add_advance]

    chacha_avx_2_merge_v_1  \
                %1,  1,     \
         5,  0,  0,  0,     \
         8,  9, 10, 11,     \
         6, 13, 14,  1

    vmovdqa s_state_base(12),   y_state_eval(2)
%endmacro

%macro chacha_avx_2_transform_body 0
    chacha_avx_2_merge  transform
; Advance pointers.
    add     g_dst,  512
    add     g_src,  512
%endmacro

%macro chacha_avx_2_generate_args 0
; All parameters are mapped to registers by abi.
%endmacro

%macro chacha_avx_2_generate_body 0
    chacha_avx_2_merge  generate
; Advance pointers.
    add     g_dst,  512
%endmacro

%macro chacha_avx_2_body 1

section .rodata
    align 16
.constant_state_add_init:
    dd      0x07, 0x06, 0x05, 0x04
    dd      0x03, 0x02, 0x01, 0x00
.constant_state_add_advance:
    dd      0x08, 0x08, 0x08, 0x08
    dd      0x08, 0x08, 0x08, 0x08
.constant_state_shuffle_rol_16:
    db      0x02, 0x03, 0x00, 0x01
    db      0x06, 0x07, 0x04, 0x05
    db      0x0a, 0x0b, 0x08, 0x09
    db      0x0e, 0x0f, 0x0c, 0x0d
    db      0x02, 0x03, 0x00, 0x01
    db      0x06, 0x07, 0x04, 0x05
    db      0x0a, 0x0b, 0x08, 0x09
    db      0x0e, 0x0f, 0x0c, 0x0d
.constant_state_shuffle_rol_8:
    db      0x03, 0x00, 0x01, 0x02
    db      0x07, 0x04, 0x05, 0x06
    db      0x0b, 0x08, 0x09, 0x0a
    db      0x0f, 0x0c, 0x0d, 0x0e
    db      0x03, 0x00, 0x01, 0x02
    db      0x07, 0x04, 0x05, 0x06
    db      0x0b, 0x08, 0x09, 0x0a
    db      0x0f, 0x0c, 0x0d, 0x0e
section .text

.start:

    push    rbp
    push    rbx
    mov     rbp,    rsp

%ifdef MILO_INTERNAL_ABI_UNIX
    sub     rsp,    stack_size + 8
%endif

%ifdef MILO_INTERNAL_ABI_WIND
    sub     rsp,    stack_size + 8 + 16 * 10
    movdqa  [rbp - 8 - 16 * 1],     xmm6
    movdqa  [rbp - 8 - 16 * 2],     xmm7
    movdqa  [rbp - 8 - 16 * 3],     xmm8
    movdqa  [rbp - 8 - 16 * 4],     xmm9
    movdqa  [rbp - 8 - 16 * 5],     xmm10
    movdqa  [rbp - 8 - 16 * 6],     xmm11
    movdqa  [rbp - 8 - 16 * 7],     xmm12
    movdqa  [rbp - 8 - 16 * 8],     xmm13
    movdqa  [rbp - 8 - 16 * 9],     xmm14
    movdqa  [rbp - 8 - 16 * 10],    xmm15
%endif

; Align stack on 32 byte boundary.
    and     rsp,    0xffffffffffffffe0

    chacha_avx_2_%{1}_args

    test    g_blocks,       g_blocks
    mov     g_blocks_cnt,   g_blocks
    jz      .blocks_loop_end

.blocks_init:

; State representation, columns are registers:
;   A0 A1 A2 A3 | A4 A5 A6 A7 | A8 A9 A10 A11 | A12 A13 A14 A15
;   B0 B1 B2 B3 | B4 B5 B6 B7 | B8 B9 B10 B11 | B12 B13 B14 B15
;   C0 C1 C2 C3 | C4 C5 C6 C7 | C8 C9 C10 C11 | C12 C13 C14 C15
;   D0 D1 D2 D3 | D4 D5 D6 D7 | D8 D9 D10 D11 | D12 D13 D14 D15
;   E0 E1 E2 E3 | E4 E5 E6 E7 | E8 E9 E10 E11 | E12 E13 E14 E15
;   F0 F1 F2 F3 | F4 F5 F6 F7 | F8 F9 F10 F11 | F12 F13 F14 F15
;   G0 G1 G2 G3 | G4 G5 G6 G7 | G8 G9 G10 G11 | G12 G13 G14 G15
;   H0 H1 H2 H3 | H4 H5 H6 H7 | H8 H9 H10 H11 | H12 H13 H14 H15
;
; This representation allows rows to be processed in parallel.

    vbroadcasti128  y_state_eval(12),    m_state(16 * 0)
    vbroadcasti128  y_state_eval(13),    m_state(16 * 1)
    vbroadcasti128  y_state_eval(14),    m_state(16 * 2)
    vbroadcasti128  y_state_eval(15),    m_state(16 * 3)

    vpshufd y_state_eval(0),    y_state_eval(12),   00000000b
    vpshufd y_state_eval(1),    y_state_eval(12),   01010101b
    vpshufd y_state_eval(2),    y_state_eval(12),   10101010b
    vpshufd y_state_eval(3),    y_state_eval(12),   11111111b
    vmovdqa s_state_base(0),    y_state_eval(0)
    vmovdqa s_state_base(1),    y_state_eval(1)
    vmovdqa s_state_base(2),    y_state_eval(2)
    vmovdqa s_state_base(3),    y_state_eval(3)

    vpshufd y_state_eval(4),    y_state_eval(13),   00000000b
    vpshufd y_state_eval(5),    y_state_eval(13),   01010101b
    vpshufd y_state_eval(6),    y_state_eval(13),   10101010b
    vpshufd y_state_eval(7),    y_state_eval(13),   11111111b
    vmovdqa s_state_base(4),    y_state_eval(4)
    vmovdqa s_state_base(5),    y_state_eval(5)
    vmovdqa s_state_base(6),    y_state_eval(6)
    vmovdqa s_state_base(7),    y_state_eval(7)

    vpshufd y_state_eval(8),    y_state_eval(14),   00000000b
    vpshufd y_state_eval(9),    y_state_eval(14),   01010101b
    vpshufd y_state_eval(10),   y_state_eval(14),   10101010b
    vpshufd y_state_eval(11),   y_state_eval(14),   11111111b
    vmovdqa s_state_base(8),    y_state_eval(8)
    vmovdqa s_state_base(9),    y_state_eval(9)
    vmovdqa s_state_base(10),   y_state_eval(10)
    vmovdqa s_state_base(11),   y_state_eval(11)

    vpshufd y_state_eval(12),   y_state_eval(15),   00000000b
    vpshufd y_state_eval(13),   y_state_eval(15),   01010101b
    vpaddd  y_state_eval(12),   y_state_eval(12),   [rel .constant_state_add_init]
    vpshufd y_state_eval(14),   y_state_eval(15),   10101010b
    vpshufd y_state_eval(15),   y_state_eval(15),   11111111b

    vmovdqa s_state_base(12),   y_state_eval(12)
    vmovdqa s_state_base(13),   y_state_eval(13)
    vmovdqa s_state_base(14),   y_state_eval(14)
    vmovdqa s_state_base(15),   y_state_eval(15)

; Avoid loading state from stack.
    jmp     .blocks_loop_first

.blocks_loop:

    vmovdqa y_state_eval(0),    s_state_base(0)
    vmovdqa y_state_eval(1),    s_state_base(1)
    vmovdqa y_state_eval(2),    s_state_base(2)
    vmovdqa y_state_eval(3),    s_state_base(3)
    vmovdqa y_state_eval(4),    s_state_base(4)
    vmovdqa y_state_eval(5),    s_state_base(5)
    vmovdqa y_state_eval(6),    s_state_base(6)
    vmovdqa y_state_eval(7),    s_state_base(7)
    vmovdqa y_state_eval(8),    s_state_base(8)
    vmovdqa y_state_eval(9),    s_state_base(9)
    vmovdqa y_state_eval(10),   s_state_base(10)
    vmovdqa y_state_eval(11),   s_state_base(11)
    vmovdqa y_state_eval(12),   s_state_base(12)
    vmovdqa y_state_eval(13),   s_state_base(13)
    vmovdqa y_state_eval(14),   s_state_base(14)
    vmovdqa y_state_eval(15),   s_state_base(15)

.blocks_loop_first:

        mov     g_rounds_cnt,       g_rounds

.rounds_loop:

    chacha_avx_2_rounds     \
         0,  1,  2,  3,     \
         4,  5,  6,  7,     \
         8,  9, 10, 11,     \
        12, 13, 14, 15,     \
        16, 12

    chacha_avx_2_rounds     \
         0,  1,  2,  3,     \
         4,  5,  6,  7,     \
         8,  9, 10, 11,     \
        12, 13, 14, 15,     \
         8,  7

    chacha_avx_2_rounds     \
         0,  1,  2,  3,     \
         5,  6,  7,  4,     \
        10, 11,  8,  9,     \
        15, 12, 13, 14,     \
        16, 12

    chacha_avx_2_rounds     \
         0,  1,  2,  3,     \
         5,  6,  7,  4,     \
        10, 11,  8,  9,     \
        15, 12, 13, 14,     \
         8,  7

    sub     g_rounds_cnt,   1
    jnz     .rounds_loop

.rounds_loop_end:

    chacha_avx_2_%{1}_body

    sub     g_blocks_cnt,   1
    jnz     .blocks_loop

.blocks_loop_end:

%ifdef MILO_INTERNAL_ABI_WIND
    movdqa  xmm6,   [rbp - 8 - 16 * 1]
    movdqa  xmm7,   [rbp - 8 - 16 * 2]
    movdqa  xmm8,   [rbp - 8 - 16 * 3]
    movdqa  xmm9,   [rbp - 8 - 16 * 4]
    movdqa  xmm10,  [rbp - 8 - 16 * 5]
    movdqa  xmm11,  [rbp - 8 - 16 * 6]
    movdqa  xmm12,  [rbp - 8 - 16 * 7]
    movdqa  xmm13,  [rbp - 8 - 16 * 8]
    movdqa  xmm14,  [rbp - 8 - 16 * 9]
    movdqa  xmm15,  [rbp - 8 - 16 * 10]
%endif

    vzeroupper

    mov     rsp,    rbp
    pop     rbx
    pop     rbp
; Return blocks done.
; Parallelization factor is 8.
    mov     rax,    g_blocks
    shl     rax,    3
    ret
%endmacro

    section .text
milo_primitive_detail_cipher_chacha_impl_hw_x86_64_avx_2_ietf_transform:
; Args:
;   0 -> rounds
;   1 -> blocks
;   2 -> state_ptr
;   3 -> dst_ptr
;   4 -> src_ptr
; Return:
;   rax -> blocks
    chacha_avx_2_body   transform

milo_primitive_detail_cipher_chacha_impl_hw_x86_64_avx_2_ietf_generate:
; Args:
;   0 -> rounds
;   1 -> blocks
;   2 -> state_ptr
;   3 -> dst_ptr
; Return:
;   rax -> blocks
    chacha_avx_2_body   generate

; END cipher_chacha_impl_hw_x86_64_avx_2
