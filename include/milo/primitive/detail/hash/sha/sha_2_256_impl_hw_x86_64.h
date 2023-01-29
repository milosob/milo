

#pragma once


#include <milo/internal.h>


namespace milo::primitive::detail
{
    #if MILO_INTERNAL_ASM && MILO_INTERNAL_ARCH_X86_64
    
    /*
     * @formatter:off
     */
    
    extern "C"
    auto
    milo_primitive_detail_hash_sha_2_256_impl_hw_x86_64_ni_blocks(
        uint64_t a_blocks,
        uint32_t* a_state_ptr,
        const uint8_t* a_src_ptr
    ) noexcept(true) -> uint64_t;
    
    /*
     * @formatter:on
     */

    class hash_sha_2_256_impl_hw_x86_64_ni
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
                    
                        using avx_1 = int;
                    
                        using sha_2 = int;
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
            size_t a_blocks,
            uint32_t* a_state_ptr,
            const t_src* a_src_ptr
        ) noexcept(true) -> void
        {
            milo_primitive_detail_hash_sha_2_256_impl_hw_x86_64_ni_blocks(
                a_blocks,
                a_state_ptr,
                reinterpret_cast<const uint8_t*>(a_src_ptr)
            );
        }
    };
    
    #else
    
    using hash_sha_2_256_impl_hw_x86_64_ni = void
    
    #endif
}
