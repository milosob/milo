

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/memory.h>


namespace milo::kdf
{
    template<
        concepts::prf_kdf_pbkdf_2 t_prf,
        typename... t_options
    >
    class pbkdf_2_basic
    {
    public:
        
        using type = pbkdf_2_basic;
        
        using prf_type = t_prf;
    
    public:
        
        struct properties
        {
            using kdf_type [[maybe_unused]] = type;
            
            using kdf_pbkdf_2_type [[maybe_unused]] = type;
        };
    
    private:
        
        static
        constexpr size_t expand_size = prf_type::digest_size;
    
    private:
    
        prf_type m_prf;
    
        memory::detail::bytes_const_view m_ikm;
    
        memory::detail::bytes_const_view m_salt;
    
        uint32_t m_iterations = 0;
    
        uint32_t m_counter = 1;
    
        uint8_t m_buffer[expand_size]{};
    
        size_t m_buffer_size = 0;

    public:
    
        constexpr pbkdf_2_basic() noexcept(true) = default;
    
        constexpr pbkdf_2_basic(type&& object) noexcept(true) = default;
    
        constexpr pbkdf_2_basic(const type& object) noexcept(true) = default;
    
        constexpr ~pbkdf_2_basic() noexcept(true)
        {
            memory::erase(m_iterations);
            memory::erase(m_counter);
            memory::erase(m_buffer);
            memory::erase(m_buffer_size);
        }
    
    public:
        
        constexpr auto
        operator =(const type& object) noexcept(true) -> type& = default;
    
    private:
        
        template<
            concepts::byte t_key
        >
        constexpr auto
        expand(
            t_key* a_key_ptr,
            uint32_t a_counter
        ) noexcept(true) -> void
        {
            uint8_t buffer[expand_size * 2];
            uint8_t buffer_counter[sizeof(uint32_t)];
    
            memory::stor_be<uint32_t>(
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
            
            memory::copy(
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
                
                memory::xorr(
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
            
            memory::xorr(
                a_key_ptr,
                buffer + expand_size,
                buffer,
                expand_size
            );
        }
    
    public:
        
        /**
         * This function initializes the context.
         *
         *
         * @tparam t_ikm
         * Ikm type.
         * @tparam t_salt
         * Salt type.
         * @param a_ikm_ptr
         * Ikm pointer.
         * Must be a valid pointer as long as methods of this class are invoked.
         * @param a_ikm_size
         * Ikm size.
         * @param a_salt_ptr
         * Salt pointer.
         * Must be a valid pointer as long as methods of this class are invoked.
         * @param a_salt_size
         * Salt size.
         * @param a_iterations
         * Iterations.
         */
        template<
            concepts::byte t_ikm,
            concepts::byte t_salt
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
            m_ikm = memory::detail::bytes_const_view(
                a_ikm_ptr,
                a_ikm_size
            );
            m_salt = memory::detail::bytes_const_view(
                a_salt_ptr,
                a_salt_size
            );
    
            m_iterations = a_iterations;
            m_counter = 1;
            m_buffer_size = 0;
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
                
                memory::copy(
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
