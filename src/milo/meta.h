

#pragma once


#include <cstdint>
#include <cstddef>
#include <type_traits>

#include <milo/error.h>


/*
 * @formatter:off
 */

#define MILO_INTERNAL_CONSTEVAL                                     \
    (std::is_constant_evaluated())                                  \

/*
 * @formatter:on
 */

namespace milo::meta
{
    template<
        typename t_type
    >
    struct astype
    {
        using type = t_type;
    };
    
    template<
        bool t_value,
        typename t_type = void
    >
    struct asbool
    {
        static
        constexpr bool value = t_value;
        
        using type = t_type;
    };
    
    template<
        typename... t_args
    >
    struct args
    {
    };
    
    template<
        typename t_input,
        template<typename ...> typename t_reader,
        typename... t_reader_args
    >
    struct args_reader
    {
        using type = decltype(
        []<typename... t_args>(
            args<t_args...>
        ) constexpr noexcept(true) -> auto
        {
            return t_reader<
                t_reader_args...,
                t_args...
            >();
        }
            (
                t_input()
            )
        );
    };
    
    template<
        typename t_input,
        typename t_push
    >
    struct args_push_front
    {
        using type = decltype(
        []<typename... t_args>(
            args<t_args...>
        ) constexpr noexcept(true) -> auto
        {
            return args<
                t_push,
                t_args...
            >();
        }
            (
                t_input()
            )
        );
    };
    
    template<
        typename t_input,
        typename t_push
    >
    struct args_push_back
    {
        using type = decltype(
        []<typename... t_args>(
            args<t_args...>
        ) constexpr noexcept(true) -> auto
        {
            return args<
                t_args...,
                t_push
            >();
        }
            (
                t_input()
            )
        );
    };
    
    template<bool t_cond, typename t_0, typename t_1>
    using conditional = std::conditional<t_cond, t_0, t_1>;
    
    template<typename... t_args>
    using disjunction = std::disjunction<t_args...>;
    
    template<typename t_0>
    concept complex = std::is_class<t_0>::value;
    
    template<typename t_0, typename... t_args>
    concept same = std::disjunction<std::is_same<t_0, t_args>...>::value;
    
    template<typename t_0>
    concept assignable_bycopy = std::is_copy_assignable<t_0>::value;
    
    template<typename t_0>
    concept assignable_bycopy_noexcept = std::is_nothrow_copy_assignable<t_0>::value;
    
    template<typename t_0>
    concept assignable_bymove = std::is_move_assignable<t_0>::value;
    
    template<typename t_0>
    concept assignable_bymove_noexcept = std::is_nothrow_move_assignable<t_0>::value;
    
    template<typename t_0, typename... t_args>
    concept constructible = std::is_constructible<t_0, t_args...>::value;
    
    template<typename t_0, typename... t_args>
    concept constructible_noexcept = std::is_nothrow_constructible<t_0, t_args...>::value;
    
    template<typename t_0>
    concept constructible_bycopy = std::is_copy_constructible<t_0>::value;
    
    template<typename t_0>
    concept constructible_bycopy_noexcept = std::is_nothrow_copy_constructible<t_0>::value;
    
    template<typename t_0>
    concept constructible_bymove = std::is_move_constructible<t_0>::value;
    
    template<typename t_0>
    concept constructible_bymove_noexcept = std::is_nothrow_move_assignable<t_0>::value;
    
    template<typename t_0, typename t_1>
    concept convertible = std::is_convertible<t_0, t_1>::value;
    
    template<typename t_0>
    concept destructible = std::is_destructible<t_0>::value;
    
    template<typename t_0>
    concept destructible_noexcept = std::is_nothrow_destructible<t_0>::value;
    
    template<typename t_0, typename... t_args>
    concept invocable = std::is_invocable<t_0, t_args...>::value;
    
    template<typename t_0, typename... t_args>
    concept invocable_noexcept = std::is_nothrow_invocable<t_0, t_args...>::value;
    
    template<typename t_0, typename... t_args>
    using invocable_result = typename std::invoke_result<t_0, t_args...>::type;
    
    template<typename t_0>
    concept integral = std::is_integral<t_0>::value;
    
