

#pragma once


#include <new>

#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/traits.h>

#include <milo/detail/bit.h>
#include <milo/detail/endian.h>


namespace milo::detail
{
    template<
        concepts::integral t_val,
        concepts::integral t_ptr
    >
    constexpr auto
    memory_load(
        const t_ptr* a_ptr,
        size_t a_off
    ) noexcept(true) -> t_val
    {
        using des_type = t_val;
        using job_type = t_ptr;
        using val_type = traits::to_integral_unsigned_t<des_type>;
        using ptr_type = traits::to_integral_unsigned_t<job_type>;
        
        constexpr auto val_size = sizeof(val_type);
        constexpr auto ptr_size = sizeof(ptr_type);
        constexpr auto val_bits = bit_width<val_type>();
        constexpr auto ptr_bits = bit_width<ptr_type>();
        
        constexpr auto full_size = val_size / ptr_size;
        constexpr auto part_size = val_size % ptr_size;
        
        static_assert(
            !(full_size == 0 && part_size == 0) &&
            !(full_size != 0 && part_size != 0)
        );
        
        if constexpr (val_size == ptr_size)
        {
            return des_type(a_ptr[a_off]);
        }
        else
        {
            if MILO_CONSTEVAL
            {
                auto val = val_type(0);
                auto bse = a_off * val_size;
                auto seg = bse / ptr_size;
                auto off = bse % ptr_size;
                
                if constexpr (part_size > 0)
                {
                    auto bits = bit_width(off);
                    
                    if constexpr (endian_big)
                    {
                        bits = ptr_bits - bits - val_bits;
                    }
                    
                    val = val_type(ptr_type(a_ptr[seg]) >> bits);
                }
                
                if constexpr (full_size > 0)
                {
                    constexpr auto todo_size = full_size;
                    constexpr auto i_beg = endian_little ? todo_size - 1 : 0;
                    constexpr auto i_end = endian_little ? size_t(-1) : todo_size;
                    
                    for (size_t i = i_beg; i != i_end;)
                    {
                        val <<= ptr_bits;
                        val |= val_type(a_ptr[seg + i]) & val_type(ptr_type(-1));
                        
                        if constexpr (endian_little)
                        {
                            i -= 1;
                        }
                        
                        if constexpr (endian_big)
                        {
                            i += 1;
                        }
                    }
                }
                
                return des_type(val);
            }
            else
            {
                val_type val;
                
                std::memcpy(
                    reinterpret_cast<uint8_t*>(&val),
                    reinterpret_cast<const val_type*>(a_ptr) + a_off,
                    val_size
                );
                
                return des_type(val);
            }
        }
    }
    
    template<
        concepts::integral t_val,
        concepts::integral t_ptr
    >
    constexpr auto
    memory_load(
        const t_ptr* a_ptr,
        size_t a_off,
        size_t a_size
    ) noexcept(true) -> t_val
    {
        using des_type = t_val;
        using job_type = t_ptr;
        using val_type = traits::to_integral_unsigned_t<des_type>;
        using ptr_type = traits::to_integral_unsigned_t<job_type>;
        
        constexpr auto val_size = sizeof(val_type);
        constexpr auto ptr_size = sizeof(ptr_type);
        constexpr auto val_bits = bit_width<val_type>();
        constexpr auto ptr_bits = bit_width<ptr_type>();
        
        constexpr auto full_size = val_size / ptr_size;
        constexpr auto part_size = val_size % ptr_size;
        
        static_assert(
            !(full_size == 0 && part_size == 0) &&
            !(full_size != 0 && part_size != 0)
        );
        
        if MILO_CONSTEVAL
        {
            auto val = val_type(0);
            auto bse = a_off * val_size;
            auto seg = bse / ptr_size;
            auto off = bse % ptr_size;
            
            if (a_size == 0)
            {
                return des_type(0);
            }
            
            if constexpr (val_size == ptr_size)
            {
                return des_type(a_ptr[a_off]);
            }
            
            if constexpr (part_size > 0 && val_size != ptr_size)
            {
                auto bits = bit_width(off);
                
                if constexpr (endian_big)
                {
                    bits = ptr_bits - bits - val_bits;
                }
                
                val = val_type(ptr_type(a_ptr[seg]) >> bits);
            }
            
            if constexpr (full_size > 0 && val_size != ptr_size)
            {
                auto todo_size = common::min(
                    a_size,
                    full_size
                );
                auto i_beg = endian_little ? todo_size - 1 : 0;
                auto i_end = endian_little ? size_t(-1) : todo_size;
                
                for (size_t i = i_beg; i != i_end;)
                {
                    val <<= ptr_bits;
                    val |= val_type(a_ptr[seg + i]) & val_type(ptr_type(-1));
                    
                    if constexpr (endian_little)
                    {
                        i -= 1;
                    }
                    
                    if constexpr (endian_big)
                    {
                        i += 1;
                    }
                }
                
                if constexpr (endian_big)
                {
                    val <<= bit_width(ptr_size * (full_size - todo_size));
                }
            }
            
            return des_type(val);
        }
        else
        {
            auto val = val_type(0);
            
            std::memcpy(
                reinterpret_cast<uint8_t*>(&val),
                reinterpret_cast<const val_type*>(a_ptr) + a_off,
                common::min(
                    val_size,
                    ptr_size * a_size
                )
            );
            
            return des_type(val);
        }
    }
    
