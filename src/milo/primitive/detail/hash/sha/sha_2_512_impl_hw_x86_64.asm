; asmsyntax=nasm

bits 64

%include "abi.asm"

; BEG hash_sha_2_512_impl_hw_x86_64

; Constants.
%define c_block_size            128

; Stack.
%xdefine    stack_size          (8)
%xdefine    s_state_base(i)     [rsp + stack_size + 8 * (i)]
%xdefine    stack_size          (stack_size + 8 * 8)
%xdefine    s_schedule(i)       [rsp + stack_size + 8 * (i)]
%xdefine    stack_size          (stack_size + 8 * 16)
%xdefine    s_params(i)         [rsp + stack_size + 8 * (i)]
%xdefine    stack_size          (stack_size + 8 * 2)

%ifdef MILO_INTERNAL_ABI_UNIX
%xdefine    s_clobbers(i)       [rsp + stack_size + 8 * ((6 - 1) - (i))]
%xdefine    stack_size          (stack_size + 8 * 6)
%endif

%ifdef MILO_INTERNAL_ABI_WIND
%xdefine    s_clobbers(i)       [rsp + stack_size + 8 * ((8 - 1) - (i))]
%xdefine    stack_size          (stack_size + 8 * 8)
%endif

; Variables.
%define     g_blocks            abi_arg_0
%define     g_blocks_cnt        abi_arg_0
%define     g_state             abi_arg_1
%define     m_state(i)          [g_state + (i)]
%define     g_src               abi_arg_2

; Aliases are selected to avoid 1st and 3rd abi arg at all and 2nd abi arg for a-h.

%ifdef MILO_INTERNAL_ABI_UNIX
%define     g_a                 rax
%define     g_b                 rbx
%define     g_c                 rcx
%define     g_d                 rbp
%define     g_e                 r8
%define     g_f                 r9
%define     g_g                 r10
%define     g_h                 r11
%define     g_tmp_0             rsi
%define     g_tmp_1             r12
%define     g_tmp_2             r13
%define     g_tmp_3             r14
%define     g_tmp_4             r15
%endif

%ifdef MILO_INTERNAL_ABI_WIND
%define     g_a                 rax
%define     g_b                 rbx
%define     g_c                 rdi
%define     g_d                 rbp
%define     g_e                 rsi
%define     g_f                 r9
%define     g_g                 r10
%define     g_h                 r11
%define     g_tmp_0             rdx
%define     g_tmp_1             r12
%define     g_tmp_2             r13
%define     g_tmp_3             r14
%define     g_tmp_4             r15
%endif

%define     g_tmp(i)            g_tmp_ %+ %eval(i)

%define     m_src(i)            [g_src + (i)]
%define     m_k(i)              [rel .constant_k + 8 * (i)]

section .text
milo_primitive_detail_hash_sha_2_256_impl_hw_x86_64_blocks:
; Args:
;   0 -> blocks
;   1 -> state_ptr
;   2 -> src_ptr
; Return:
;   rax -> blocks

section .rodata
    align 16