    template<typename t_0>
    concept integral_s = std::is_signed<t_0>::value;
    
    template<typename t_0>
    concept integral_u = std::is_unsigned<t_0>::value;
    
    template<typename t_0>
    using add_constant = typename std::add_const<t_0>::type;
    
    template<typename t_0>
    using add_constant_volatile = typename std::add_cv<t_0>::type;
    
    template<typename t_0>
    using add_volatile = typename std::add_volatile<t_0>::type;
    
    template<typename t_0>
    using del_constant = typename std::remove_const<t_0>::type;
    
    template<typename t_0>
    using del_constant_volatile = typename std::remove_cv<t_0>::type;
    
    template<typename t_0>
    using del_volatile = typename std::remove_volatile<t_0>::type;
    
    template<typename t_0>
    using del_reference = typename std::remove_reference<t_0>::type;
    
    template<typename t_0>
    using to_integral_signed = typename std::make_signed<t_0>::type;
    
    template<typename t_0>
    using to_integral_unsigned = typename std::make_unsigned<t_0>::type;
    
    template<
        typename t_arg,
        typename t_to,
        typename... t_args
    >
    using type_map = asbool<
        same<
            t_arg,
            t_args...
        >,
        t_to
    >;
    
    template<
        typename t_arg
    >
    using type_map_char_ptr_to_unsigned_char_ptr = type_map<
        t_arg,
        unsigned char*,
        char*
    >;
    
    template<
        typename t_arg
    >
    using type_map_const_char_ptr_to_const_unsigned_char_ptr = type_map<
        t_arg,
        const unsigned char*,
        const char*
    >;
    
    template<typename t_0>
    concept byte = same<
        t_0,
        char,
        signed char,
        unsigned char
    >;
    
    template<typename t_0>
    concept bytes =
    requires
    {
        typename t_0::value_type;
    } &&
    requires(t_0 impl)
    {
        {
        impl.data()
        } noexcept -> same<
            const char*,
            const signed char*,
            const unsigned char*,
            char*,
            signed char*,
            unsigned char*
        >;
        {
        impl.size()
        } noexcept -> same<
            size_t
        >;
    } &&
    requires(const t_0 impl)
    {
        {
        impl.data()
        } noexcept -> same<
            const char*,
            const signed char*,
            const unsigned char*,
            char*,
            signed char*,
            unsigned char*
        >;
        {
        impl.size()
        } noexcept -> same<
            size_t
        >;
    };
    
    template<typename t_0>
    concept container =
    requires
    {
        typename t_0::value_type;
    } &&
    requires(t_0 impl)
    {
        {
        impl.data()
        } noexcept -> same<
            const typename t_0::value_type*,
            typename t_0::value_type*
        >;
        {
        impl.size()
        } noexcept -> same<
            size_t
        >;
    } &&
    requires(const t_0 impl)
    {
        {
        impl.data()
        } noexcept -> same<
            const typename t_0::value_type*
        >;
        {
        impl.size()
        } noexcept -> same<
            size_t
        >;
    };
    
    template<typename t_0>
    concept container_reservable =
    requires(t_0 impl)
    {
        {
        impl.reserve(
            0
        )
        } -> same<void>;
    };
    
    template<typename t_0>
    concept container_resizable =
    requires(t_0 impl)
    {
        {
        impl.resize(
            0,
            typename t_0::value_type()
        )
        } -> same<void>;
    };
    
    template<typename t_0>
    concept container_dynamic =
    requires
    {
        requires container<t_0>;
        requires container_reservable<t_0>;
        requires container_resizable<t_0>;
    };
    
    template<typename t_0, size_t t_min = size_t(0), size_t t_max = size_t(-1)>
    concept container_static =
    requires
    {
        requires container<t_0>;
        requires !container_reservable<t_0>;
        requires !container_resizable<t_0>;
        requires t_0().size() >= t_min;
        requires t_0().size() <= t_max;
    };
    
    template<container_static t_0>
    constexpr auto container_static_size = t_0().size();
}

namespace milo::meta::primitive
{
    /*
     * Start codec.
     */
    