    template<
        concepts::integral t_val,
        concepts::integral t_ptr
    >
    constexpr auto
    memory_load_be(
        const t_ptr* a_ptr,
        size_t a_off
    ) noexcept(true) -> t_val
    {
        return endian_bigof(
            memory_load<
                t_val,
                t_ptr
            >(
                a_ptr,
                a_off
            )
        );
    }
    
    template<
        concepts::integral t_val,
        concepts::integral t_ptr
    >
    constexpr auto
    memory_load_be(
        const t_ptr* a_ptr,
        size_t a_off,
        size_t a_size
    ) noexcept(true) -> t_val
    {
        return endian_bigof(
            memory_load<
                t_val,
                t_ptr
            >(
                a_ptr,
                a_off,
                a_size
            )
        );
    }
    
    template<
        concepts::integral t_val,
        concepts::integral t_ptr
    >
    constexpr auto
    memory_load_le(
        const t_ptr* a_ptr,
        size_t a_off
    ) noexcept(true) -> t_val
    {
        return endian_littleof(
            memory_load<
                t_val,
                t_ptr
            >(
                a_ptr,
                a_off
            )
        );
    }
    
    template<
        concepts::integral t_val,
        concepts::integral t_ptr
    >
    constexpr auto
    memory_load_le(
        const t_ptr* a_ptr,
        size_t a_off,
        size_t a_size
    ) noexcept(true) -> t_val
    {
        return endian_littleof(
            memory_load<
                t_val,
                t_ptr
            >(
                a_ptr,
                a_off,
                a_size
            )
        );
    }
    
    template<
        concepts::integral t_val,
        concepts::integral t_ptr
    >
    constexpr auto
    memory_stor(
        t_ptr* a_ptr,
        size_t a_off,
        t_val a_val
    ) noexcept(true) -> void
    {
        using des_type = t_val;
        using job_type = t_ptr;
        using val_type = traits::to_integral_unsigned_t<des_type>;
        using ptr_type = traits::to_integral_unsigned_t<job_type>;
        
        constexpr auto val_size = sizeof(val_type);
        constexpr auto ptr_size = sizeof(ptr_type);
        constexpr auto val_bits = bit_width<val_type>();
        constexpr auto ptr_bits = bit_width<ptr_type>();
        
        constexpr auto full_size = val_size / ptr_size;
        constexpr auto part_size = val_size % ptr_size;
        
        static_assert(
            !(full_size == 0 && part_size == 0) &&
            !(full_size != 0 && part_size != 0)
        );
        
        if constexpr (val_size == ptr_size)
        {
            a_ptr[a_off] = job_type(a_val);
        }
        else
        {
            if MILO_CONSTEVAL
            {
                auto val = val_type(a_val);
                auto bse = a_off * val_size;
                auto seg = bse / ptr_size;
                auto off = bse % ptr_size;
                
                if constexpr (part_size > 0)
                {
                    auto bits = bit_width(off);
                    
                    if constexpr (endian_big)
                    {
                        bits = ptr_bits - bits - val_bits;
                    }
                    
                    a_ptr[seg] = job_type((a_ptr[seg] & ~(ptr_type(val_type(-1)) << bits)) | (ptr_type(val) << bits));
                }
                
                if constexpr (full_size > 0)
                {
                    constexpr auto todo_size = full_size;
                    constexpr auto i_beg = endian_little ? 0 : todo_size - 1;
                    constexpr auto i_end = endian_little ? todo_size : size_t(-1);
                    
                    for (size_t i = i_beg; i != i_end;)
                    {
                        a_ptr[seg + i] = job_type(val);
                        val >>= ptr_bits;
                        
                        if constexpr (endian_little)
                        {
                            i += 1;
                        }
                        
                        if constexpr (endian_big)
                        {
                            i -= 1;
                        }
                    }
                }
            }
            else
            {
                std::memcpy(
                    reinterpret_cast<val_type*>(a_ptr) + a_off,
                    reinterpret_cast<const uint8_t*>(&a_val),
                    val_size
                );
            }
        }
    }
    