.constant_k:
    dq  0x428a2f98d728ae22, 0x7137449123ef65cd
    dq  0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc
    dq  0x3956c25bf348b538, 0x59f111f1b605d019
    dq  0x923f82a4af194f9b, 0xab1c5ed5da6d8118
    dq  0xd807aa98a3030242, 0x12835b0145706fbe
    dq  0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2
    dq  0x72be5d74f27b896f, 0x80deb1fe3b1696b1
    dq  0x9bdc06a725c71235, 0xc19bf174cf692694
    dq  0xe49b69c19ef14ad2, 0xefbe4786384f25e3
    dq  0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65
    dq  0x2de92c6f592b0275, 0x4a7484aa6ea6e483
    dq  0x5cb0a9dcbd41fbd4, 0x76f988da831153b5
    dq  0x983e5152ee66dfab, 0xa831c66d2db43210
    dq  0xb00327c898fb213f, 0xbf597fc7beef0ee4
    dq  0xc6e00bf33da88fc2, 0xd5a79147930aa725
    dq  0x06ca6351e003826f, 0x142929670a0e6e70
    dq  0x27b70a8546d22ffc, 0x2e1b21385c26c926
    dq  0x4d2c6dfc5ac42aed, 0x53380d139d95b3df
    dq  0x650a73548baf63de, 0x766a0abb3c77b2a8
    dq  0x81c2c92e47edaee6, 0x92722c851482353b
    dq  0xa2bfe8a14cf10364, 0xa81a664bbc423001
    dq  0xc24b8b70d0f89791, 0xc76c51a30654be30
    dq  0xd192e819d6ef5218, 0xd69906245565a910
    dq  0xf40e35855771202a, 0x106aa07032bbd1b8
    dq  0x19a4c116b8d2d0c8, 0x1e376c085141ab53
    dq  0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8
    dq  0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb
    dq  0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3
    dq  0x748f82ee5defb2fc, 0x78a5636f43172f60
    dq  0x84c87814a1f0ab72, 0x8cc702081a6439ec
    dq  0x90befffa23631e28, 0xa4506cebde82bde9
    dq  0xbef9a3f7b2c67915, 0xc67178f2e372532b
    dq  0xca273eceea26619c, 0xd186b8c721c0c207
    dq  0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178
    dq  0x06f067aa72176fba, 0x0a637dc5a2c898a6
    dq  0x113f9804bef90dae, 0x1b710b35131c471b
    dq  0x28db77f523047d84, 0x32caab7b40c72493
    dq  0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c
    dq  0x4cc5d4becb3e42b6, 0x597f299cfc657e2a
    dq  0x5fcb6fab3ad6faec, 0x6c44198c4a475817
section .text

.start:

    test        g_blocks,   g_blocks
    jz          .ret

; Save clobbers.
    sub         rsp,            stack_size
    mov         s_clobbers(0),  rbp
    mov         s_clobbers(1),  rbx
    mov         s_clobbers(2),  r12
    mov         s_clobbers(3),  r13
    mov         s_clobbers(4),  r14
    mov         s_clobbers(5),  r15
%ifdef MILO_INTERNAL_ABI_WIND
    mov         s_clobbers(6),  rdi
    mov         s_clobbers(7),  rsi
%endif

; Save params.
    mov         s_params(0),    g_blocks
    mov         s_params(1),    g_state
; Load state.
    mov         g_a,            m_state(0)
    mov         g_b,            m_state(1)
    mov         g_c,            m_state(2)
    mov         g_d,            m_state(3)
    mov         g_e,            m_state(4)
    mov         g_f,            m_state(5)
    mov         g_g,            m_state(6)
    mov         g_h,            m_state(7)

.blocks_loop:
; Read block.
; TODO
; Advance pointer.
    add         g_src,          c_block_size
; Rounds.
; TODO
; Merge state.
; TODO

    sub         g_blocks_cnt,   1
    jnz         .blocks_loop

.blocks_loop_end:
; Save state.
    mov         m_state(0), g_a
    mov         m_state(1), g_b
    mov         m_state(2), g_c
    mov         m_state(3), g_d
    mov         m_state(4), g_e
    mov         m_state(5), g_f
    mov         m_state(6), g_g
    mov         m_state(7), g_h
; Restore clobbers.
    mov         rbp,    s_clobbers(0)
    mov         rbx,    s_clobbers(1)
    mov         r12,    s_clobbers(2)
    mov         r13,    s_clobbers(3)
    mov         r14,    s_clobbers(4)
    mov         r15,    s_clobbers(5)
%ifdef MILO_INTERNAL_ABI_WIND
    mov         rdi,    s_clobbers(6)
    mov         rsi,    s_clobbers(7)
%endif

; Return blocks done.
    mov         rax,    s_params(0)
; Restore stack.
    add         rsp,    stack_size
    ret
.ret:
    xor         rax,    rax
    ret

; END hash_sha_2_512_impl_hw_x86_64
