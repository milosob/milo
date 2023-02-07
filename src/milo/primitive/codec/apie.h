

#pragma once


#include <milo/internal.h>


namespace milo::primitive::codec
{
    /**
     * Codec wrapper that provides access to additional APIs.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        meta::primitive::codec t_impl
    >
    class apie
    {
    public:
        
        using impl_type = t_impl;
    
    public:
        
        static
        constexpr auto ratio_bytes_size = impl_type::ratio_bytes_size;
        
        static
        constexpr auto ratio_chars_size = impl_type::ratio_chars_size;
    
    public:
        
        /**
         * This function encodes bytes.
         *
         * @tparam t_from
         * From type.
         * @tparam t_to
         * To type.
         * @param a_from_ptr
         * From pointer.
         * @param a_from_size
         * From size.
         * @param a_to_ptr
         * To pointer.
         * @return
         * To size.
         */
        template<
            meta::byte t_from,
            meta::byte t_to
        >
        static
        constexpr auto
        encode(
            const t_from* a_from_ptr,
            size_t a_from_size,
            t_to* a_to_ptr
        ) noexcept(true) -> size_t
        {
            return impl_type::encode(
                a_from_ptr,
                a_from_size,
                a_to_ptr
            );
        }
        
        /**
         * This function encodes bytes.
         *
         * @tparam t_from
         * From type.
         * @tparam t_to
         * To type.
         * @param a_from
         * From.
         * @param a_to
         * To.
         * @return
         * To size.
         */
        template<
            meta::bytes t_from,
            meta::bytes t_to
        >
        static
        constexpr auto
        encode(
            const t_from& a_from,
            t_to& a_to
        ) noexcept(meta::container_static<t_to>) -> size_t
        {
            internal::resize(
                a_to,
                encode_size(a_from)
            );
            
            auto size = encode(
                a_from.data(),
                a_from.size(),
                a_to.data()
            );
            
            internal::resize(
                a_to,
                size
            );
            
            return size;
        }
        
        /**
         * This function encodes bytes.
         *
         * @tparam t_to
         * To type.
         * @tparam t_from
         * From type.
         * @param a_from
         * From.
         * @return
         * To.
         */
        template<
            meta::bytes t_to,
            meta::bytes t_from
        >
        static
        constexpr auto
        encode(
            const t_from& a_from
        ) noexcept(false) -> t_to
        requires
        requires
        {
            requires meta::container_dynamic<t_to>;
        }
        {
            t_to result;
            
            encode(
                a_from,
                result
            );
            
            return result;
        }
        
        /**
         * This function calculates minimum bytes size after encoding.
         *
         * @tparam t_from
         * From type.
         * @param a_from_ptr
         * From pointer.
         * @param a_from_size
         * From size.
         * @return
         * To size.
         */
        template<
            meta::byte t_from
        >
        static
        constexpr auto
        encode_size(
            const t_from* a_from_ptr,
            size_t a_from_size
        ) noexcept(true) -> size_t
        {
            return impl_type::encode_size(
                a_from_ptr,
                a_from_size
            );
        }
        
        /**
         * This function calculates minimum bytes size after encoding.
         *
         * @tparam t_from
         * From type.
         * @param a_from
         * From.
         * @return
         * To size.
         */
        template<
            meta::bytes t_from
        >
        static
        constexpr auto
        encode_size(
            const t_from& a_from
        ) noexcept(true) -> size_t
        {
            return encode_size(
                a_from.data(),
                a_from.size()
            );
        }
        
        /**
         * This function decodes bytes.
         *
         * @tparam t_from
         * From type.
         * @tparam t_to
         * To type.
         * @param a_from_ptr
         * From pointer.
         * @param a_from_size
         * From size.
         * @param a_to_ptr
         * To pointer.
         * @param a_error
         * Error.
         * @return
         * To size.
         */
        template<
            meta::byte t_from,
            meta::byte t_to
        >
        static
        constexpr auto
        decode(
            const t_from* a_from_ptr,
            size_t a_from_size,
            t_to* a_to_ptr,
            error& a_error
        ) noexcept(true) -> size_t
        {
            return impl_type::decode(
                a_from_ptr,
                a_from_size,
                a_to_ptr,
                a_error
            );
        }
        
