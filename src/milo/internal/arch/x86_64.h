

#pragma once


#include <milo/internal/base.h>


#if defined(__x86_64__) || \
    defined(_WIN64)
    #define MILO_INTERNAL_ARCH_X86_64 true
#else
    #define MILO_INTERNAL_ARCH_X86_64 false
#endif

namespace milo::internal
{
    /*
     * @formatter:off
     */
    
    #if MILO_INTERNAL_ASM && MILO_INTERNAL_ARCH_X86_64
    
    extern "C"
    auto
    milo_internal_arch_x86_64_cpuid(
        uint32_t* a_info_ptr,
        uint32_t a_eax,
        uint32_t a_ecx
    ) noexcept(true) -> void;
    
    #endif
    
    /*
     * @formatter:on
     */
    
    #if MILO_INTERNAL_ARCH_X86_64
    
    auto
    arch_x86_64_cpuid(
        uint32_t* a_info_ptr,
        uint32_t a_eax,
        uint32_t a_ecx = 0
    ) noexcept(true) -> void
    {
        #if MILO_INTERNAL_ASM && MILO_INTERNAL_ARCH_X86_64
        
        milo_internal_arch_x86_64_cpuid(
            a_info_ptr,
            a_eax,
            a_ecx
        );
        
        #else
        
        a_info_ptr[0] = 0;
        a_info_ptr[1] = 0;
        a_info_ptr[2] = 0;
        a_info_ptr[3] = 0;
        
        #endif
    }
    
    inline
    const
    class arch_x86_64_info_basic
    {
    private:
        
        struct registers
        {
            uint32_t eax;
            uint32_t ebx;
            uint32_t ecx;
            uint32_t edx;
        };
    
    private:
        
        registers m_features[2]{};
    
    public:
        
        arch_x86_64_info_basic() noexcept(true)
        {
            uint32_t max;
            uint32_t info[4]{};
            
            arch_x86_64_cpuid(
                info,
                0
            );
            
            max = info[0];
            
            if (max >= 1)
            {
                arch_x86_64_cpuid(
                    info,
                    1
                );
                
                m_features[0] = {info[0], info[1], info[2], info[3]};
            }
            
            if (max >= 7)
            {
                arch_x86_64_cpuid(
                    info,
                    7
                );
                
                m_features[1] = {info[0], info[1], info[2], info[3]};
            }
        }
        
        arch_x86_64_info_basic(arch_x86_64_info_basic&& a_object) noexcept(true) = default;
        
        arch_x86_64_info_basic(const arch_x86_64_info_basic& a_object) noexcept(true) = default;
        
        ~arch_x86_64_info_basic() noexcept(true) = default;
    
    public:
        
        auto
        operator =(const arch_x86_64_info_basic& a_object) noexcept(true) -> arch_x86_64_info_basic& = default;
    
    public:
        
        auto
        sse_1(
        ) const noexcept(true) -> bool
        {
            return m_features[0].edx & (1 << 25);
        }
        
        auto
        sse_2(
        ) const noexcept(true) -> bool
        {
            return m_features[0].edx & (1 << 26);
        }
        
        auto
        sse_3(
        ) const noexcept(true) -> bool
        {
            return m_features[0].ecx & (1 << 0);
        }
        
        auto
        ssse_3(
        ) const noexcept(true) -> bool
        {
            return m_features[0].ecx & (1 << 9);
        }
        
        auto
        sse_4_1(
        ) const noexcept(true) -> bool
        {
            return m_features[0].ecx & (1 << 19);
        }
        
        auto
        sse_4_2(
        ) const noexcept(true) -> bool
        {
            return m_features[0].ecx & (1 << 20);
        }
        
        auto
        avx_1(
        ) const noexcept(true) -> bool
        {
            return m_features[0].ecx & (1 << 28);
        }
        
        auto
        avx_2(
        ) const noexcept(true) -> bool
        {
            return m_features[1].ebx & (1 << 5);
        }
        