    template<
        concepts::integral t_val,
        concepts::integral t_ptr
    >
    constexpr auto
    memory_stor(
        t_ptr* a_ptr,
        size_t a_off,
        t_val a_val,
        size_t a_size
    ) noexcept(true) -> void
    {
        using des_type = t_val;
        using job_type = t_ptr;
        using val_type = traits::to_integral_unsigned_t<des_type>;
        using ptr_type = traits::to_integral_unsigned_t<job_type>;
        
        constexpr auto val_size = sizeof(val_type);
        constexpr auto ptr_size = sizeof(ptr_type);
        constexpr auto val_bits = bit_width<val_type>();
        constexpr auto ptr_bits = bit_width<ptr_type>();
        
        constexpr auto full_size = val_size / ptr_size;
        constexpr auto part_size = val_size % ptr_size;
        
        static_assert(
            !(full_size == 0 && part_size == 0) &&
            !(full_size != 0 && part_size != 0)
        );
        
        if MILO_CONSTEVAL
        {
            auto val = val_type(a_val);
            auto bse = a_off * val_size;
            auto seg = bse / ptr_size;
            auto off = bse % ptr_size;
            
            if (a_size == 0)
            {
                return;
            }
            
            if constexpr (val_size == ptr_size)
            {
                a_ptr[a_off] = job_type(val);
                
                return;
            }
            
            if constexpr (part_size > 0 && val_size != ptr_size)
            {
                auto bits = bit_width(off);
                
                if constexpr (endian_big)
                {
                    bits = ptr_bits - bits - val_bits;
                }
                
                a_ptr[seg] = job_type((a_ptr[seg] & ~(ptr_type(val_type(-1)) << bits)) | (ptr_type(val) << bits));
            }
            
            if constexpr (full_size > 0 && val_size != ptr_size)
            {
                auto todo_size = common::min(
                    a_size,
                    full_size
                );
                auto i_beg = endian_little ? 0 : todo_size - 1;
                auto i_end = endian_little ? todo_size : size_t(-1);
                
                if constexpr (endian_big)
                {
                    val >>= bit_width(ptr_size * (full_size - todo_size));
                }
                
                for (size_t i = i_beg; i != i_end;)
                {
                    a_ptr[seg + i] = job_type(val);
                    val >>= ptr_bits;
                    
                    if constexpr (endian_little)
                    {
                        i += 1;
                    }
                    
                    if constexpr (endian_big)
                    {
                        i -= 1;
                    }
                }
            }
        }
        else
        {
            std::memcpy(
                reinterpret_cast<val_type*>(a_ptr) + a_off,
                reinterpret_cast<const uint8_t*>(&a_val),
                common::min(
                    val_size,
                    ptr_size * a_size
                )
            );
        }
    }
    
    template<
        concepts::integral t_val,
        concepts::integral t_ptr
    >
    constexpr auto
    memory_stor_be(
        t_ptr* a_ptr,
        size_t a_off,
        t_val a_val
    ) noexcept(true) -> void
    {
        memory_stor(
            a_ptr,
            a_off,
            endian_bigof(
                a_val
            )
        );
    }
    
