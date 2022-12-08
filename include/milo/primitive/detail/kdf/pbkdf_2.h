

#pragma once


#include <milo/inner.h>


namespace milo::primitive::detail
{
    template<
        meta::primitive::prf_kdf_pbkdf_2 t_prf,
        typename... t_options
    >
    class kdf_pbkdf_2
    {
    public:
        
        using prf_type = t_prf;
    
    public:
        
        struct properties
        {
            using kdf [[maybe_unused]] = int;
            
            using kdf_pbkdf_2 [[maybe_unused]] = int;
        };
    
    private:
        
        static
        constexpr size_t expand_size = prf_type::digest_size;
    
    private:
        
        prf_type m_prf;
        
        inner::memory_bytes_const_view m_ikm;
        
        inner::memory_bytes_const_view m_salt;
        
        uint32_t m_iterations = 0;
        
        uint32_t m_counter = 1;
        
        uint8_t m_buffer[expand_size]{};
        
        size_t m_buffer_size = 0;
    
    public:
        
        constexpr kdf_pbkdf_2() noexcept(true) = default;
        
        constexpr kdf_pbkdf_2(kdf_pbkdf_2&& object) noexcept(true) = default;
        
        constexpr kdf_pbkdf_2(const kdf_pbkdf_2& object) noexcept(true) = default;
        
        constexpr ~kdf_pbkdf_2() noexcept(true)
        {
            inner::memory_erase(m_iterations);
            inner::memory_erase(m_counter);
            inner::memory_erase(m_buffer);
            inner::memory_erase(m_buffer_size);
        }
    
    public:
        
        constexpr auto
        operator =(
            const kdf_pbkdf_2& object
        ) noexcept(true) -> kdf_pbkdf_2& = default;
    
    private:
        
        template<
            meta::byte t_key
        >
        constexpr auto
        expand(
            t_key* a_key_ptr,
            uint32_t a_counter
        ) noexcept(true) -> void
        {
            uint8_t buffer[expand_size * 2];
            uint8_t buffer_counter[sizeof(uint32_t)];
            
            inner::memory_stor_be<uint32_t>(
                buffer_counter,
                0,
                a_counter
            );
            
            m_prf.initialize(
                m_ikm.data(),
                m_ikm.size()
            );
            m_prf.update(
                m_salt.data(),
                m_salt.size()
            );
            m_prf.update(
                buffer_counter,
                sizeof(buffer_counter)
            );
            m_prf.finalize();
            m_prf.digest(
                buffer
            );
            
            inner::memory_copy(
                buffer + expand_size,
                buffer,
                expand_size
            );
            
            for (size_t i = 1; i < m_iterations - 1; i += 1)
            {
                m_prf.initialize(
                    m_ikm.data(),
                    m_ikm.size()
                );
                m_prf.update(
                    buffer,
                    expand_size
                );
                m_prf.finalize();
                m_prf.digest(
                    buffer
                );
                
                inner::memory_xor(
                    buffer + expand_size,
                    buffer + expand_size,
                    buffer,
                    expand_size
                );
            }
            
            m_prf.initialize(
                m_ikm.data(),
                m_ikm.size()
            );
            m_prf.update(
                buffer,
                expand_size
            );
            m_prf.finalize();
            m_prf.digest(
                buffer
            );
            
            inner::memory_xor(
                a_key_ptr,
                buffer + expand_size,
                buffer,
                expand_size
            );
        }
    
    public:
        
        template<
            meta::byte t_ikm,
            meta::byte t_salt
        >
        constexpr auto
        initialize(
            const t_ikm* a_ikm_ptr,
            size_t a_ikm_size,
            const t_salt* a_salt_ptr,
            size_t a_salt_size,
            uint32_t a_iterations
        ) noexcept(true) -> void
        {
            m_ikm = inner::memory_bytes_const_view(
                a_ikm_ptr,
                a_ikm_size
            );
            m_salt = inner::memory_bytes_const_view(
                a_salt_ptr,
                a_salt_size
            );
            
            m_iterations = a_iterations;
            m_counter = 1;
            m_buffer_size = 0;
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
                auto copy_size = inner::min(
                    key_size,
                    m_buffer_size
                );
                
                inner::memory_copy(
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
            
            for (size_t i = 0; i < full_size; i += 1)
            {
                expand(
                    key_ptr,
                    counter
                );
                
                counter += 1;
                key_ptr += expand_size;
            }
            
            if (last_size > 0)
            {
                expand(
                    m_buffer,
                    counter
                );
                
                counter += 1;
                
                inner::memory_copy(
                    key_ptr,
                    m_buffer,
                    last_size
                );
                
                m_buffer_size = expand_size - last_size;
            }
            else
            {
                m_buffer_size = 0;
            }
            
            m_counter = counter;
            
            return a_key_size;
        }
    };
}
