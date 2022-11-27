

#pragma once


#include <milo/common.h>


#if defined(__i386__) || \
    defined(__x86_64__) || \
    defined(_WIN32) || \
    defined(_WIN64)
    #define MILO_ARCH_X86 true
#else
    #define MILO_ARCH_X86 false;
#endif

/*
 * TODO
 *  This won't work for MSVC.
 */

#if MILO_ARCH_X86 && defined __SSE__
    #define MILO_ARCH_X86_ISE_SSE_1 true
#else
    #define MILO_ARCH_X86_ISE_SSE_1 false
#endif

#if MILO_ARCH_X86 && defined __SSE2__
    #define MILO_ARCH_X86_ISE_SSE_2 true
#else
    #define MILO_ARCH_X86_ISE_SSE_2 false
#endif

#if MILO_ARCH_X86 && defined __SSE3__
    #define MILO_ARCH_X86_ISE_SSE_3 true
#else
    #define MILO_ARCH_X86_ISE_SSE_3 false
#endif

#if MILO_ARCH_X86 && defined __SSSE3__
    #define MILO_ARCH_X86_ISE_SSSE_3 true
#else
    #define MILO_ARCH_X86_ISE_SSSE_3 false
#endif

#if MILO_ARCH_X86 && defined __SSE4_1__
    #define MILO_ARCH_X86_ISE_SSE_4_1 true
#else
    #define MILO_ARCH_X86_ISE_SSE_4_1 false
#endif

#if MILO_ARCH_X86 && defined __SSE4_2__
    #define MILO_ARCH_X86_ISE_SSE_4_2 true
#else
    #define MILO_ARCH_X86_ISE_SSE_4_2 false
#endif

#if MILO_ARCH_X86 && defined __AVX__
    #define MILO_ARCH_X86_ISE_AVX_1 true
#else
    #define MILO_ARCH_X86_ISE_AVX_1 false
#endif

#if MILO_ARCH_X86 && defined __AVX2__
    #define MILO_ARCH_X86_ISE_AVX_2 true
#else
    #define MILO_ARCH_X86_ISE_AVX_2 false
#endif

#if MILO_ARCH_X86 && defined __AVX2__
    #define MILO_ARCH_X86_ISE_AVX_2 true
#else
    #define MILO_ARCH_X86_ISE_AVX_2 false
#endif

#if MILO_ARCH_X86 && defined __SHA__
    #define MILO_ARCH_X86_ISE_SHA_1 true
#else
    #define MILO_ARCH_X86_ISE_SHA_1 false
#endif

#if MILO_ARCH_X86 && defined __SHA__
    #define MILO_ARCH_X86_ISE_SHA_2 true
#else
    #define MILO_ARCH_X86_ISE_SHA_2 false
#endif

namespace milo::detail
{
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
    
    template<
        typename t_mode,
        typename t_impl
    >
    static
    constexpr auto
    arch_x86_ise_check(
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
                using mode_type = t_mode;
                using impl_type = typename t_impl::requirements::arch::x86::ise;
    
                if constexpr (requires {
                    typename impl_type::sse_1;
                })
                {
                    if (!mode_type::sse_1)
                    {
                        return false;
                    }
                }
    
                if constexpr (requires {
                    typename impl_type::sse_2;
                })
                {
                    if (!mode_type::sse_2)
                    {
                        return false;
                    }
                }
    
                if constexpr (requires {
                    typename impl_type::sse_3;
                })
                {
                    if (!mode_type::sse_3)
                    {
                        return false;
                    }
                }
    
                if constexpr (requires {
                    typename impl_type::ssse_3;
                })
                {
                    if (!mode_type::ssse_3)
                    {
                        return false;
                    }
                }
    
                if constexpr (requires {
                    typename impl_type::sse_4_1;
                })
                {
                    if (!mode_type::sse_4_1)
                    {
                        return false;
                    }
                }
    
                if constexpr (requires {
                    typename impl_type::sse_4_2;
                })
                {
                    if (!mode_type::sse_4_2)
                    {
                        return false;
                    }
                }
    
                if constexpr (requires {
                    typename impl_type::avx_1;
                })
                {
                    if (!mode_type::avx_1)
                    {
                        return false;
                    }
                }
    
                if constexpr (requires {
                    typename impl_type::avx_2;
                })
                {
                    if (!mode_type::avx_2)
                    {
                        return false;
                    }
                }
    
                if constexpr (requires {
                    typename impl_type::sha_1;
                })
                {
                    if (!mode_type::sha_1)
                    {
                        return false;
                    }
                }
    
                if constexpr (requires {
                    typename impl_type::sha_2;
                })
                {
                    if (!mode_type::sha_2)
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
    
    struct arch_x86_ise_strict
        : private common::disabled
    {
        static
        constexpr bool sse_1 = false;
        
        static
        constexpr bool sse_2 = false;
        
        static
        constexpr bool sse_3 = false;
        
        static
        constexpr bool ssse_3 = false;
        
        static
        constexpr bool sse_4_1 = false;
        
        static
        constexpr bool sse_4_2 = false;
        
        static
        constexpr bool avx_1 = false;
        
        static
        constexpr bool avx_2 = false;
        
        static
        constexpr bool sha_1 = false;
        
        static
        constexpr bool sha_2 = false;
        
        template<
            typename t_impl
        >
        static
        constexpr auto
        check(
        ) noexcept(true) -> bool
        {
            return arch_x86_ise_check<arch_x86_ise_strict, t_impl>();
        }
    };
    
    struct arch_x86_ise_native
        : private common::disabled
    {
        static
        constexpr bool sse_1 = MILO_ARCH_X86_ISE_SSE_1;
        
        static
        constexpr bool sse_2 = MILO_ARCH_X86_ISE_SSE_2;
        
        static
        constexpr bool sse_3 = MILO_ARCH_X86_ISE_SSE_3;
        
        static
        constexpr bool ssse_3 = MILO_ARCH_X86_ISE_SSSE_3;
        
        static
        constexpr bool sse_4_1 = MILO_ARCH_X86_ISE_SSE_4_1;
        
        static
        constexpr bool sse_4_2 = MILO_ARCH_X86_ISE_SSE_4_2;
        
        static
        constexpr bool avx_1 = MILO_ARCH_X86_ISE_AVX_1;
        
        static
        constexpr bool avx_2 = MILO_ARCH_X86_ISE_AVX_2;
        
        static
        constexpr bool sha_1 = MILO_ARCH_X86_ISE_SHA_1;
        
        static
        constexpr bool sha_2 = MILO_ARCH_X86_ISE_SHA_2;
        
        template<
            typename t_impl
        >
        static
        constexpr auto
        check(
        ) noexcept(true) -> bool
        {
            return arch_x86_ise_check<arch_x86_ise_native, t_impl>();
        }
    };
    
    struct arch_x86_ise_runtime
        : private common::disabled
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
            return arch_x86_ise_check<arch_x86_ise_runtime, t_impl>();
        }
    };
    
    struct arch_x86_ise
        : private common::disabled
    {
        using strict = arch_x86_ise_strict;
        
        using native = arch_x86_ise_native;
        
        using runtime = arch_x86_ise_runtime;
    };
    
    struct arch_x86_impl
        : private common::disabled
    {
        static
        constexpr bool value = MILO_ARCH_X86;
        
        using ise = arch_x86_ise;
    };
}
