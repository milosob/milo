

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/memory.h>


namespace milo::kdf
{
    template<
        concepts::mac_hmac t_hmac,
        typename... t_options
    >
    class hkdf_basic
    {
    public:
        
        using type = hkdf_basic;
        
        using hmac_type = t_hmac;
    
    public:
        
        struct properties
        {
            using kdf_type [[maybe_unused]] = type;
            
            using kdf_hkdf_type [[maybe_unused]] = type;
        };

    private:
    
        static
        constexpr size_t expand_size = hmac_type::digest_size;

    private:
    
        hmac_type m_hmac;
    
        memory::detail::bytes_const_view m_info;
    
        uint8_t m_counter = 1;
    
        uint8_t m_prk[expand_size]{};
    
        uint8_t m_buffer[expand_size]{};
    
        size_t m_buffer_size = 0;

    public:
    
        constexpr hkdf_basic() noexcept(true) = default;
        
        constexpr hkdf_basic(type&& object) noexcept(true) = default;
        
        constexpr hkdf_basic(const type& object) noexcept(true) = default;
        
        constexpr ~hkdf_basic() noexcept(true)
        {
            memory::erase(m_counter);
            memory::erase(m_prk);
            memory::erase(m_buffer);
            memory::erase(m_buffer_size);
        }
    
    public:
        
        constexpr auto
        operator =(const type& object) noexcept(true) -> type& = default;
    
    private:
        
        template<
            concepts::byte t_key,
            concepts::byte t_previous
        >
        constexpr auto
        expand(
            t_key* a_key,
            uint8_t a_counter,
            const t_previous* a_previous_ptr,
            size_t a_previous_size
        ) noexcept(true) -> void
        {
            m_hmac.initialize(
                m_prk,
                sizeof(m_prk)
            );
            m_hmac.update(
                a_previous_ptr,
                a_previous_size
            );
            m_hmac.update(
                m_info.data(),
                m_info.size()
            );
            m_hmac.update(
                &a_counter,
                sizeof(a_counter)
            );
            m_hmac.finalize();
            m_hmac.digest(
                a_key
            );
        }
    
    public:
        
        /**
         * This function initializes context.
         *
         * @tparam t_ikm
         * Ikm type.
         * @tparam t_salt
         * Salt type.
         * @tparam t_info
         * Info type.
         * @param a_ikm_ptr
         * Ikm pointer.
         * @param a_ikm_size
         * Ikm size.
         * @param a_salt_ptr
         * Salt pointer.
         * @param a_salt_size
         * Salt size.
         * @param a_info_ptr
         * Info ptr.
         * Must be a valid pointer as long as methods of this class are invoked.
         * @param a_info_size
         * Info size.
         */
        template<
            concepts::byte t_ikm,
            concepts::byte t_salt,
            concepts::byte t_info
        >
        constexpr auto
        initialize(
            const t_ikm* a_ikm_ptr,
            size_t a_ikm_size,
            const t_salt* a_salt_ptr,
            size_t a_salt_size,
            const t_info* a_info_ptr,
            size_t a_info_size
        ) noexcept(true) -> void
        {
            m_info = memory::detail::bytes_const_view(
                a_info_ptr,
                a_info_size
            );
    
            m_counter = 1;
            m_buffer_size = 0;
    
            m_hmac.initialize(
                a_salt_ptr,
                a_salt_size
            );
            m_hmac.update(
                a_ikm_ptr,
                a_ikm_size
            );
            m_hmac.finalize();
            m_hmac.digest(
                m_prk
            );
        }
        
        /**
         * This function derives key.
         *
         * @tparam t_key
         * Key type.
         * @param a_key_ptr
         * Key pointer.
         * @param a_key_size
         * Key size.
         * @return
         * Key size.
         */
        template<
            concepts::byte t_key
        >
        constexpr auto
        derive(
            t_key* a_key_ptr,
            size_t a_key_size
        ) noexcept(true) -> size_t
        {
            auto key_ptr = a_key_ptr;
            auto key_size = a_key_size;
            
            if (m_buffer_size > 0)
            {
                auto copy_size = common::min(
                    key_size,
                    m_buffer_size
                );
                
                memory::copy(
                    key_ptr,
                    m_buffer + expand_size - m_buffer_size,
                    copy_size
                );
                
                key_ptr += copy_size;
                key_size -= copy_size;
                
                m_buffer_size -= copy_size;
            }
            
            if (key_size == 0)
            {
                return a_key_size;
            }
            
            auto counter = m_counter;
            
            auto full_size = key_size / expand_size;
            auto last_size = key_size % expand_size;
            
            auto beg_ptr = counter == 1 ? nullptr : m_buffer;
            auto beg_size = counter == 1 ? 0 : expand_size;
            
            if (full_size == 0)
            {
                expand(
                    m_buffer,
                    counter,
                    beg_ptr,
                    beg_size
                );
                
                counter += 1;
            }
            else
            {
                auto tmp_ptr = key_ptr;
                
                expand(
                    key_ptr,
                    counter,
                    beg_ptr,
                    beg_size
                );
                
                counter += 1;
                key_ptr += expand_size;
                
                for (size_t i = 1; i < full_size; i += 1)
                {
                    expand(
                        key_ptr,
                        counter,
                        tmp_ptr,
                        expand_size
                    );
                    
                    counter += 1;
                    key_ptr += expand_size;
                    tmp_ptr += expand_size;
                }
                
                expand(
                    m_buffer,
                    counter,
                    tmp_ptr,
                    expand_size
                );
                
                counter += 1;
            }
    
            memory::copy(
                key_ptr,
                m_buffer,
                last_size
            );
    
            m_buffer_size = expand_size - last_size;
            m_counter = counter;
    
            return a_key_size;
        }
    };
}