    template<
        concepts::integral t_val,
        concepts::integral t_ptr
    >
    constexpr auto
    memory_stor_be(
        t_ptr* a_ptr,
        size_t a_off,
        t_val a_val,
        size_t a_size
    ) noexcept(true) -> void
    {
        memory_stor(
            a_ptr,
            a_off,
            endian_bigof(
                a_val
            ),
            a_size
        );
    }
    
    template<
        concepts::integral t_val,
        concepts::integral t_ptr
    >
    constexpr auto
    memory_stor_le(
        t_ptr* a_ptr,
        size_t a_off,
        t_val a_val
    ) noexcept(true) -> void
    {
        memory_stor(
            a_ptr,
            a_off,
            endian_littleof(
                a_val
            )
        );
    }
    
    template<
        concepts::integral t_val,
        concepts::integral t_ptr
    >
    constexpr auto
    memory_stor_le(
        t_ptr* a_ptr,
        size_t a_off,
        t_val a_val,
        size_t a_size
    ) noexcept(true) -> void
    {
        memory_stor(
            a_ptr,
            a_off,
            endian_littleof(
                a_val
            ),
            a_size
        );
    }
    
    template<
        concepts::integral t_dst,
        concepts::integral t_src
    >
    constexpr auto
    memory_copy(
        t_dst* a_dst_ptr,
        const t_src* a_src_ptr,
        size_t a_size
    ) noexcept(true) -> void
    {
        if MILO_CONSTEVAL
        {
            for (size_t i = 0; i < a_size; i += 1)
            {
                memory_stor<uint8_t>(
                    a_dst_ptr,
                    i,
                    memory_load<uint8_t>(
                        a_src_ptr,
                        i
                    )
                );
            }
        }
        else
        {
            std::memcpy(
                reinterpret_cast<uint8_t*>(a_dst_ptr),
                reinterpret_cast<const uint8_t*>(a_src_ptr),
                a_size
            );
        }
    }
    
    template<
        concepts::integral t_dst,
        concepts::integral t_src
    >
    constexpr auto
    memory_copy_be(
        t_dst* a_dst_ptr,
        const t_src* a_src_ptr,
        size_t a_size
    ) noexcept(true) -> void
    {
        auto full_size = a_size / sizeof(t_src);
        auto last_size = a_size % sizeof(t_src);
        
        for (size_t i = 0; i < full_size; i += 1)
        {
            memory_stor_be<t_src>(
                a_dst_ptr,
                i,
                a_src_ptr[i]
            );
        }
        
        if (last_size > 0)
        {
            memory_stor_be<t_src>(
                a_dst_ptr,
                full_size,
                a_src_ptr[full_size],
                last_size
            );
        }
    }
    
    template<
        concepts::integral t_dst,
        concepts::integral t_src
    >
    constexpr auto
    memory_copy_le(
        t_dst* a_dst_ptr,
        const t_src* a_src_ptr,
        size_t a_size
    ) noexcept(true) -> void
    {
        auto full_size = a_size / sizeof(t_src);
        auto last_size = a_size % sizeof(t_src);
        
        for (size_t i = 0; i < full_size; i += 1)
        {
            memory_stor_le<t_src>(
                a_dst_ptr,
                i,
                a_src_ptr[i]
            );
        }
        
        if (last_size > 0)
        {
            memory_stor_le<t_src>(
                a_dst_ptr,
                full_size,
                a_src_ptr[full_size],
                last_size
            );
        }
    }
    
    template<
        concepts::integral t_ptr,
        concepts::integral t_val
    >
    constexpr auto
    memory_set(
        t_ptr* a_ptr,
        t_val a_val,
        size_t a_size
    ) noexcept(true) -> void
    {
        if MILO_CONSTEVAL
        {
            for (size_t i = 0; i < a_size; i += 1)
            {
                memory_stor<uint8_t>(
                    a_ptr,
                    i,
                    a_val
                );
            }
        }
        else
        {
            std::memset(
                a_ptr,
                a_val,
                a_size
            );
        }
    }
    
