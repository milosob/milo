

#pragma once


#include <milo/internal.h>


namespace milo::primitive::detail
{
    template<
        meta::primitive::mac_hmac t_hmac,
        typename... t_options
    >
    class kdf_hkdf
    {
    public:
        
        using hmac_type = t_hmac;
    
    public:
        
        struct properties
        {
            using kdf [[maybe_unused]] = int;
            
            using kdf_hkdf [[maybe_unused]] = int;
        };
    
    private:
        
        static
        constexpr size_t expand_size = hmac_type::digest_size;
    
    private:
        
        hmac_type m_hmac;
        
        internal::memory_bytes_const_view m_info;
        
        uint8_t m_counter = 1;
        
        uint8_t m_prk[expand_size]{};
        
        uint8_t m_buffer[expand_size]{};
        
        size_t m_buffer_size = 0;
    
    public:
        
        constexpr kdf_hkdf() noexcept(true) = default;
        
        constexpr kdf_hkdf(kdf_hkdf&& object) noexcept(true) = default;
        
        constexpr kdf_hkdf(const kdf_hkdf& object) noexcept(true) = default;
        
        constexpr ~kdf_hkdf() noexcept(true)
        {
            internal::memory_erase(m_counter);
            internal::memory_erase(m_prk);
            internal::memory_erase(m_buffer);
            internal::memory_erase(m_buffer_size);
        }
    
    public:
        
        constexpr auto
        operator =(const kdf_hkdf& object) noexcept(true) -> kdf_hkdf& = default;
    
    private:
        
        template<
            meta::byte t_key,
            meta::byte t_previous
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
        
        template<
            meta::byte t_ikm,
            meta::byte t_salt,
            meta::byte t_info
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
            m_info = internal::memory_bytes_const_view(
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
        
        template<
            meta::byte t_key
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
                auto copy_size = internal::min(
                    key_size,
                    m_buffer_size
                );
                
                internal::memory_copy(
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
            
            internal::memory_copy(
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
