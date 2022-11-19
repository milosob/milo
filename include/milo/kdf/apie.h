

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/container.h>
#include <milo/error.h>
#include <milo/utility.h>


namespace milo::kdf
{
    /**
     * Kdf wrapper that provides access to additional APIs.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        concepts::kdf t_impl
    >
    class apie
    {
    public:
        
        using type = apie;
        
        using impl_type = t_impl;
    
    private:
        
        impl_type m_impl;
    
    public:
        
        constexpr apie() noexcept(true) = default;
        
        constexpr apie(type&& object) noexcept(true) = default;
    
        constexpr apie(const type& object) noexcept(true) = default;
    
        constexpr ~apie() noexcept(true) = default;

    public:
    
        constexpr auto
        operator =(const type& object) noexcept(true) -> type& = default;

    public:
    
        /**
         * Constructs object.
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
        constexpr
        apie(
            const t_ikm* a_ikm_ptr,
            size_t a_ikm_size,
            const t_salt* a_salt_ptr,
            size_t a_salt_size,
            const t_info* a_info_ptr,
            size_t a_info_size
        ) noexcept(true)
        requires
        requires
        {
            requires concepts::kdf_hkdf<impl_type>;
        }
        {
            initialize(
                a_ikm_ptr,
                a_ikm_size,
                a_salt_ptr,
                a_salt_size,
                a_info_ptr,
                a_info_size
            );
        }
    
        /**
         * Constructs object.
         *
         * @tparam t_ikm
         * Ikm type.
         * @tparam t_salt
         * Salt type.
         * @tparam t_info
         * Info type.
         * @param a_ikm
         * Ikm.
         * @param a_salt
         * Salt.
         * @param a_info
         * Info.
         * Internal memory pointer must be valid and not change
         * as long as methods of this class are invoked.
         */
        template<
            concepts::bytes t_ikm,
            concepts::bytes t_salt,
            concepts::bytes t_info
        >
        constexpr
        apie(
            const t_ikm& a_ikm,
            const t_salt& a_salt,
            const t_info& a_info
        ) noexcept(true)
        requires
        requires
        {
            requires concepts::kdf_hkdf<impl_type>;
        }
        {
            initialize(
                a_ikm,
                a_salt,
                a_info
            );
        }
    