    template<
        concepts::integral t_dst,
        concepts::integral t_src_0,
        concepts::integral t_src_1
    >
    constexpr auto
    memory_xor(
        t_dst* a_dst_ptr,
        const t_src_0* a_src_0_ptr,
        const t_src_1* a_src_1_ptr,
        size_t a_size
    ) noexcept(true) -> void
    {
        for (size_t i = 0; i < a_size; i += 1)
        {
            memory_stor<uint8_t>(
                a_dst_ptr,
                i,
                memory_load<uint8_t>(
                    a_src_0_ptr,
                    i
                ) ^ memory_load<uint8_t>(
                    a_src_1_ptr,
                    i
                )
            );
        }
    }
    
    template<
        concepts::integral t_src_a,
        concepts::integral t_src_b
    >
    constexpr auto
    memory_equal(
        const t_src_a* a_src_a_ptr,
        const t_src_b* a_src_b_ptr,
        size_t a_size
    ) noexcept(true) -> bool
    {
        if MILO_CONSTEVAL
        {
            for (size_t i = 0; i < a_size; i += 1)
            {
                if (memory_load<uint8_t>(
                    a_src_a_ptr,
                    i
                ) != memory_load<uint8_t>(
                    a_src_b_ptr,
                    i
                ))
                {
                    return false;
                }
            }
            
            return true;
        }
        else
        {
            return std::memcmp(
                a_src_a_ptr,
                a_src_b_ptr,
                a_size
            ) == 0;
        }
    }
    
    template<
        concepts::container t_container_a,
        concepts::container t_container_b
    >
    constexpr auto
    memory_equal(
        const t_container_a& a_container_a,
        const t_container_b& a_container_b
    ) noexcept(true) -> bool
    requires
    requires
    {
        requires concepts::integral<typename t_container_a::value_type>;
        requires concepts::integral<typename t_container_b::value_type>;
        requires concepts::convertible<typename t_container_a::value_type, typename t_container_b::value_type>;
    }
    {
        return
            (
                a_container_a.size() * sizeof(typename t_container_a::value_type) ==
                a_container_b.size() * sizeof(typename t_container_b::value_type)
            ) &&
            memory_equal(
                a_container_a.data(),
                a_container_b.data(),
                a_container_b.size() * sizeof(typename t_container_b::value_type)
            );
    }
    
    template<
        concepts::integral t_src_a,
        concepts::integral t_src_b
    >
    constexpr auto
    memory_match(
        const t_src_a* a_src_a_ptr,
        const t_src_b* a_src_b_ptr,
        size_t a_size
    ) noexcept(true) -> bool
    {
        uint8_t tmp = 0;
        
        for (size_t i = 0; i < a_size; i += 1)
        {
            tmp |= memory_load<uint8_t>(
                a_src_a_ptr,
                i
            ) ^ memory_load<uint8_t>(
                a_src_b_ptr,
                i
            );
        }
        
        return tmp == 0;
    }
    
    template<
        concepts::container t_container_a,
        concepts::container t_container_b
    >
    constexpr auto
    memory_match(
        const t_container_a& a_container_a,
        const t_container_b& a_container_b
    ) noexcept(true) -> bool
    requires
    requires
    {
        requires concepts::integral<typename t_container_a::value_type>;
        requires concepts::integral<typename t_container_b::value_type>;
        requires concepts::convertible<typename t_container_a::value_type, typename t_container_b::value_type>;
    }
    {
        return
            bool(
                size_t(
                    a_container_a.size() * sizeof(typename t_container_a::value_type) ==
                    a_container_b.size() * sizeof(typename t_container_b::value_type)
                ) &
                size_t(
                    memory_match(
                        a_container_a.data(),
                        a_container_b.data(),
                        common::min(
                            a_container_a.size() * sizeof(typename t_container_a::value_type),
                            a_container_b.size() * sizeof(typename t_container_b::value_type)
                        )
                    )
                )
            );
    }
    
    template<
        concepts::integral t_ptr
    >
    constexpr auto
    memory_zero(
        t_ptr* a_ptr,
        size_t a_size
    ) noexcept(true) -> void
    {
        for (size_t i = 0; i < a_size; i += 1)
        {
            a_ptr[i] = 0;
        }
    }
    
