; asmsyntax=nasm

bits 64

global milo_primitive_detail_hash_sha_2_256_impl_hw_x86_64_ni_blocks:

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

; BEG hash_sha_2_256_impl_hw_x86_64_ni

; TODO Support for wabi.

; Constants.
%define     c_block_size        64

; Stack.

; Variables.
%define     g_blocks            abi_arg_0
%define     g_blocks_cnt        rax
%define     g_state             abi_arg_1
%define     m_state(i)          [g_state + (i)]
%define     g_src               abi_arg_2
%define     m_src(i)            [g_src + (i)]
%define     m_k(i)              [rel .constant_k + 16 * (i)]
%define     x_implicit          xmm0
%define     x_state_base(i)     xmm %+ %eval((i) + 1)
%define     x_message(i)        xmm %+ %eval((i) + 3)
%define     x_shuff_mask        xmm8
%define     x_state_save(i)     xmm %+ %eval((i) + 9)

%macro sha_2_256_rounds 5
; Args:
;   1-4     ->  messages
;   5       ->  k

    movdqa      x_implicit,         x_message(%4)
    movdqa      x_message(4),       x_message(%4)
    paddd       x_implicit,         m_k(%5)
    sha256rnds2 x_state_base(1),    x_state_base(0)
    palignr     x_message(4),       x_message(%3),   4
    paddd       x_message(%1),      x_message(4)
    pshufd      x_implicit,         x_implicit, 0b00001110
    sha256msg2  x_message(%1),      x_message(%4)
    sha256rnds2 x_state_base(0),    x_state_base(1)
    sha256msg1  x_message(%3),      x_message(%4)
%endmacro

section .text
milo_primitive_detail_hash_sha_2_256_impl_hw_x86_64_ni_blocks:
; Args:
;   0 -> blocks
;   1 -> state_ptr
;   2 -> src_ptr

section .rodata
    align 16
.constant_shuffle_mask_message:
    db  0x03, 0x02, 0x01, 0x00
    db  0x07, 0x06, 0x05, 0x04
    db  0x0b, 0x0a, 0x09, 0x08
    db  0x0f, 0x0e, 0x0d, 0x0c
.constant_k:
    dq  0x71374491428a2f98, 0xe9b5dba5b5c0fbcf
    dq  0x59f111f13956c25b, 0xab1c5ed5923f82a4
    dq  0x12835b01d807aa98, 0x550c7dc3243185be
    dq  0x80deb1fe72be5d74, 0xc19bf1749bdc06a7
    dq  0xefbe4786e49b69c1, 0x240ca1cc0fc19dc6
    dq  0x4a7484aa2de92c6f, 0x76f988da5cb0a9dc
    dq  0xa831c66d983e5152, 0xbf597fc7b00327c8
    dq  0xd5a79147c6e00bf3, 0x1429296706ca6351
    dq  0x2e1b213827b70a85, 0x53380d134d2c6dfc
    dq  0x766a0abb650a7354, 0x92722c8581c2c92e
    dq  0xa81a664ba2bfe8a1, 0xc76c51a3c24b8b70
    dq  0xd6990624d192e819, 0x106aa070f40e3585
    dq  0x1e376c0819a4c116, 0x34b0bcb52748774c
    dq  0x4ed8aa4a391c0cb3, 0x682e6ff35b9cca4f
    dq  0x78a5636f748f82ee, 0x8cc7020884c87814
    dq  0xa4506ceb90befffa, 0xc67178f2bef9a3f7
section .text

.start:
; Load state.
    movdqu      x_state_save(0),    m_state(16 * 0)
    movdqu      x_state_save(1),    m_state(16 * 1)
    movdqa      x_shuff_mask,       [rel .constant_shuffle_mask_message]
    pshufd      x_state_save(0),    x_state_save(0),    0b10110001
    pshufd      x_state_save(1),    x_state_save(1),    0b10110001
    vpunpcklqdq x_state_base(0),    x_state_save(1),    x_state_save(0)
    vpunpckhqdq x_state_base(1),    x_state_save(1),    x_state_save(0)

    test        g_blocks,       g_blocks
    mov         g_blocks_cnt,   g_blocks
    jz          .blocks_loop_end

.blocks_loop:
; Read block.
    movdqu      x_implicit,         m_src(16 * 0)
    movdqu      x_message(1),       m_src(16 * 1)
    movdqu      x_message(2),       m_src(16 * 2)
    movdqu      x_message(3),       m_src(16 * 3)
    movdqa      x_state_save(0),    x_state_base(0)
    movdqa      x_state_save(1),    x_state_base(1)
