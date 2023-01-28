

#pragma once


#include <milo/internal.h>


namespace milo::primitive::detail
{
    #if MILO_INTERNAL_ASM && MILO_INTERNAL_ARCH_X86_64
    
    extern "C"
    auto
    milo_primitive_detail_hash_sha_1_160_impl_hw_x86_64_ni_blocks(
        uint64_t a_blocks,
        uint32_t* a_h_ptr,
        const uint8_t* a_src_ptr
    ) noexcept(true) -> uint64_t;
    
    class hash_sha_1_160_impl_hw_x86_64_ni
    {
    public:
        
        struct requirements
        {
            struct arch
            {
                struct x86
                {
                    struct ise
                    {
                        using sse_1 = int;
                        
                        using sse_2 = int;
                        
                        using sse_3 = int;
                        
                        using ssse_3 = int;
                        
                        using sha_1 = int;
                    };
                };
            };
        };
    
    public:
        
        static
        constexpr size_t block_size = 64;
    
    public:
        
        template<
            meta::byte t_src
        >
        static
        auto
        blocks(
            const t_src* a_src_ptr,
            size_t a_blocks,
            uint32_t* a_h_ptr
        ) noexcept(true) -> void
        {
            milo_primitive_detail_hash_sha_1_160_impl_hw_x86_64_ni_blocks(
                a_blocks,
                a_h_ptr,
                reinterpret_cast<const uint8_t*>(a_src_ptr)
            );
        }
    };
    
    #else
    
    using hash_sha_1_160_impl_hw_x86_64_ni = void;
    
    #endif
}