        /**
         * This function decodes bytes.
         *
         * @tparam t_from
         * From type.
         * @tparam t_to
         * To type.
         * @param a_from_ptr
         * From pointer.
         * @param a_from_size
         * From size.
         * @param a_to_ptr
         * To pointer.
         * @return
         * To size.
         */
        template<
            meta::byte t_from,
            meta::byte t_to
        >
        static
        constexpr auto
        decode(
            const t_from* a_from_ptr,
            size_t a_from_size,
            t_to* a_to_ptr
        ) noexcept(false) -> size_t
        {
            error error;
            
            auto size = decode(
                a_from_ptr,
                a_from_size,
                a_to_ptr,
                error
            );
            
            [[unlikely]]
            if (error)
            {
                throw error;
            }
            
            return size;
        }
        
        /**
         * This function decodes bytes.
         *
         * @tparam t_from
         * From type.
         * @tparam t_to
         * To type.
         * @param a_from
         * From.
         * @param a_to
         * To.
         * @param a_error
         * Error.
         * @return
         * To size.
         */
        template<
            meta::bytes t_from,
            meta::bytes t_to
        >
        static
        constexpr auto
        decode(
            const t_from& a_from,
            t_to& a_to,
            error& a_error
        ) noexcept(meta::container_static<t_to>) -> size_t
        {
            internal::resize(
                a_to,
                decode_size(a_from)
            );
            
            auto size = decode(
                a_from.data(),
                a_from.size(),
                a_to.data(),
                a_error
            );
            
            internal::resize(
                a_to,
                size
            );
            
            return size;
        }
        
        /**
         * This function decodes bytes.
         *
         * @tparam t_from
         * From type.
         * @tparam t_to
         * To type.
         * @param a_from
         * From.
         * @param a_to
         * To.
         * @return
         * To size.
         */
        template<
            meta::bytes t_from,
            meta::bytes t_to
        >
        static
        constexpr auto
        decode(
            const t_from& a_from,
            t_to& a_to
        ) noexcept(false) -> size_t
        {
            internal::resize(
                a_to,
                decode_size(a_from)
            );
            
            auto size = decode(
                a_from.data(),
                a_from.size(),
                a_to.data()
            );
            
            internal::resize(
                a_to,
                size
            );
            
            return size;
        }
        
        /**
         * This function decodes bytes.
         *
         * @tparam t_to
         * To type.
         * @tparam t_from
         * From type.
         * @param a_from
         * From.
         * @return
         * To.
         */
        template<
            meta::bytes t_to,
            meta::bytes t_from
        >
        static
        constexpr auto
        decode(
            const t_from& a_from
        ) noexcept(false) -> t_to
        requires
        requires
        {
            requires meta::container_dynamic<t_to>;
        }
        {
            t_to result;
            
            decode(
                a_from,
                result
            );
            
            return result;
        }
        
        /**
         * This function calculates minimum bytes size after decoding.
         *
         * @tparam t_from
         * From type.
         * @param a_from_ptr
         * From pointer.
         * @param a_from_size
         * From size.
         * @return
         * To size.
         */
        template<
            meta::byte t_from
        >
        static
        constexpr auto
        decode_size(
            const t_from* a_from_ptr,
            size_t a_from_size
        ) noexcept(true) -> size_t
        {
            return impl_type::decode_size(
                a_from_ptr,
                a_from_size
            );
        }
        
