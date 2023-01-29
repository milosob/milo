; asmsyntax=nasm

bits 64

global milo_primitive_detail_hash_sha_1_160_impl_hw_x86_64_ni_blocks:

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

; BEG hash_sha_1_160_impl_hw_x86_64_ni

; TODO Support for wabi.

; Constants.
%define     c_block_size        64

; Stack.

; Variables.
%define     x_endian_mask       xmm0
%define     x_state_save(i)     xmm %+ %eval((i) + 1)
%define     x_state_base(i)     xmm %+ %eval((i) + 3)
%define     x_message(i)        xmm %+ %eval((i) + 6)
%define     g_blocks            abi_arg_0
%define     g_blocks_cnt        rax
%define     g_state             abi_arg_1
%define     m_state(i)          [g_state + (i)]
%define     g_src               abi_arg_2
%define     m_src(i)            [g_src + (i)]

%macro sha_1_160_rounds 8
; Args:
;   1       ->  function
;   2-4     ->  states
;   5-8     ->  messages

    sha1nexte   x_state_base(%3),   x_message(%5)
    movdqa      x_state_base(%4),   x_state_base(%2)
    sha1msg2    x_message(%6),      x_message(%5)
    sha1rnds4   x_state_base(%2),   x_state_base(%3),   %1
    sha1msg1    x_message(%8),      x_message(%5)
    pxor        x_message(%7),      x_message(%5)
%endmacro

section .text
milo_primitive_detail_hash_sha_1_160_impl_hw_x86_64_ni_blocks:
; Args:
;   0 -> blocks
;   1 -> state_ptr
;   2 -> src_ptr

section .rodata
    align 16
.constant_endian_mask:
    db  0x0f, 0x0e, 0x0d, 0x0c
    db  0x0b, 0x0a, 0x09, 0x08
    db  0x07, 0x06, 0x05, 0x04
    db  0x03, 0x02, 0x01, 0x00
section .text

; Load state.
    movdqu  x_state_base(0),    m_state(0)
    movd    x_state_base(1),    m_state(16)
    movdqa  x_endian_mask,      [rel .constant_endian_mask]
    pshufd  x_state_base(0),    x_state_base(0),    0b00011011
    pslldq  x_state_base(1),    12

    test    g_blocks,       g_blocks
    mov     g_blocks_cnt,   g_blocks
    jz      .blocks_loop_end

.blocks_loop:

; Read block.
    movdqu  x_message(0),       m_src(16 * 0)
    movdqu  x_message(1),       m_src(16 * 1)
    movdqu  x_message(2),       m_src(16 * 2)
    movdqu  x_message(3),       m_src(16 * 3)
    movdqa  x_state_save(0),    x_state_base(0)
    movdqa  x_state_save(1),    x_state_base(1)
; Advance pointer.
    add     g_src,              c_block_size
; Rounds 0 to 3.
    pshufb      x_message(0),       x_endian_mask
    paddd       x_state_base(1),    x_message(0)
    movdqa      x_state_base(2),    x_state_base(0)
    sha1rnds4   x_state_base(0),    x_state_base(1),    0
; Rounds 4 to 7.
    pshufb      x_message(1),       x_endian_mask
    sha1nexte   x_state_base(2),    x_message(1)
    movdqa      x_state_base(1),    x_state_base(0)
    sha1rnds4   x_state_base(0),    x_state_base(2),    0
    sha1msg1    x_message(0),       x_message(1)
; Rounds 8 to 11.
    pshufb      x_message(2),       x_endian_mask
    sha1nexte   x_state_base(1),    x_message(2)
    movdqa      x_state_base(2),    x_state_base(0)
    sha1rnds4   x_state_base(0),    x_state_base(1),    0
    sha1msg1    x_message(1),       x_message(2)
    pxor        x_message(0),       x_message(2)
; Rounds 12 to 15.
    pshufb      x_message(3),       x_endian_mask
    sha_1_160_rounds    0, 0, 2, 1, 3, 0, 1, 2
; Rounds 16 to 19.
    sha_1_160_rounds    0, 0, 1, 2, 0, 1, 2, 3
; Rounds 20 to 23.
    sha_1_160_rounds    1, 0, 2, 1, 1, 2, 3, 0
; Rounds 24 to 27.
    sha_1_160_rounds    1, 0, 1, 2, 2, 3, 0, 1
; Rounds 28 to 31.
    sha_1_160_rounds    1, 0, 2, 1, 3, 0, 1, 2
; Rounds 32 to 35.
    sha_1_160_rounds    1, 0, 1, 2, 0, 1, 2, 3
; Rounds 36 to 39.
    sha_1_160_rounds    1, 0, 2, 1, 1, 2, 3, 0
; Rounds 40 to 43.
    sha_1_160_rounds    2, 0, 1, 2, 2, 3, 0, 1
; Rounds 44 to 47.
    sha_1_160_rounds    2, 0, 2, 1, 3, 0, 1, 2
; Rounds 48 to 51.
    sha_1_160_rounds    2, 0, 1, 2, 0, 1, 2, 3
; Rounds 52 to 55.
    sha_1_160_rounds    2, 0, 2, 1, 1, 2, 3, 0
; Rounds 56 to 59.
    sha_1_160_rounds    2, 0, 1, 2, 2, 3, 0, 1
; Rounds 60 to 63.
    sha_1_160_rounds    3, 0, 2, 1, 3, 0, 1, 2
; Rounds 64 to 67.
    sha_1_160_rounds    3, 0, 1, 2, 0, 1, 2, 3
; Rounds 68 to 71.
    sha1nexte   x_state_base(2),    x_message(1)
    movdqa      x_state_base(1),    x_state_base(0)
    sha1msg2    x_message(2),       x_message(1)
    sha1rnds4   x_state_base(0),    x_state_base(2),    3
    pxor        x_message(3),       x_message(1)
; Rounds 72 to 75.
    sha1nexte   x_state_base(1),    x_message(2)
    movdqa      x_state_base(2),    x_state_base(0)
    sha1msg2    x_message(3),       x_message(2)
    sha1rnds4   x_state_base(0),    x_state_base(1),    3
; Rounds 76 to 79.
    sha1nexte   x_state_base(2),    x_message(3)
    movdqa      x_state_base(1),    x_state_base(0)
    sha1rnds4   x_state_base(0),    x_state_base(2),    3
; Merge state.
    sha1nexte   x_state_base(1),    x_state_save(1)
    paddd       x_state_base(0),    x_state_save(0)

    sub     g_blocks_cnt,   1
    jnz     .blocks_loop

.blocks_loop_end:

; Save state.
    pshufd  x_state_base(0),    x_state_base(0),    0b00011011
    psrldq  x_state_base(1),    12
    movdqu  m_state(0),         x_state_base(0)
    movd    m_state(16),        x_state_base(1)
; Return blocks done.
    mov     rax,    g_blocks
    ret

; END hash_sha_1_160_impl_hw_x86_64_ni
