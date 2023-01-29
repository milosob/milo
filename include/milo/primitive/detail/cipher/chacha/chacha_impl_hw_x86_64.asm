; asmsyntax=nasm

bits 64

global milo_primitive_detail_cipher_chacha_impl_hw_x86_64_ssse_3_ietf_transform
global milo_primitive_detail_cipher_chacha_impl_hw_x86_64_ssse_3_ietf_generate

; uabi
%define abi_arg_0 rdi
%define abi_arg_1 rsi
%define abi_arg_2 rdx
%define abi_arg_3 rcx
%define abi_arg_4 r8
%define abi_arg_5 r9

; wabi
;%define abi_arg_0 rdx
;%define abi_arg_1 rcx
;%define abi_arg_2 r8
;%define abi_arg_3 r9
;%define abi_arg_4

; BEG chacha_impl_hw_x86_64_ssse_3

; TODO Support for wabi.

; Constants.
%define     c_ratio                 2

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
%define     g_src                   abi_arg_4
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

%macro chacha_ssse_3_transform 0
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

%macro chacha_ssse_3_generate 0
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
    sub     rsp,    stack_size + 8

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

    movdqu  x_state_eval(0),    m_state(16 * 0)
    movdqu  x_state_eval(1),    m_state(16 * 1)
    movdqu  x_state_eval(2),    m_state(16 * 2)
    movdqu  x_state_eval(3),    m_state(16 * 3)

    pshufd  x_state_eval(4),    x_state_eval(0), 00000000b
    pshufd  x_state_eval(5),    x_state_eval(0), 01010101b
    pshufd  x_state_eval(6),    x_state_eval(0), 10101010b
    pshufd  x_state_eval(7),    x_state_eval(0), 11111111b
    movdqa  s_state_base(0),    x_state_eval(4)
    movdqa  s_state_base(1),    x_state_eval(5)
    movdqa  s_state_base(2),    x_state_eval(6)
    movdqa  s_state_base(3),    x_state_eval(7)

    pshufd  x_state_eval(8),    x_state_eval(1), 00000000b
    pshufd  x_state_eval(9),    x_state_eval(1), 01010101b
    pshufd  x_state_eval(10),   x_state_eval(1), 10101010b
    pshufd  x_state_eval(11),   x_state_eval(1), 11111111b
    movdqa  s_state_base(4),    x_state_eval(8)
    movdqa  s_state_base(5),    x_state_eval(9)
    movdqa  s_state_base(6),    x_state_eval(10)
    movdqa  s_state_base(7),    x_state_eval(11)

    pshufd  x_state_eval(12),   x_state_eval(2), 00000000b
    pshufd  x_state_eval(13),   x_state_eval(2), 01010101b
    pshufd  x_state_eval(14),   x_state_eval(2), 10101010b
    pshufd  x_state_eval(15),   x_state_eval(2), 11111111b
    movdqa  s_state_base(8),    x_state_eval(12)
    movdqa  s_state_base(9),    x_state_eval(13)
    movdqa  s_state_base(10),   x_state_eval(14)
    movdqa  s_state_base(11),   x_state_eval(15)

    pshufd  x_state_eval(4),    x_state_eval(3), 00000000b
    pshufd  x_state_eval(5),    x_state_eval(3), 01010101b
    paddd   x_state_eval(4),    [rel .constant_state_add_init]
    pshufd  x_state_eval(6),    x_state_eval(3), 10101010b
    pshufd  x_state_eval(7),    x_state_eval(3), 11111111b

    movdqa  s_state_base(12),   x_state_eval(4)
    movdqa  s_state_base(13),   x_state_eval(5)
    movdqa  s_state_base(14),   x_state_eval(6)
    movdqa  s_state_base(15),   x_state_eval(7)

.blocks_loop:
    mov     g_rounds_cnt,       g_rounds

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

    chacha_ssse_3_%1

    sub     g_blocks_cnt,   1
    jnz     .blocks_loop

.blocks_loop_end:

; Leave frame.
; Return blocks done.
    mov     rax,    g_blocks
    mov     rsp,    rbp
    shl     rax,    c_ratio
    pop     rbx
    pop     rbp
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
    chacha_ssse_3_body transform

milo_primitive_detail_cipher_chacha_impl_hw_x86_64_ssse_3_ietf_generate:
; Args:
;   0 -> rounds
;   1 -> blocks
;   2 -> state_ptr
;   3 -> dst_ptr
    chacha_ssse_3_body generate

; END chacha_impl_hw_x86_64_ssse_3