        /**
         * This function calculates minimum bytes size after decoding.
         *
         * @tparam t_from
         * From type.
         * @param a_from
         * From.
         * @return
         * To size.
         */
        template<
            meta::bytes t_from
        >
        static
        constexpr auto
        decode_size(
            const t_from& a_from
        ) noexcept(true) -> size_t
        {
            return decode_size(
                a_from.data(),
                a_from.size()
            );
        }
    };
    
    /**
     * This function encodes bytes.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_from
     * From type.
     * @tparam t_to
     * To type.
     * @param a_from_ptr
     * From pointer.
     * @param a_from_size
     * From size.
     * @param a_to_ptr
     * To pointer.
     * @return
     * To size.
     */
    template<
        meta::primitive::codec t_impl,
        meta::byte t_from,
        meta::byte t_to
    >
    constexpr auto
    encode(
        const t_from* a_from_ptr,
        size_t a_from_size,
        t_to* a_to_ptr
    ) noexcept(true) -> size_t
    {
        return apie<t_impl>::template encode<t_from, t_to>(
            a_from_ptr,
            a_from_size,
            a_to_ptr
        );
    }
    
    /**
     * This function encodes bytes.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_from
     * From type.
     * @tparam t_to
     * To type.
     * @param a_from
     * From.
     * @param a_to
     * To.
     * @return
     * To size.
     */
    template<
        meta::primitive::codec t_impl,
        meta::bytes t_from,
        meta::bytes t_to
    >
    static
    constexpr auto
    encode(
        const t_from& a_from,
        t_to& a_to
    ) noexcept(meta::container_static<t_to>) -> size_t
    {
        return apie<t_impl>::template encode<t_from, t_to>(
            a_from,
            a_to
        );
    }
    
    /**
     * This function encodes bytes.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_to
     * To type.
     * @tparam t_from
     * From type.
     * @param a_from
     * From.
     * @return
     * To.
     */
    template<
        meta::primitive::codec t_impl,
        meta::bytes t_to = container::bytes_dynamic,
        meta::bytes t_from
    >
    static
    constexpr auto
    encode(
        const t_from& a_from
    ) noexcept(false) -> t_to
    {
        return apie<t_impl>::template encode<t_to, t_from>(
            a_from
        );
    }
    
    /**
     * This function decodes bytes.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_from
     * From type.
     * @tparam t_to
     * To type.
     * @param a_from_ptr
     * From pointer.
     * @param a_from_size
     * From size.
     * @param a_to_ptr
     * To pointer.
     * @param a_error
     * Error.
     * @return
     * To size.
     */
    template<
        meta::primitive::codec t_impl,
        meta::byte t_from,
        meta::byte t_to
    >
    constexpr auto
    decode(
        const t_from* a_from_ptr,
        size_t a_from_size,
        t_to* a_to_ptr,
        error& a_error
    ) noexcept(true) -> size_t
    {
        return apie<t_impl>::template decode<t_from, t_to>(
            a_from_ptr,
            a_from_size,
            a_to_ptr,
            a_error
        );
    }
    
    /**
     * This function decodes bytes.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_from
     * From type.
     * @tparam t_to
     * To type.
     * @param a_from_ptr
     * From pointer.
     * @param a_from_size
     * From size.
     * @param a_to_ptr
     * To pointer.
     * @return
     * To size.
     */
    template<
        meta::primitive::codec t_impl,
        meta::byte t_from,
        meta::byte t_to
    >
    constexpr auto
    decode(
        const t_from* a_from_ptr,
        size_t a_from_size,
        t_to* a_to_ptr
    ) noexcept(false) -> size_t
    {
        return apie<t_impl>::template decode<t_from, t_to>(
            a_from_ptr,
            a_from_size,
            a_to_ptr
        );
    }
    
    /**
     * This function decodes bytes.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_from
     * From type.
     * @tparam t_to
     * To type.
     * @param a_from
     * From.
     * @param a_to
     * To.
     * @param a_error
     * Error.
     * @return
     * To size.
     */
    template<
        meta::primitive::codec t_impl,
        meta::bytes t_from,
        meta::bytes t_to
    >
    static
    constexpr auto
    decode(
        const t_from& a_from,
        t_to& a_to,
        error& a_error
    ) noexcept(meta::container_static<t_to>) -> size_t
    {
        return apie<t_impl>::template decode<t_from, t_to>(
            a_from,
            a_to,
            a_error
        );
    }
    
    /**
     * This function decodes bytes.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_from
     * From type.
     * @tparam t_to
     * To type.
     * @param a_from
     * From.
     * @param a_to
     * To.
     * @return
     * To size.
     */
    template<
        meta::primitive::codec t_impl,
        meta::bytes t_from,
        meta::bytes t_to
    >
    static
    constexpr auto
    decode(
        const t_from& a_from,
        t_to& a_to
    ) noexcept(false) -> size_t
    {
        return apie<t_impl>::template decode<t_from, t_to>(
            a_from,
            a_to
        );
    }
    
    /**
     * This function decodes bytes.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_to
     * To type.
     * @tparam t_from
     * From type.
     * @param a_from
     * From.
     * @return
     * To.
     */
    template<
        meta::primitive::codec t_impl,
        meta::bytes t_to = container::bytes_dynamic,
        meta::bytes t_from
    >
    static
    constexpr auto
    decode(
        const t_from& a_from
    ) noexcept(false) -> t_to
    {
        return apie<t_impl>::template decode<t_to, t_from>(
            a_from
        );
    }
}
