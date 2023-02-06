

#pragma once


#include <milo/internal.h>


namespace milo::primitive::detail
{
    #if MILO_INTERNAL_ASM && MILO_INTERNAL_ARCH_X86_64
    
    extern "C"
    auto
    milo_primitive_detail_cipher_chacha_impl_hw_x86_64_ssse_3_ietf_transform(
        uint64_t a_rounds,
        uint64_t a_blocks,
        const uint32_t* a_state_ptr,
        uint8_t* a_dst_ptr,
        const uint8_t* a_src_ptr
    ) noexcept(true) -> uint64_t;
    
    extern "C"
    auto
    milo_primitive_detail_cipher_chacha_impl_hw_x86_64_ssse_3_ietf_generate(
        uint64_t a_rounds,
        uint64_t a_blocks,
        const uint32_t* a_state_ptr,
        uint8_t* a_dst_ptr
    ) noexcept(true) -> uint64_t;
    
    class cipher_chacha_impl_hw_x86_64_ssse_3_ietf
    {
    public:
        
        struct requirements
        {
            struct arch
            {
                struct x86_64
                {
                    struct ise
                    {
                        using sse_1 = int;
                        
                        using sse_2 = int;
                        
                        using sse_3 = int;
                        
                        using ssse_3 = int;
                    };
                };
            };
        };
    
    public:
        
        static
        constexpr size_t block_size = 64;
        
        static
        constexpr size_t block_ratio = 4;
        
        static
        constexpr size_t rounds = 10;
    
    public:
        
        template<
            meta::byte t_dst_ptr,
            meta::byte t_src_ptr
        >
        static
        auto
        transform(
            size_t a_blocks,
            const uint32_t* a_state_ptr,
            t_dst_ptr* a_dst_ptr,
            const t_src_ptr* a_src_ptr
        ) noexcept(true) -> size_t
        {
            return milo_primitive_detail_cipher_chacha_impl_hw_x86_64_ssse_3_ietf_transform(
                uint64_t(rounds),
                uint64_t(a_blocks),
                a_state_ptr,
                reinterpret_cast<uint8_t*>(a_dst_ptr),
                reinterpret_cast<const uint8_t*>(a_src_ptr)
            );
        }
        
        template<
            meta::byte t_dst_ptr
        >
        static
        auto
        generate(
            size_t a_blocks,
            const uint32_t* a_state_ptr,
            t_dst_ptr* a_dst_ptr
        ) noexcept(true) -> size_t
        {
            return milo_primitive_detail_cipher_chacha_impl_hw_x86_64_ssse_3_ietf_generate(
                uint64_t(rounds),
                uint64_t(a_blocks),
                a_state_ptr,
                reinterpret_cast<uint8_t*>(a_dst_ptr)
            );
        }
    };
    
    #else
    
    using cipher_chacha_impl_hw_x86_64_ssse_3_ietf = void;
    
    #endif
}