        auto
        sha_1(
        ) const noexcept(true) -> bool
        {
            return m_features[1].ebx & (1 << 29);
        }
        
        auto
        sha_2(
        ) const noexcept(true) -> bool
        {
            return m_features[1].ebx & (1 << 29);
        }
    } arch_x86_64_info;
    
    struct arch_x86_64_ise
    {
        inline
        static
        const bool sse_1 = arch_x86_64_info.sse_1();
        
        inline
        static
        const bool sse_2 = arch_x86_64_info.sse_2();
        
        inline
        static
        const bool sse_3 = arch_x86_64_info.sse_3();
        
        inline
        static
        const bool ssse_3 = arch_x86_64_info.ssse_3();
        
        inline
        static
        const bool sse_4_1 = arch_x86_64_info.sse_4_1();
        
        inline
        static
        const bool sse_4_2 = arch_x86_64_info.sse_4_2();
        
        inline
        static
        const bool sse_4 = arch_x86_64_info.sse_4_1() && arch_x86_64_info.sse_4_2();
        
        inline
        static
        const bool avx_1 = arch_x86_64_info.avx_1();
        
        inline
        static
        const bool avx_2 = arch_x86_64_info.avx_2();
        
        inline
        static
        const bool sha_1 = arch_x86_64_info.sha_1();
        
        inline
        static
        const bool sha_2 = arch_x86_64_info.sha_2();
        
        template<
            typename t_impl
        >
        static
        constexpr auto
        check(
        ) noexcept(true) -> bool
        {
            if constexpr (requires{
                typename t_impl::requirements::arch::x86_64;
            })
            {
                if constexpr (requires{
                    typename t_impl::requirements::arch::x86_64::ise;
                })
                {
                    using impl_type = typename t_impl::requirements::arch::x86_64::ise;
                    
                    if constexpr (requires {
                        typename impl_type::sse_1;
                    })
                    {
                        if (!sse_1)
                        {
                            return false;
                        }
                    }
                    
                    if constexpr (requires {
                        typename impl_type::sse_2;
                    })
                    {
                        if (!sse_2)
                        {
                            return false;
                        }
                    }
                    
                    if constexpr (requires {
                        typename impl_type::sse_3;
                    })
                    {
                        if (!sse_3)
                        {
                            return false;
                        }
                    }
                    
                    if constexpr (requires {
                        typename impl_type::ssse_3;
                    })
                    {
                        if (!ssse_3)
                        {
                            return false;
                        }
                    }
                    
                    if constexpr (requires {
                        typename impl_type::sse_4_1;
                    })
                    {
                        if (!sse_4_1)
                        {
                            return false;
                        }
                    }
                    
                    if constexpr (requires {
                        typename impl_type::sse_4_2;
                    })
                    {
                        if (!sse_4_2)
                        {
                            return false;
                        }
                    }
                    
                    if constexpr (requires {
                        typename impl_type::sse_4;
                    })
                    {
                        if (!sse_4)
                        {
                            return false;
                        }
                    }
                    
                    if constexpr (requires {
                        typename impl_type::avx_1;
                    })
                    {
                        if (!avx_1)
                        {
                            return false;
                        }
                    }
                    
                    if constexpr (requires {
                        typename impl_type::avx_2;
                    })
                    {
                        if (!avx_2)
                        {
                            return false;
                        }
                    }
                    
                    if constexpr (requires {
                        typename impl_type::sha_1;
                    })
                    {
                        if (!sha_1)
                        {
                            return false;
                        }
                    }
                    
                    if constexpr (requires {
                        typename impl_type::sha_2;
                    })
                    {
                        if (!sha_2)
                        {
                            return false;
                        }
                    }
                }
                
                return true;
            }
            else
            {
                return false;
            }
        }
    };
    
    struct arch_x86_64
    {
        static
        constexpr bool value = true;
        
        using ise = arch_x86_64_ise;
    };
    
    #else
    
    struct arch_x86_64
    {
        static
        constexpr bool value = false;
        
        using ise = void;
    };
    
    #endif
}