    template<typename t_0>
    concept codec =
    requires
    {
        typename t_0::properties::codec;
    } &&
    requires
    {
        t_0::ratio_bytes_size;
        t_0::ratio_chars_size;
    } &&
    requires(error a_error)
    {
        {
        t_0::encode(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<unsigned char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        t_0::encode(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        t_0::encode_size(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        t_0::encode_size(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        
        {
        t_0::decode(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<unsigned char*>(nullptr),
            a_error
        )
        } noexcept -> same<size_t>;
        {
        t_0::decode(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<char*>(nullptr),
            a_error
        )
        } noexcept -> same<size_t>;
        {
        t_0::decode_size(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        t_0::decode_size(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
    };
    
    template<typename t_0>
    concept codec_base_16 =
    requires
    {
        requires codec<t_0>;
    } &&
    requires
    {
        typename t_0::properties::codec_base_16;
    };
    
    template<typename t_0>
    concept codec_base_64 =
    requires
    {
        requires codec<t_0>;
    } &&
    requires
    {
        typename t_0::properties::codec_base_64;
    };
    
    /*
     * Start hash.
     */
    
    template<typename t_0>
    concept hash =
    requires
    {
        requires constructible_noexcept<t_0>;
        requires destructible_noexcept<t_0>;
    } &&
    requires
    {
        typename t_0::properties::hash;
    } &&
    requires
    {
        t_0::block_size;
        t_0::digest_size;
    } &&
    requires(t_0 impl)
    {
        {
        impl.initialize()
        } noexcept -> same<void>;
        {
        impl.update(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.update(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.finalize()
        } noexcept -> same<void>;
        {
        impl.digest(
            static_cast<char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<unsigned char*>(nullptr)
        )
        } noexcept -> same<size_t>;
    };
    
    template<typename t_0>
    concept hash_sha_1 =
    requires
    {
        requires hash<t_0>;
    } &&
    requires
    {
        typename t_0::properties::hash_sha_1;
    };
    
    template<typename t_0>
    concept hash_sha_2 =
    requires
    {
        requires hash<t_0>;
    } &&
    requires
    {
        typename t_0::properties::hash_sha_2;
    };
    
    /*
     * Start mac.
     */
    
    template<typename t_0>
    concept mac =
    requires
    {
        requires constructible_noexcept<t_0>;
        requires destructible_noexcept<t_0>;
    } &&
    requires
    {
        typename t_0::properties::mac;
    } &&
    requires
    {
        t_0::key_size;
        t_0::digest_size;
    } &&
    requires(t_0 impl)
    {
        {
        impl.initialize(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.initialize(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.update(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.update(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.finalize()
        } noexcept -> same<void>;
        {
        impl.digest(
            static_cast<char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<unsigned char*>(nullptr)
        )
        } noexcept -> same<size_t>;
    };
    
    template<typename t_0>
    concept mac_hmac =
    requires
    {
        requires mac<t_0>;
    } &&
    requires
    {
        typename t_0::properties::mac_hmac;
    };
    
    template<typename t_0>
    concept mac_poly_1305 =
    requires
    {
        requires mac<t_0>;
    } &&
    requires
    {
        typename t_0::properties::mac_poly_1305;
    };
    
    /*
     * Start prf.
     */
    
    template<typename t_0>
    concept prf =
    requires
    {
        requires constructible_noexcept<t_0>;
        requires destructible_noexcept<t_0>;
    } &&
    requires
    {
        typename t_0::properties::prf;
    } &&
    requires
    {
        t_0::key_size;
        t_0::digest_size;
    } &&
    requires(t_0 impl)
    {
        {
        impl.initialize(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.initialize(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.update(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.update(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.finalize()
        } noexcept -> same<void>;
        {
        impl.digest(
            static_cast<char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<unsigned char*>(nullptr)
        )
        } noexcept -> same<size_t>;
    };
    
    template<typename t_0>
    concept prf_kdf_pbkdf_2 =
    requires
    {
        requires prf<t_0>;
    } &&
    requires
    {
        requires t_0::key_size == 0;
    };
    
    /*
     * Start kdf.
     */
    
    template<typename t_0>
    concept kdf =
    requires
    {
        requires constructible_noexcept<t_0>;
        requires destructible_noexcept<t_0>;
    } &&
    requires
    {
        typename t_0::properties::kdf;
    } &&
    requires(t_0 impl)
    {
        {
        impl.derive(
            static_cast<char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.derive(
            static_cast<unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
    };
    
    template<typename t_0>
    concept kdf_pbkdf_2 =
    requires
    {
        requires kdf<t_0>;
    } &&
    requires
    {
        typename t_0::properties::kdf_pbkdf_2;
    } &&
    requires(t_0 impl)
    {
        {
        impl.initialize(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<const char*>(nullptr),
            size_t(0),
            uint32_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.initialize(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            uint32_t(0)
        )
        } noexcept -> same<void>;
    };
    
    template<typename t_0>
    concept kdf_hkdf =
    requires
    {
        requires kdf<t_0>;
    } &&
    requires
    {
        typename t_0::properties::kdf_hkdf;
    } &&
    requires(t_0 impl)
    {
        {
        impl.initialize(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.initialize(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
    };
    
    /*
     * Start cipher.
     */
    
    template<typename t_0>
    concept cipher =
    requires
    {
        requires constructible_noexcept<t_0>;
        requires destructible_noexcept<t_0>;
    } &&
    requires
    {
        typename t_0::properties::cipher;
    } &&
    requires
    {
        t_0::key_size;
        t_0::iv_size;
        t_0::block_size;
    } &&
    requires(t_0 impl)
    {
        {
        impl.initialize(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.initialize(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.encrypt(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.encrypt(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<unsigned char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.encrypt_size(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.encrypt_size(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.decrypt(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.decrypt(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<unsigned char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.decrypt_size(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.decrypt_size(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
    };
    
    template<typename t_0>
    concept cipher_block =
    requires
    {
        requires cipher<t_0>;
    } &&
    requires
    {
        typename t_0::properties::cipher_block;
    };
    
    template<typename t_0>
    concept cipher_stream =
    requires
    {
        requires cipher<t_0>;
    } &&
    requires
    {
        typename t_0::properties::cipher_stream;
    };
    
    template<typename t_0>
    concept cipher_chacha_20 =
    requires
    {
        requires cipher_stream<t_0>;
    } &&
    requires
    {
        typename t_0::properties::cipher_chacha_20;
    } &&
    requires
    {
        requires t_0::key_size == 32;
        requires t_0::iv_size == 16;
    };
    
    /*
     * Start aead.
     */
    
    template<typename t_0>
    concept aead =
    requires
    {
        requires constructible_noexcept<t_0>;
        requires destructible_noexcept<t_0>;
    } &&
    requires
    {
        typename t_0::properties;
        typename t_0::properties::aead;
    } &&
    requires
    {
        typename t_0::cipher_type;
        typename t_0::mac_type;
    } &&
    requires
    {
        {
        typename t_0::cipher_type()
        } noexcept -> cipher;
        {
        typename t_0::mac_type()
        } noexcept -> mac;
    } &&
    requires
    {
        t_0::key_size;
        t_0::iv_size;
        t_0::digest_size;
    } &&
    requires(t_0 impl)
    {
        {
        impl.initialize(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.initialize(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.aad(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.aad(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.aadover()
        } noexcept -> same<void>;
        {
        impl.encrypt(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.encrypt(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<unsigned char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.encrypt_size(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.encrypt_size(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.decrypt(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.decrypt(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<unsigned char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.decrypt_size(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.decrypt_size(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.finalize()
        } noexcept -> same<void>;
        {
        impl.digest(
            static_cast<char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<unsigned char*>(nullptr)
        )
        } noexcept -> same<size_t>;
    };
    
    template<typename t_0>
    concept aead_chacha_20_poly_1305 =
    requires
    {
        requires aead<t_0>;
    } &&
    requires
    {
        typename t_0::properties::aead_chacha_20_poly_1305;
    } &&
    requires
    {
        {
        typename t_0::ciph_type()
        } noexcept -> cipher_chacha_20;
        {
        typename t_0::mac_type()
        } noexcept -> mac_poly_1305;
    } &&
    requires
    {
        requires t_0::key_size == 32;
        requires t_0::iv_size == 12;
        requires t_0::digest_size == 16;
    };
}
