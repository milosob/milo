

#pragma once


#include <milo/internal/base.h>


#if defined(__i386__) || \
    defined(_WIN32)
    #define MILO_INTERNAL_ARCH_X86_32 true
#else
    #define MILO_INTERNAL_ARCH_X86_32 false
#endif

#if defined(__x86_64__) || \
    defined(_WIN64)
    #define MILO_INTERNAL_ARCH_X86_64 true
#else
    #define MILO_INTERNAL_ARCH_X86_64 false
#endif

#if MILO_INTERNAL_ARCH_X86_32 || \
    MILO_INTERNAL_ARCH_X86_64
    #define MILO_INTERNAL_ARCH_X86 true
#else
    #define MILO_INTERNAL_ARCH_X86 false
#endif

namespace milo::internal
{
    #if MILO_INTERNAL_ARCH_X86
    
    inline
    const
    class arch_x86_cpuid_basic
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
        
        uint32_t m_mask = 0;
        
        registers m_features[2]{};
    
    public:
        
        arch_x86_cpuid_basic() noexcept(true)
        {
            uint32_t max;
            uint32_t eax;
            uint32_t ebx;
            uint32_t ecx;
            uint32_t edx;
            
            asm volatile (
                "cpuid"
                :"=a"(max), "=b"(ebx), "=c"(ecx), "=d"(edx)
                : "0"(0));
            
            m_mask = -uint32_t(max > 0);
            
            if (max >= 1)
            {
                asm volatile (
                    "cpuid"
                    :"=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                    : "0"(1));
                
                m_features[0] = {eax, ebx, ecx, edx};
            }
            
            if (max >= 7)
            {
                asm volatile (
                    "cpuid"
                    :"=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                    : "0"(7), "2"(0));
                
                m_features[1] = {eax, ebx, ecx, edx};
            }
        }
        
        arch_x86_cpuid_basic(arch_x86_cpuid_basic&& a_object) noexcept(true) = default;
        
        arch_x86_cpuid_basic(const arch_x86_cpuid_basic& a_object) noexcept(true) = default;
        
        ~arch_x86_cpuid_basic() noexcept(true) = default;
    
    public:
        
        auto
        operator =(const arch_x86_cpuid_basic& object) noexcept(true) -> arch_x86_cpuid_basic& = default;
    
    public:
        
        auto
        sse_1(
        ) const noexcept(true) -> bool
        {
            return m_mask & m_features[0].edx & (1 << 25);
        }
        
        auto
        sse_2(
        ) const noexcept(true) -> bool
        {
            return m_mask & m_features[0].edx & (1 << 26);
        }
        
        auto
        sse_3(
        ) const noexcept(true) -> bool
        {
            return m_mask & m_features[0].ecx & (1 << 0);
        }
        
        auto
        ssse_3(
        ) const noexcept(true) -> bool
        {
            return m_mask & m_features[0].ecx & (1 << 9);
        }
        
        auto
        sse_4_1(
        ) const noexcept(true) -> bool
        {
            return m_mask & m_features[0].ecx & (1 << 19);
        }
        
        auto
        sse_4_2(
        ) const noexcept(true) -> bool
        {
            return m_mask & m_features[0].ecx & (1 << 20);
        }
        
        auto
        avx_1(
        ) const noexcept(true) -> bool
        {
            return m_mask & m_features[0].ecx & (1 << 28);
        }
        
        auto
        avx_2(
        ) const noexcept(true) -> bool
        {
            return m_mask & m_features[1].ebx & (1 << 5);
        }
        
        auto
        sha_1(
        ) const noexcept(true) -> bool
        {
            return m_mask & m_features[1].ebx & (1 << 29);
        }
        
        auto
        sha_2(
        ) const noexcept(true) -> bool
        {
            return m_mask & m_features[1].ebx & (1 << 29);
        }
    } arch_x86_cpuid;
    
    struct arch_x86_ise
    {
        inline
        static
        const bool sse_1 = arch_x86_cpuid.sse_1();
        
        inline
        static
        const bool sse_2 = arch_x86_cpuid.sse_2();
        
        inline
        static
        const bool sse_3 = arch_x86_cpuid.sse_3();
    
        inline
        static
        const bool ssse_3 = arch_x86_cpuid.ssse_3();
        
        inline
        static
        const bool sse_4_1 = arch_x86_cpuid.sse_4_1();
        
        inline
        static
        const bool sse_4_2 = arch_x86_cpuid.sse_4_2();
        
        inline
        static
        const bool sse_4 = arch_x86_cpuid.sse_4_1() && arch_x86_cpuid.sse_4_2();
        
        inline
        static
        const bool avx_1 = arch_x86_cpuid.avx_1();
        
        inline
        static
        const bool avx_2 = arch_x86_cpuid.avx_2();
        
        inline
        static
        const bool sha_1 = arch_x86_cpuid.sha_1();
        
        inline
        static
        const bool sha_2 = arch_x86_cpuid.sha_2();
        
        template<
            typename t_impl
        >
        static
        constexpr auto
        check(
        ) noexcept(true) -> bool
        {
            if constexpr (requires{
                typename t_impl::requirements::arch::x86;
            })
            {
                if constexpr (requires{
                    typename t_impl::requirements::arch::x86::ise;
                })
                {
                    using impl_type = typename t_impl::requirements::arch::x86::ise;
                    
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
    
    struct arch_x86
    {
        static
        constexpr bool value = true;
        
        using ise = arch_x86_ise;
    };
    
    #else
    
    struct arch_x86
    {
        static
        constexpr bool value = false;
        
        using ise = void;
    };
    
    #endif
}