    template<
        concepts::integral t_ptr,
        size_t t_size
    >
    constexpr auto
    memory_zero(
        t_ptr(& a_ptr)[t_size]
    ) noexcept(true) -> void
    {
        memory_zero(
            a_ptr,
            t_size
        );
    }
    
    template<
        concepts::integral t_ptr
    >
    constexpr auto
    memory_erase(
        t_ptr* a_ptr,
        size_t a_size
    ) noexcept(true) -> void
    {
        if MILO_CONSTEVAL
        {
            auto ptr = a_ptr;
            
            for (size_t i = 0; i < a_size; i += 1)
            {
                ptr[i] = 0;
            }
        }
        else
        {
            auto ptr = static_cast<volatile t_ptr*>(a_ptr);
            
            for (size_t i = 0; i < a_size; i += 1)
            {
                ptr[i] = 0;
            }
        }
    }
    
    template<
        concepts::integral t_ref
    >
    constexpr auto
    memory_erase(
        t_ref& a_ref
    ) noexcept(true) -> void
    {
        if MILO_CONSTEVAL
        {
            static_cast<t_ref&>(a_ref) = 0;
        }
        else
        {
            static_cast<volatile t_ref&>(a_ref) = 0;
        }
    }
    
    template<
        typename t_ptr,
        size_t t_size
    >
    constexpr auto
    memory_erase(
        t_ptr(& a_ptr)[t_size]
    ) noexcept(true) -> void
    {
        memory_erase(
            a_ptr,
            t_size
        );
    }
    
    template<
        concepts::integral t_dst,
        concepts::integral t_src
    >
    constexpr auto
    memory_init(
        t_dst* a_dst_ptr,
        size_t a_dst_size,
        const t_src* a_src_ptr,
        size_t a_src_size
    ) noexcept(true) -> void
    {
        memory_zero(
            a_dst_ptr,
            a_dst_size
        );
        
        memory_copy(
            a_dst_ptr,
            a_src_ptr,
            common::min(
                a_dst_size * sizeof(t_dst),
                a_src_size * sizeof(t_src)
            )
        );
    }
    
    template<
        concepts::integral t_dst,
        concepts::integral t_src,
        size_t t_dst_size
    >
    constexpr auto
    memory_init(
        t_dst (& a_dst_ptr)[t_dst_size],
        const t_src* a_src_ptr,
        size_t a_src_size
    ) noexcept(true) -> void
    {
        memory_init(
            a_dst_ptr,
            t_dst_size,
            a_src_ptr,
            a_src_size
        );
    }
    
    template<
        concepts::integral t_dst,
        concepts::integral t_src
    >
    constexpr auto
    memory_init_be(
        t_dst* a_dst_ptr,
        size_t a_dst_size,
        const t_src* a_src_ptr,
        size_t a_src_size
    ) noexcept(true) -> void
    {
        memory_init(
            a_dst_ptr,
            a_dst_size,
            a_src_ptr,
            a_src_size
        );
        
        if constexpr (sizeof(t_dst) != 1 && !endian_big)
        {
            for (size_t i = 0; i < a_dst_size; i += 1)
            {
                a_dst_ptr[i] = endian_bigof(
                    a_dst_ptr[i]
                );
            }
        }
    }
    
    template<
        concepts::integral t_dst,
        concepts::integral t_src,
        size_t t_dst_size
    >
    constexpr auto
    memory_init_be(
        t_dst (& a_dst_ptr)[t_dst_size],
        const t_src* a_src_ptr,
        size_t a_src_size
    ) noexcept(true) -> void
    {
        memory_init_be(
            a_dst_ptr,
            t_dst_size,
            a_src_ptr,
            a_src_size
        );
    }
    
    template<
        concepts::integral t_dst,
        concepts::integral t_src
    >
    constexpr auto
    memory_init_le(
        t_dst* a_dst_ptr,
        size_t a_dst_size,
        const t_src* a_src_ptr,
        size_t a_src_size
    ) noexcept(true) -> void
    {
        memory_init(
            a_dst_ptr,
            a_dst_size,
            a_src_ptr,
            a_src_size
        );
        
        if constexpr (sizeof(t_dst) != 1 && !endian_little)
        {
            for (size_t i = 0; i < a_dst_size; i += 1)
            {
                a_dst_ptr[i] = endian_littleof(
                    a_dst_ptr[i]
                );
            }
        }
    }
    