        /**
         * Constructs object.
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
        constexpr
        apie(
            const t_ikm* a_ikm_ptr,
            size_t a_ikm_size,
            const t_salt* a_salt_ptr,
            size_t a_salt_size,
            uint32_t a_iterations
        ) noexcept(true)
        requires
        requires
        {
            requires concepts::kdf_pbkdf_2<impl_type>;
        }
        {
            initialize(
                a_ikm_ptr,
                a_ikm_size,
                a_salt_ptr,
                a_salt_size,
                a_iterations
            );
        }
    
        /**
         * Constructs object.
         *
         * @tparam t_ikm
         * Ikm type.
         * @tparam t_salt
         * Salt type.
         * @param a_ikm
         * Ikm.
         * Internal memory pointer must be valid and not change
         * as long as methods of this class are invoked.
         * @param a_salt
         * Salt.
         * Internal memory pointer must be valid and not change
         * as long as methods of this class are invoked.
         * @param a_iterations
         * Iterations.
         */
        template<
            concepts::bytes t_ikm,
            concepts::bytes t_salt
        >
        constexpr
        apie(
            const t_ikm& a_ikm,
            const t_salt& a_salt,
            uint32_t a_iterations
        ) noexcept(true)
        requires
        requires
        {
            requires concepts::kdf_pbkdf_2<impl_type>;
        }
        {
            initialize(
                a_ikm,
                a_salt,
                a_iterations
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
        requires
        requires
        {
            requires concepts::kdf_hkdf<impl_type>;
        }
        {
            m_impl.initialize(
                a_ikm_ptr,
                a_ikm_size,
                a_salt_ptr,
                a_salt_size,
                a_info_ptr,
                a_info_size
            );
        }
        
        /**
         * This function initializes context.
         *
         * @tparam t_ikm
         * Ikm type.
         * @tparam t_salt
         * Salt type.
         * @tparam t_info
         * Info type.
         * @param a_ikm
         * Ikm.
         * @param a_salt
         * Salt.
         * @param a_info
         * Info.
         * Internal memory pointer must be valid and not change
         * as long as methods of this class are invoked.
         */
        template<
            concepts::bytes t_ikm,
            concepts::bytes t_salt,
            concepts::bytes t_info
        >
        constexpr auto
        initialize(
            const t_ikm& a_ikm,
            const t_salt& a_salt,
            const t_info& a_info
        ) noexcept(true) -> void
        requires
        requires
        {
            requires concepts::kdf_hkdf<impl_type>;
        }
        {
            initialize(
                a_ikm.data(),
                a_ikm.size(),
                a_salt.data(),
                a_salt.size(),
                a_info.data(),
                a_info.size()
            );
        }
        
        /**
         * This function initializes context.
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
        requires
        requires
        {
            requires concepts::kdf_pbkdf_2<impl_type>;
        }
        {
            m_impl.initialize(
                a_ikm_ptr,
                a_ikm_size,
                a_salt_ptr,
                a_salt_size,
                a_iterations
            );
        }
        
        /**
         * This function initializes context.
         *
         * @tparam t_ikm
         * Ikm type.
         * @tparam t_salt
         * Salt type.
         * @param a_ikm
         * Ikm.
         * Internal memory pointer must be valid and not change
         * as long as methods of this class are invoked.
         * @param a_salt
         * Salt.
         * Internal memory pointer must be valid and not change
         * as long as methods of this class are invoked.
         * @param a_iterations
         * Iterations.
         */
        template<
            concepts::bytes t_ikm,
            concepts::bytes t_salt
        >
        constexpr auto
        initialize(
            const t_ikm& a_ikm,
            const t_salt& a_salt,
            uint32_t a_iterations
        ) noexcept(true) -> void
        requires
        requires
        {
            requires concepts::kdf_pbkdf_2<impl_type>;
        }
        {
            initialize(
                a_ikm.data(),
                a_ikm.size(),
                a_salt.data(),
                a_salt.size(),
                a_iterations
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
            return m_impl.derive(
                a_key_ptr,
                a_key_size
            );
        }
    
        /**
         * This function derives key.
         *
         * @tparam t_key
         * Key type.
         * @param a_key
         * Key.
         * @param a_key_size
         * Key size.
         * @return
         * Key size.
         */
        template<
            concepts::bytes t_key
        >
        constexpr auto
        derive(
            t_key& a_key,
            size_t a_key_size
        ) noexcept(concepts::container_static<t_key>) -> size_t
        {
            a_key_size = utility::resize(
                a_key,
                a_key_size
            );
    
            return derive(
                a_key.data(),
                a_key_size
            );
        }
    
        /**
         * This function derives key.
         *
         * @tparam t_key
         * Key type.
         * @param a_key_size
         * Key size.
         * @return
         * Key.
         */
        template<
            concepts::bytes t_key = container::bytes_dynamic
        >
        constexpr auto
        derive(
            size_t a_key_size
        ) noexcept(false) -> t_key
        requires
        requires
        {
            requires concepts::container_dynamic<t_key>;
        }
        {
            t_key result;
    
            derive(
                result,
                a_key_size
            );
    
            return result;
        }
    
        /**
         * This function derives key.
         *
         * @tparam t_key
         * Key type.
         * @return
         * Key.
         */
        template<
            concepts::bytes t_key
        >
        constexpr auto
        derive(
        ) noexcept(true) -> t_key
        requires
        requires
        {
            requires concepts::container_static<t_key>;
        }
        {
            t_key result;
    
            derive(
                result,
                result.size()
            );
    
            return result;
        }
    };
    
    /**
     * This function derives key.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_ikm
     * Ikm type.
     * @tparam t_salt
     * Salt type.
     * @tparam t_info
     * Info type.
     * @tparam t_key
     * Key type.
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
     * @param a_info_size
     * Info size.
     * @param a_key_ptr
     * Key pointer.
     * @param a_key_size
     * Key size.
     * @return
     * Key size.
     */
    template<
        concepts::kdf_hkdf t_impl,
        concepts::byte t_ikm,
        concepts::byte t_salt,
        concepts::byte t_info,
        concepts::byte t_key
    >
    constexpr auto
    derive(
        const t_ikm* a_ikm_ptr,
        size_t a_ikm_size,
        const t_salt* a_salt_ptr,
        size_t a_salt_size,
        const t_info* a_info_ptr,
        size_t a_info_size,
        t_key* a_key_ptr,
        size_t a_key_size
    ) noexcept(true) -> size_t
    {
        apie<t_impl> apie(
            a_ikm_ptr,
            a_ikm_size,
            a_salt_ptr,
            a_salt_size,
            a_info_ptr,
            a_info_size
        );
        
        return apie.template derive<t_key>(
            a_key_ptr,
            a_key_size
        );
    }
    
    /**
     * This function derives key.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_ikm
     * Ikm type.
     * @tparam t_salt
     * Salt type.
     * @tparam t_info
     * Info type.
     * @tparam t_key
     * Key type.
     * @param a_ikm
     * Ikm.
     * @param a_salt
     * Salt.
     * @param a_info
     * Info.
     * @param a_key
     * Key.
     * @param a_key_size
     * Key size.
     * @return
     * Key size.
     */
    template<
        concepts::kdf_hkdf t_impl,
        concepts::bytes t_ikm,
        concepts::bytes t_salt,
        concepts::bytes t_info,
        concepts::bytes t_key
    >
    constexpr auto
    derive(
        const t_ikm& a_ikm,
        const t_salt& a_salt,
        const t_info& a_info,
        t_key& a_key,
        size_t a_key_size
    ) noexcept(concepts::container_static<t_key>) -> size_t
    {
        apie<t_impl> apie(
            a_ikm,
            a_salt,
            a_info
        );
    
        return apie.template derive<t_key>(
            a_key,
            a_key_size
        );
    }
    
    /**
     * This function derives key.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_ikm
     * Ikm type.
     * @tparam t_salt
     * Salt type.
     * @tparam t_info
     * Info type.
     * @param a_ikm
     * Ikm.
     * @param a_salt
     * Salt.
     * @param a_info
     * Info.
     * @param a_key_size
     * Key size.
     * @return
     * Key.
     */
    template<
        concepts::kdf_hkdf t_impl,
        concepts::bytes t_key = container::bytes_dynamic,
        concepts::bytes t_ikm,
        concepts::bytes t_salt,
        concepts::bytes t_info
    >
    constexpr auto
    derive(
        const t_ikm& a_ikm,
        const t_salt& a_salt,
        const t_info& a_info,
        size_t a_key_size
    ) noexcept(false) -> t_key
    {
        apie<t_impl> apie(
            a_ikm,
            a_salt,
            a_info
        );
        
        return apie.template derive<t_key>(
            a_key_size
        );
    }
    
    /**
     * This function derives key.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_ikm
     * Ikm type.
     * @tparam t_salt
     * Salt type.
     * @tparam t_info
     * Info type.
     * @param a_ikm
     * Ikm.
     * @param a_salt
     * Salt.
     * @param a_info
     * Info.
     * @return
     * Key.
     */
    template<
        concepts::kdf_hkdf t_impl,
        concepts::bytes t_key,
        concepts::bytes t_ikm,
        concepts::bytes t_salt,
        concepts::bytes t_info
    >
    constexpr auto
    derive(
        const t_ikm& a_ikm,
        const t_salt& a_salt,
        const t_info& a_info
    ) noexcept(true) -> t_key
    {
        apie<t_impl> apie(
            a_ikm,
            a_salt,
            a_info
        );
    
        return apie.template derive<t_key>();
    }
    
    /**
     * This function derives key.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_ikm
     * Ikm type.
     * @tparam t_salt
     * Salt type.
     * @tparam t_key
     * Key type.
     * @param a_ikm_ptr
     * Ikm pointer.
     * @param a_ikm_size
     * Ikm size.
     * @param a_salt_ptr
     * Salt pointer.
     * @param a_salt_size
     * Salt size.
     * @param a_iterations
     * Iterations.
     * @param a_key_ptr
     * Key pointer.
     * @param a_key_size
     * Key size.
     * @return
     * Key size.
     */
    template<
        concepts::kdf_pbkdf_2 t_impl,
        concepts::byte t_ikm,
        concepts::byte t_salt,
        concepts::byte t_key
    >
    constexpr auto
    derive(
        const t_ikm* a_ikm_ptr,
        size_t a_ikm_size,
        const t_salt* a_salt_ptr,
        size_t a_salt_size,
        uint32_t a_iterations,
        t_key* a_key_ptr,
        size_t a_key_size
    ) noexcept(true) -> size_t
    {
        apie<t_impl> apie(
            a_ikm_ptr,
            a_ikm_size,
            a_salt_ptr,
            a_salt_size,
            a_iterations
        );
    
        return apie.template derive<t_key>(
            a_key_ptr,
            a_key_size
        );
    }
    
    /**
     * This function derives key.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_ikm
     * Ikm type.
     * @tparam t_salt
     * Salt type.
     * @tparam t_key
     * Key type.
     * @param a_ikm
     * Ikm.
     * @param a_salt
     * Salt.
     * @param a_iterations
     * Iterations.
     * @param a_key
     * Key.
     * @param a_key_size
     * Key size.
     * @return
     * Key size.
     */
    template<
        concepts::kdf_pbkdf_2 t_impl,
        concepts::bytes t_ikm,
        concepts::bytes t_salt,
        concepts::bytes t_key
    >
    constexpr auto
    derive(
        const t_ikm& a_ikm,
        const t_salt& a_salt,
        uint32_t a_iterations,
        t_key& a_key,
        size_t a_key_size
    ) noexcept(concepts::container_static<t_key>) -> size_t
    {
        apie<t_impl> apie(
            a_ikm,
            a_salt,
            a_iterations
        );
    
        return apie.template derive<t_key>(
            a_key,
            a_key_size
        );
    }
    
    /**
     * This function derives key.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_ikm
     * Ikm type.
     * @tparam t_salt
     * Salt type.
     * @param a_ikm
     * Ikm.
     * @param a_salt
     * Salt.
     * @param a_iterations
     * Iterations.
     * @param a_key_size
     * Key size.
     * @return
     * Key.
     */
    template<
        concepts::kdf_pbkdf_2 t_impl,
        concepts::bytes t_key = container::bytes_dynamic,
        concepts::bytes t_ikm,
        concepts::bytes t_salt
    >
    constexpr auto
    derive(
        const t_ikm& a_ikm,
        const t_salt& a_salt,
        uint32_t a_iterations,
        size_t a_key_size
    ) noexcept(false) -> t_key
    {
        apie<t_impl> apie(
            a_ikm,
            a_salt,
            a_iterations
        );
        
        return apie.template derive<t_key>(
            a_key_size
        );
    }
    
    /**
     * This function derives key.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_ikm
     * Ikm type.
     * @tparam t_salt
     * Salt type.
     * @param a_ikm
     * Ikm.
     * @param a_salt
     * Salt.
     * @param a_iterations
     * Iterations.
     * @return
     * Key.
     */
    template<
        concepts::kdf_pbkdf_2 t_impl,
        concepts::bytes t_key,
        concepts::bytes t_ikm,
        concepts::bytes t_salt
    >
    constexpr auto
    derive(
        const t_ikm& a_ikm,
        const t_salt& a_salt,
        uint32_t a_iterations
    ) noexcept(true) -> t_key
    {
        apie<t_impl> apie(
            a_ikm,
            a_salt,
            a_iterations
        );
        
        return apie.template derive<t_key>();
    }
}