; Advance pointer.
    add         g_src,              c_block_size
; Rounds 0 to 3.
    pshufb      x_implicit,         x_shuff_mask
    pshufb      x_message(1),       x_shuff_mask
    movdqa      x_message(0),       x_implicit
    paddd       x_implicit,         m_k(0)
    sha256rnds2 x_state_base(1),    x_state_base(0)
    pshufd      x_implicit,         x_implicit, 0b00001110
    sha256rnds2 x_state_base(0),    x_state_base(1)
; Rounds 4 to 7.
    movdqa      x_implicit,         x_message(1)
    pshufb      x_message(2),       x_shuff_mask
    paddd       x_implicit,         m_k(1)
    sha256rnds2 x_state_base(1),    x_state_base(0)
    pshufd      x_implicit,         x_implicit, 0b00001110
    sha256rnds2 x_state_base(0),    x_state_base(1)
    sha256msg1  x_message(0),       x_message(1)
; Rounds 8 to 11.
    movdqa      x_implicit,         x_message(2)
    pshufb      x_message(3),       x_shuff_mask
    paddd       x_implicit,         m_k(2)
    sha256rnds2 x_state_base(1),    x_state_base(0)
    pshufd      x_implicit,         x_implicit, 0b00001110
    sha256rnds2 x_state_base(0),    x_state_base(1)
    sha256msg1  x_message(1),       x_message(2)
; Rounds 12 to 15.
    sha_2_256_rounds    0, 1, 2, 3, 3
; Rounds 16 to 19.
    sha_2_256_rounds    1, 2, 3, 0, 4
; Rounds 20 to 23.
    sha_2_256_rounds    2, 3, 0, 1, 5
; Rounds 24 to 27.
    sha_2_256_rounds    3, 0, 1, 2, 6
; Rounds 28 to 31.
    sha_2_256_rounds    0, 1, 2, 3, 7
; Rounds 32 to 35.
    sha_2_256_rounds    1, 2, 3, 0, 8
; Rounds 36 to 39.
    sha_2_256_rounds    2, 3, 0, 1, 9
; Rounds 40 to 43.
    sha_2_256_rounds    3, 0, 1, 2, 10
; Rounds 44 to 47.
    sha_2_256_rounds    0, 1, 2, 3, 11
; Rounds 48 to 51.
    sha_2_256_rounds    1, 2, 3, 0, 12
; Rounds 52 to 55.
    movdqa      x_implicit,         x_message(1)
    movdqa      x_message(4),       x_message(1)
    paddd       x_implicit,         m_k(13)
    sha256rnds2 x_state_base(1),    x_state_base(0)
    palignr     x_message(4),       x_message(0),   4
    paddd       x_message(2),       x_message(4)
    pshufd      x_implicit,         x_implicit, 0b00001110
    sha256msg2  x_message(2),       x_message(1)
    sha256rnds2 x_state_base(0),    x_state_base(1)
; Rounds 56 to 59.
    movdqa      x_implicit,         x_message(2)
    movdqa      x_message(4),       x_message(2)
    paddd       x_implicit,         m_k(14)
    sha256rnds2 x_state_base(1),    x_state_base(0)
    palignr     x_message(4),       x_message(1),   4
    paddd       x_message(3),       x_message(4)
    pshufd      x_implicit,         x_implicit, 0b00001110
    sha256msg2  x_message(3),       x_message(2)
    sha256rnds2 x_state_base(0),    x_state_base(1)
; Rounds 60 to 63.
    movdqa      x_implicit,         x_message(3)
    paddd       x_implicit,         m_k(15)
    sha256rnds2 x_state_base(1),    x_state_base(0)
    pshufd      x_implicit,         x_implicit, 0b00001110
    sha256rnds2 x_state_base(0),    x_state_base(1)
; Merge state.
    paddd       x_state_base(1),    x_state_save(1)
    paddd       x_state_base(0),    x_state_save(0)

    sub         g_blocks_cnt,   1
    jnz         .blocks_loop

.blocks_loop_end:
; Save state.
    vpunpckhqdq x_state_save(0),    x_state_base(0),    x_state_base(1)
    vpunpcklqdq x_state_save(1),    x_state_base(0),    x_state_base(1)
    pshufd      x_state_save(0),    x_state_save(0),    0b10110001
    pshufd      x_state_save(1),    x_state_save(1),    0b10110001
    movdqu      m_state(16 * 0),    x_state_save(0)
    movdqu      m_state(16 * 1),    x_state_save(1)
; Return blocks done.
    mov         rax,    g_blocks
    ret

; END hash_sha_2_256_impl_hw_x86_64_ni