    template<
        concepts::integral t_dst,
        concepts::integral t_src,
        size_t t_dst_size
    >
    constexpr auto
    memory_init_le(
        t_dst (& a_dst_ptr)[t_dst_size],
        const t_src* a_src_ptr,
        size_t a_src_size
    ) noexcept(true) -> void
    {
        memory_init_le(
            a_dst_ptr,
            t_dst_size,
            a_src_ptr,
            a_src_size
        );
    }
    
    class memory_bytes_const_view
    {
    public:
        
        using value_type = unsigned char;
    
    private:
        
        const value_type* m_ptr;
        
        size_t m_size;
    
    public:
        
        constexpr memory_bytes_const_view() noexcept(true)
        {
            m_ptr = nullptr;
            m_size = 0;
        }
        
        constexpr memory_bytes_const_view(memory_bytes_const_view&& a_object) noexcept(true)
        {
            m_ptr = a_object.m_ptr;
            m_size = a_object.m_size;
            
            if MILO_CONSTEVAL
            {
                a_object.m_ptr = nullptr;
                a_object.m_size = 0;
            }
        }
        
        constexpr memory_bytes_const_view(const memory_bytes_const_view& a_object) noexcept(true)
        {
            if MILO_CONSTEVAL
            {
                auto size = a_object.m_size;
                auto ptr = new value_type[size];
                
                for (size_t i = 0; i < size; i += 1)
                {
                    ptr[i] = value_type(a_object.m_ptr[i]);
                }
                
                m_ptr = ptr;
                m_size = size;
            }
            else
            {
                m_ptr = a_object.m_ptr;
                m_size = a_object.m_size;
            }
        }
        
        constexpr ~memory_bytes_const_view() noexcept(true)
        {
            if MILO_CONSTEVAL
            {
                delete[] m_ptr;
            }
        }
    
    public:
        
        template<
            concepts::byte t_ptr
        >
        constexpr
        memory_bytes_const_view(
            const t_ptr* a_ptr,
            size_t a_size
        ) noexcept(true)
        {
            if MILO_CONSTEVAL
            {
                auto size = a_size;
                auto ptr = new value_type[size];
                
                for (size_t i = 0; i < size; i += 1)
                {
                    ptr[i] = value_type(a_ptr[i]);
                }
                
                m_ptr = ptr;
                m_size = size;
            }
            else
            {
                m_ptr = reinterpret_cast<const unsigned char*>(a_ptr);
                m_size = a_size;
            }
        }
    
    public:
        
        constexpr auto
        operator =(
            memory_bytes_const_view&& a_object
        ) noexcept(true) -> memory_bytes_const_view&
        {
            if MILO_CONSTEVAL
            {
                if (this != &a_object)
                {
                    delete[] m_ptr;
                    
                    m_ptr = a_object.m_ptr;
                    m_size = a_object.m_size;
                    a_object.m_ptr = nullptr;
                    a_object.m_size = 0;
                }
            }
            else
            {
                m_ptr = a_object.m_ptr;
                m_size = a_object.m_size;
            }
            
            return *this;
        }
        
        constexpr auto
        operator =(
            const memory_bytes_const_view& a_object
        ) noexcept(true) -> memory_bytes_const_view&
        {
            if MILO_CONSTEVAL
            {
                if (this != &a_object)
                {
                    delete[] m_ptr;
                    
                    auto size = a_object.m_size;
                    auto ptr = new value_type[size];
                    
                    for (size_t i = 0; i < size; i += 1)
                    {
                        ptr[i] = value_type(a_object.m_ptr[i]);
                    }
                    
                    m_ptr = ptr;
                    m_size = size;
                }
            }
            else
            {
                m_ptr = a_object.m_ptr;
                m_size = a_object.m_size;
            }
            
            return *this;
        }
    
    public:
        
        constexpr auto
        data() const noexcept(true) -> const value_type*
        {
            return m_ptr;
        }
        
        constexpr auto
        size() const noexcept(true) -> size_t
        {
            return m_size;
        }
    };
}
