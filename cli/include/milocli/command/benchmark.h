
#pragma once


#include <chrono>
#include <iostream>

#include <milocli/app.h>
#include <milocli/dep.h>
#include <milocli/pattern.h>
#include <milocli/parameter.h>
#include <milocli/type.h>


namespace milocli::command::benchmark
{
    using memory_type = milo::container::chars_dynamic;
    
    struct
    {
        size_t repeats_warmup;
        
        size_t repeats_measure;
    } options;
    
    template<
        typename t_invocable,
        typename... t_args
    >
    MILO_INTERNAL_INLINE
    auto
    measure(
        size_t a_repeats_warmup,
        size_t a_repeats_measure,
        t_invocable&& a_invocable,
        t_args&& ... a_args
    ) noexcept(true) -> uint64_t
    {
        using clock_type = std::chrono::steady_clock;
        using guard_type = milo::meta::invocable_result<t_invocable, t_args...>;
        
        static_assert(milo::meta::integral<guard_type>);
        
        guard_type guard [[maybe_unused]] = 0;
        guard_type volatile guard_vol [[maybe_unused]];
        
        for (size_t i = 0; i < a_repeats_warmup; i += 1)
        {
            guard += a_invocable(
                std::forward<
                    t_args
                >(
                    a_args
                )...
            );
        }
        
        guard_vol = guard;
        
        auto beg = clock_type::now();
        
        for (size_t i = 0; i < a_repeats_measure; i += 1)
        {
            guard += a_invocable(
                std::forward<
                    t_args
                >(
                    a_args
                )...
            );
        }
        
        auto end = clock_type::now();
        
        guard_vol = guard;
        
        return std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count();
    }
    
    template<
        typename t_invocable,
        typename... t_args
    >
    auto
    measure(
        t_invocable&& a_invocable,
        t_args&& ... a_args
    ) noexcept(true) -> uint64_t
    {
        return measure(
            options.repeats_warmup,
            options.repeats_measure,
            a_invocable,
            std::forward<
                t_args
            >(
                a_args
            )...
        );
    }
    
    template<
        typename t_signature,
        typename t_container = std::map<
            std::string_view,
            std::function<t_signature>
        >
    >
    auto
    declare(
        std::initializer_list<typename t_container::value_type> a_basic,
        std::initializer_list<typename t_container::value_type> a_advanced
    )
    {
        t_container container(
            a_basic
        );
        
        if (app::options.advanced)
        {
            container.insert(
                a_advanced
            );
        }
        
        return container;
    }
    
    template<
        typename t_parameters,
        typename t_pattern,
        typename t_benchmarks
    >
    auto
    invoke(
        const t_pattern& a_pattern,
        const t_benchmarks& a_benchmarks
    ) -> void
    {
        if (a_pattern == "list")
        {
            app::options.suppress = true;
            
            for (const auto& [name, _]: a_benchmarks)
            {
                std::cout << name << "\n";
            }
            
            std::cout << std::flush;
            
            return;
        }
        
        if (std::find_if(
            a_benchmarks.begin(),
            a_benchmarks.end(),
            [&](auto item)
            {
                return pattern_match_star(
                    a_pattern,
                    item.first
                );
            }
        ) == a_benchmarks.end())
        {
            return;
        }
        
        auto repeats_warmup = options.repeats_warmup;
        auto repeats_measure = options.repeats_measure;
        auto config = type::object{
            {
                "benchmark",
                {
                    {"repeats-warmup", repeats_warmup},
                    {"repeats-measure", repeats_measure}
                }}
        };
        
        auto parameters = parameter::parse(
            t_parameters{},
            config["input"]
        );
        
        auto& results = app::context["result"]["benchmark"]["primitive"];
        
        for (const auto& [name, benchmark]: a_benchmarks)
        {
            if (!pattern_match_star(
                a_pattern,
                name
            ))
            {
                continue;
            }
            
            auto [
                nanoseconds,
                processed
            ] = std::apply(
                benchmark,
                parameters
            );
            
            auto duration_total = nanoseconds;
            auto duration_average = double(duration_total) / double(repeats_measure);
            
            auto& result = results[name];
            result["config"] = config;
            result["metrics"]["duration"] = {
                {"average", duration_average},
                {"total",   duration_total}
            };
            
            if (processed != -1)
            {
                auto throughput_gigabytes_per_second = double(processed) / double(duration_total);
                auto throughput_megabytes_per_second = double(processed * 1000) / double(duration_total);
                
                result["metrics"]["throughput"] = {
                    {"gigabytes_per_second", throughput_gigabytes_per_second},
                    {"megabytes_per_second", throughput_megabytes_per_second}
                };
            }
        }
    }
    
    template<
        milo::meta::primitive::codec t_impl
    >
    MILO_INTERNAL_INLINE
    auto
    primitive_codec_encode_call(
        const char* a_from_ptr,
        size_t a_from_size,
        char* a_to_ptr
    ) noexcept(true)
    {
        return t_impl::encode(
            a_from_ptr,
            a_from_size,
            a_to_ptr
        );
    }
    
    template<
        milo::meta::primitive::codec t_impl
    >
    MILO_INTERNAL_INLINE
    auto
    primitive_codec_decode_call(
        const char* a_from_ptr,
        size_t a_from_size,
        char* a_to_ptr,
        milo::error& a_error
    ) noexcept(true)
    {
        return t_impl::decode(
            a_from_ptr,
            a_from_size,
            a_to_ptr,
            a_error
        );
    }
    
    template<
        milo::meta::primitive::hash t_impl
    >
    MILO_INTERNAL_INLINE
    auto
    primitive_hash_call(
        t_impl& a_impl,
        const char* a_message_ptr,
        size_t a_message_size
    ) noexcept(true)
    {
        char digest[t_impl::digest_size];
        
        a_impl.initialize();
        a_impl.update(
            a_message_ptr,
            a_message_size
        );
        a_impl.finalize();
        a_impl.digest(
            digest
        );
        
        return digest[0];
    }
    
    template<
        milo::meta::primitive::mac t_impl
    >
    MILO_INTERNAL_INLINE
    auto
    primitive_mac_call(
        t_impl& a_impl,
        const char* a_key_ptr,
        size_t a_key_size,
        const char* a_message_ptr,
        size_t a_message_size
    ) noexcept(true)
    {
        char digest[t_impl::digest_size];
        
        a_impl.initialize(
            a_key_ptr,
            a_key_size
        );
        a_impl.update(
            a_message_ptr,
            a_message_size
        );
        a_impl.finalize();
        a_impl.digest(
            digest
        );
        
        return digest[0];
    }
    
    template<
        milo::meta::primitive::kdf_hkdf t_impl
    >
    auto
    primitive_kdf_call(
        t_impl& a_impl,
        const char* a_ikm_ptr,
        size_t a_ikm_size,
        const char* a_salt_ptr,
        size_t a_salt_size,
        const char* a_info_ptr,
        size_t a_info_size,
        char* a_key_ptr,
        size_t a_key_size
    )
    {
        a_impl.initialize(
            a_ikm_ptr,
            a_ikm_size,
            a_salt_ptr,
            a_salt_size,
            a_info_ptr,
            a_info_size
        );
        a_impl.derive(
            a_key_ptr,
            a_key_size
        );
        
        return a_key_ptr[0];
    }
    
    template<
        milo::meta::primitive::kdf_pbkdf_2 t_impl
    >
    auto
    primitive_kdf_call(
        t_impl& a_impl,
        const char* a_ikm_ptr,
        size_t a_ikm_size,
        const char* a_salt_ptr,
        size_t a_salt_size,
        size_t a_iterations,
        char* a_key_ptr,
        size_t a_key_size
    )
    {
        a_impl.initialize(
            a_ikm_ptr,
            a_ikm_size,
            a_salt_ptr,
            a_salt_size,
            a_iterations
        );
        a_impl.derive(
            a_key_ptr,
            a_key_size
        );
        
        return a_key_ptr[0];
    }
    
    template<
        milo::meta::primitive::cipher t_impl
    >
    auto
    primitive_cipher_encrypt_call(
        t_impl& a_impl,
        const char* a_key_ptr,
        size_t a_key_size,
        const char* a_iv_ptr,
        size_t a_iv_size,
        const char* a_plaintext_ptr,
        size_t a_plaintext_size,
        char* a_ciphertext_ptr
    )
    {
        a_impl.initialize(
            a_key_ptr,
            a_key_size,
            a_iv_ptr,
            a_iv_size
        );
        
        return a_impl.encrypt(
            a_plaintext_ptr,
            a_plaintext_size,
            a_ciphertext_ptr
        );
    }
    
    template<
        milo::meta::primitive::cipher t_impl
    >
    auto
    primitive_cipher_decrypt_call(
        t_impl& a_impl,
        const char* a_key_ptr,
        size_t a_key_size,
        const char* a_iv_ptr,
        size_t a_iv_size,
        const char* a_ciphertext_ptr,
        size_t a_ciphertext_size,
        char* a_plaintext_ptr
    )
    {
        a_impl.initialize(
            a_key_ptr,
            a_key_size,
            a_iv_ptr,
            a_iv_size
        );
        
        return a_impl.decrypt(
            a_ciphertext_ptr,
            a_ciphertext_size,
            a_plaintext_ptr
        );
    }
    
    template<
        milo::meta::primitive::codec t_impl
    >
    auto
    primitive_codec_encode(
        const memory_type& a_bytes
    )
    {
        auto encoded = milo::primitive::codec::encode<
            t_impl,
            memory_type
        >(
            a_bytes
        );
        auto decoded = milo::primitive::codec::decode<
            t_impl,
            memory_type
        >(
            encoded
        );
        
        return std::make_tuple(
            measure(
                primitive_codec_encode_call<t_impl>,
                decoded.data(),
                decoded.size(),
                encoded.data()
            ),
            options.repeats_measure * a_bytes.size()
        );
    }
    
    template<
        milo::meta::primitive::codec t_impl
    >
    auto
    primitive_codec_decode(
        const memory_type& a_bytes
    )
    {
        milo::error error;
        
        auto encoded = milo::primitive::codec::encode<
            t_impl,
            memory_type
        >(
            a_bytes
        );
        auto decoded = milo::primitive::codec::decode<
            t_impl,
            memory_type
        >(
            encoded
        );
        
        return std::make_tuple(
            measure(
                primitive_codec_decode_call<t_impl>,
                encoded.data(),
                encoded.size(),
                decoded.data(),
                error
            ),
            options.repeats_measure * a_bytes.size()
        );
    }
    
    template<
        milo::meta::primitive::hash t_impl
    >
    auto
    primitive_hash(
        const memory_type& a_message
    )
    {
        t_impl impl;
        
        return std::make_tuple(
            measure(
                primitive_hash_call<t_impl>,
                impl,
                a_message.data(),
                a_message.size()
            ),
            options.repeats_measure * a_message.size()
        );
    }
    
    template<
        milo::meta::primitive::mac t_impl
    >
    auto
    primitive_mac(
        const memory_type& a_key,
        const memory_type& a_message
    )
    {
        t_impl impl;
        
        return std::make_tuple(
            measure(
                primitive_mac_call<t_impl>,
                impl,
                a_key.data(),
                a_key.size(),
                a_message.data(),
                a_message.size()
            ),
            options.repeats_measure * a_message.size()
        );
    }
    
    template<
        milo::meta::primitive::kdf_hkdf t_impl
    >
    auto
    primitive_kdf(
        const memory_type& a_ikm,
        const memory_type& a_salt,
        const memory_type& a_info,
        memory_type& a_key
    )
    {
        t_impl impl;
        
        return std::make_tuple(
            measure(
                primitive_kdf_call<t_impl>,
                impl,
                a_ikm.data(),
                a_ikm.size(),
                a_salt.data(),
                a_salt.size(),
                a_info.data(),
                a_info.size(),
                a_key.data(),
                a_key.size()
            ),
            options.repeats_measure * a_key.size()
        );
    }
    
    template<
        milo::meta::primitive::kdf_pbkdf_2 t_impl
    >
    auto
    primitive_kdf(
        const memory_type& a_ikm,
        const memory_type& a_salt,
        size_t a_iterations,
        memory_type& a_key
    )
    {
        t_impl impl;
        
        return std::make_tuple(
            measure(
                primitive_kdf_call<t_impl>,
                impl,
                a_ikm.data(),
                a_ikm.size(),
                a_salt.data(),
                a_salt.size(),
                a_iterations,
                a_key.data(),
                a_key.size()
            ),
            options.repeats_measure * a_key.size()
        );
    }
    
    template<
        milo::meta::primitive::cipher t_impl
    >
    auto
    primitive_cipher_encrypt(
        const memory_type& a_message
    )
    {
        t_impl impl;
        
        milo::container::chars_static<t_impl::key_size> key;
        milo::container::chars_static<t_impl::iv_size> iv;
        
        std::iota(
            key.begin(),
            key.end(),
            0
        );
        std::iota(
            iv.begin(),
            iv.end(),
            0
        );
        
        auto ciphertext = milo::primitive::cipher::encrypt<
            t_impl,
            memory_type
        >(
            key,
            iv,
            a_message
        );
        
        auto plaintext = milo::primitive::cipher::decrypt<
            t_impl,
            memory_type
        >(
            key,
            iv,
            ciphertext
        );
        
        return std::make_tuple(
            measure(
                primitive_cipher_encrypt_call<t_impl>,
                impl,
                key.data(),
                key.size(),
                iv.data(),
                iv.size(),
                plaintext.data(),
                plaintext.size(),
                ciphertext.data()
            ),
            options.repeats_measure * a_message.size()
        );
    }
    
    template<
        milo::meta::primitive::cipher t_impl
    >
    auto
    primitive_cipher_decrypt(
        const memory_type& a_bytes
    )
    {
        t_impl impl;
        
        milo::container::chars_static<t_impl::key_size> key;
        milo::container::chars_static<t_impl::iv_size> iv;
        
        std::iota(
            key.begin(),
            key.end(),
            0
        );
        std::iota(
            iv.begin(),
            iv.end(),
            0
        );
        
        auto ciphertext = milo::primitive::cipher::encrypt<
            t_impl,
            memory_type
        >(
            key,
            iv,
            a_bytes
        );
        
        auto plaintext = milo::primitive::cipher::decrypt<
            t_impl,
            memory_type
        >(
            key,
            iv,
            ciphertext
        );
        
        return std::make_tuple(
            measure(
                primitive_cipher_decrypt_call<t_impl>,
                impl,
                key.data(),
                key.size(),
                iv.data(),
                iv.size(),
                ciphertext.data(),
                ciphertext.size(),
                plaintext.data()
            ),
            options.repeats_measure * a_bytes.size()
        );
    }
    
    template<
        milo::meta::primitive::aead t_impl
    >
    auto
    primitive_aead_encrypt_call(
        t_impl& a_impl,
        const char* a_key_ptr,
        size_t a_key_size,
        const char* a_iv_ptr,
        size_t a_iv_size,
        const char* a_aad_ptr,
        size_t a_aad_size,
        const char* a_plaintext_ptr,
        size_t a_plaintext_size,
        char* a_ciphertext_ptr,
        char* a_digest_ptr
    )
    {
        a_impl.initialize(
            a_key_ptr,
            a_key_size,
            a_iv_ptr,
            a_iv_size
        );
        a_impl.aad(
            a_aad_ptr,
            a_aad_size
        );
        a_impl.aadover();
        a_impl.encrypt(
            a_plaintext_ptr,
            a_plaintext_size,
            a_ciphertext_ptr
        );
        a_impl.finalize();
        
        return a_impl.digest(
            a_digest_ptr
        );
    }
    
    template<
        milo::meta::primitive::aead t_impl
    >
    auto
    primitive_aead_encrypt(
        const memory_type& a_aad,
        const memory_type& a_bytes
    )
    {
        t_impl impl;
        
        milo::container::chars_static<t_impl::key_size> key;
        milo::container::chars_static<t_impl::iv_size> iv;
        milo::container::chars_static<t_impl::digest_size> digest;
        
        std::iota(
            key.begin(),
            key.end(),
            0
        );
        std::iota(
            iv.begin(),
            iv.end(),
            0
        );
        
        auto [ciphertext, mac] = milo::primitive::aead::encrypt<
            t_impl,
            memory_type
        >(
            key,
            iv,
            a_aad,
            a_bytes
        );
        
        auto plaintext = milo::primitive::aead::decrypt<
            t_impl,
            memory_type
        >(
            key,
            iv,
            a_aad,
            ciphertext,
            mac
        );
        
        return std::make_tuple(
            measure(
                primitive_aead_encrypt_call<t_impl>,
                impl,
                key.data(),
                key.size(),
                iv.data(),
                iv.size(),
                a_aad.data(),
                a_aad.size(),
                plaintext.data(),
                plaintext.size(),
                ciphertext.data(),
                digest.data()
            ),
            options.repeats_measure * (a_aad.size() + a_bytes.size())
        );
    }
    
    template<
        milo::meta::primitive::aead t_impl
    >
    auto
    primitive_aead_decrypt_call(
        t_impl& a_impl,
        const char* a_key_ptr,
        size_t a_key_size,
        const char* a_iv_ptr,
        size_t a_iv_size,
        const char* a_aad_ptr,
        size_t a_aad_size,
        const char* a_ciphertext_ptr,
        size_t a_ciphertext_size,
        char* a_plaintext_ptr,
        char* a_digest_ptr
    )
    {
        a_impl.initialize(
            a_key_ptr,
            a_key_size,
            a_iv_ptr,
            a_iv_size
        );
        a_impl.aad(
            a_aad_ptr,
            a_aad_size
        );
        a_impl.aadover();
        a_impl.decrypt(
            a_ciphertext_ptr,
            a_ciphertext_size,
            a_plaintext_ptr
        );
        a_impl.finalize();
        
        return a_impl.digest(
            a_digest_ptr
        );
    }
    
    template<
        milo::meta::primitive::aead t_impl
    >
    auto
    primitive_aead_decrypt(
        const memory_type& a_aad,
        const memory_type& a_bytes
    )
    {
        t_impl impl;
        
        milo::container::chars_static<t_impl::key_size> key;
        milo::container::chars_static<t_impl::iv_size> iv;
        milo::container::chars_static<t_impl::digest_size> digest;
        
        std::iota(
            key.begin(),
            key.end(),
            0
        );
        std::iota(
            iv.begin(),
            iv.end(),
            0
        );
        
        auto [ciphertext, mac] = milo::primitive::aead::encrypt<
            t_impl,
            memory_type
        >(
            key,
            iv,
            a_aad,
            a_bytes
        );
        
        auto plaintext = milo::primitive::aead::decrypt<
            t_impl,
            memory_type
        >(
            key,
            iv,
            a_aad,
            ciphertext,
            mac
        );
        
        return std::make_tuple(
            measure(
                primitive_aead_decrypt_call<t_impl>,
                impl,
                key.data(),
                key.size(),
                iv.data(),
                iv.size(),
                a_aad.data(),
                a_aad.size(),
                ciphertext.data(),
                ciphertext.size(),
                plaintext.data(),
                digest.data()
            ),
            options.repeats_measure * (a_aad.size() + a_bytes.size())
        );
    }
    
    auto
    primitive(
    ) -> void
    {
        auto pattern = app::args.command();
        auto benchmarks = std::map<
            std::string_view,
            std::function<void()>
        >{
            {
                "aead",
                [&]()
                {
                    invoke<
                        parameter::group<
                            parameter::parameter<"aad-size", size_t, parameter::convert<memory_type>>,
                            parameter::parameter<"bytes-size", size_t, parameter::convert<memory_type>>
                        >
                    >(
                        pattern,
                        declare<decltype(primitive_aead_encrypt<milo::primitive::aead::chacha_20_poly_1305<>>)>(
                            /*
                             * @formatter:off
                             */
                            {
                                {"aead-chacha-20-poly-1305-encrypt", primitive_aead_encrypt<milo::primitive::aead::chacha_20_poly_1305<>>},
                                {"aead-chacha-20-poly-1305-decrypt", primitive_aead_decrypt<milo::primitive::aead::chacha_20_poly_1305<>>},
                            },
                            {
                            }
                            /*
                             * @formatter:on
                             */
                        )
                    );
                }
            },
            {
                "codec",
                [&]()
                {
                    invoke<
                        parameter::group<
                            parameter::parameter<"bytes-size", size_t, parameter::convert<memory_type>>
                        >
                    >(
                        pattern,
                        declare<decltype(primitive_codec_encode<milo::primitive::codec::base_16<>>)>(
                            /*
                             * @formatter:off
                             */
                            {
                                {"codec-base-16-encode", primitive_codec_encode<milo::primitive::codec::base_16<>>},
                                {"codec-base-16-decode", primitive_codec_decode<milo::primitive::codec::base_16<>>},
                                {"codec-base-64-encode", primitive_codec_encode<milo::primitive::codec::base_64<>>},
                                {"codec-base-64-decode", primitive_codec_decode<milo::primitive::codec::base_64<>>},
                            },
                            {
                            }
                            /*
                             * @formatter:on
                             */
                        )
                    );
                }
            },
            {
                "hash",
                [&]()
                {
                    invoke<
                        parameter::group<
                            parameter::parameter<"message-size", size_t, parameter::convert<memory_type>>
                        >
                    >(
                        pattern,
                        declare<decltype(primitive_hash<milo::primitive::hash::sha_1_160<>>)>(
                            /*
                             * @formatter:off
                             */
                            {
                                {"hash-sha-1-160",              primitive_hash<milo::primitive::hash::sha_1_160<>>},
                                {"hash-sha-2-224",              primitive_hash<milo::primitive::hash::sha_2_224<>>},
                                {"hash-sha-2-256",              primitive_hash<milo::primitive::hash::sha_2_256<>>},
                                {"hash-sha-2-384",              primitive_hash<milo::primitive::hash::sha_2_384<>>},
                                {"hash-sha-2-512",              primitive_hash<milo::primitive::hash::sha_2_512<>>},
                                {"hash-sha-2-512-224",          primitive_hash<milo::primitive::hash::sha_2_512_224<>>},
                                {"hash-sha-2-512-256",          primitive_hash<milo::primitive::hash::sha_2_512_256<>>},
                            },
                            {
                                {"hash-sha-1-160-sw",           primitive_hash<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_sw>>>},
                                {"hash-sha-2-224-sw",           primitive_hash<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>},
                                {"hash-sha-2-256-sw",           primitive_hash<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>},
                                {"hash-sha-2-384-sw",           primitive_hash<milo::primitive::hash::sha_2_384<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>},
                                {"hash-sha-2-512-sw",           primitive_hash<milo::primitive::hash::sha_2_512<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>},
                                {"hash-sha-2-512-224-sw",       primitive_hash<milo::primitive::hash::sha_2_512_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>},
                                {"hash-sha-2-512-256-sw",       primitive_hash<milo::primitive::hash::sha_2_512_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>},
                                #if MILO_INTERNAL_ARCH_X86
                                {"hash-sha-1-160-hw-x86-v-1",   primitive_hash<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_1>>>},
                                {"hash-sha-1-160-hw-x86-v-2",   primitive_hash<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_2>>>},
                                {"hash-sha-2-224-hw-x86-v-1",   primitive_hash<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>},
                                {"hash-sha-2-256-hw-x86-v-1",   primitive_hash<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>},
                                #endif
                            }
                            /*
                             * @formatter:on
                             */
                        )
                    );
                }
            },
            {
                "mac",
                [&]()
                {
                    invoke<
                        parameter::group<
                            parameter::parameter<"key-size", size_t, parameter::convert<memory_type>>,
                            parameter::parameter<"message-size", size_t, parameter::convert<memory_type>>
                        >
                    >(
                        pattern,
                        declare<decltype(primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<>>>)>(
                            /*
                             * @formatter:off
                             */
                            {
                                {"mac-hmac-sha-1-160",              primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<>>>},
                                {"mac-hmac-sha-2-224",              primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_224<>>>},
                                {"mac-hmac-sha-2-256",              primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<>>>},
                                {"mac-hmac-sha-2-384",              primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_384<>>>},
                                {"mac-hmac-sha-2-512",              primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512<>>>},
                                {"mac-hmac-sha-2-512-224",          primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_224<>>>},
                                {"mac-hmac-sha-2-512-256",          primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_256<>>>},
                                {"mac-poly-1305",                   primitive_mac<milo::primitive::mac::poly_1305<>>}
                            },
                            {
                                {"mac-hmac-sha-1-160-sw",           primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_sw>>>>},
                                {"mac-hmac-sha-2-224-sw",           primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>>},
                                {"mac-hmac-sha-2-256-sw",           primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>>},
                                {"mac-hmac-sha-2-384-sw",           primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_384<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>},
                                {"mac-hmac-sha-2-512-sw",           primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>},
                                {"mac-hmac-sha-2-512-224-sw",       primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>},
                                {"mac-hmac-sha-2-512-256-sw",       primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>},
                                {"mac-poly-1305-sw",                primitive_mac<milo::primitive::mac::poly_1305<milo::option::impl_runtime<milo::primitive::detail::mac_poly_1305_impl_sw>>>},
                                #if MILO_INTERNAL_ARCH_X86
                                {"mac-hmac-sha-1-160-hw-x86-v-1",   primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_1>>>>},
                                {"mac-hmac-sha-1-160-hw-x86-v-2",   primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_2>>>>},
                                {"mac-hmac-sha-2-224-hw-x86-v-1",   primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>>},
                                {"mac-hmac-sha-2-256-hw-x86-v-1",   primitive_mac<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>>}
                                #endif
                            }
                            /*
                             * @formatter:on
                             */
                        )
                    );
                }
            },
            {
                "kdf-hkdf",
                [&]()
                {
                    invoke<
                        parameter::group<
                            parameter::parameter<"ikm-size", size_t, parameter::convert<memory_type>>,
                            parameter::parameter<"salt-size", size_t, parameter::convert<memory_type>>,
                            parameter::parameter<"info-size", size_t, parameter::convert<memory_type>>,
                            parameter::parameter<"key-size", size_t, parameter::convert<memory_type>>
                        >
                    >(
                        pattern,
                        declare<decltype(primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<>>>>)>(
                            /*
                             * @formatter:off
                             */
                            {
                                {"kdf-hkdf-hmac-sha-1-160",            primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<>>>>},
                                {"kdf-hkdf-hmac-sha-2-224",            primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_224<>>>>},
                                {"kdf-hkdf-hmac-sha-2-256",            primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<>>>>},
                                {"kdf-hkdf-hmac-sha-2-384",            primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_384<>>>>},
                                {"kdf-hkdf-hmac-sha-2-512",            primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512<>>>>},
                                {"kdf-hkdf-hmac-sha-2-512-224",        primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_224<>>>>},
                                {"kdf-hkdf-hmac-sha-2-512-256",        primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_256<>>>>}
                            },
                            {
                                {"kdf-hkdf-hmac-sha-1-160-sw",           primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_sw>>>>>},
                                {"kdf-hkdf-hmac-sha-2-224-sw",           primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>>>},
                                {"kdf-hkdf-hmac-sha-2-256-sw",           primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>>>},
                                {"kdf-hkdf-hmac-sha-2-384-sw",           primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_384<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>>},
                                {"kdf-hkdf-hmac-sha-2-512-sw",           primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>>},
                                {"kdf-hkdf-hmac-sha-2-512-224-sw",       primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>>},
                                {"kdf-hkdf-hmac-sha-2-512-256-sw",       primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>>},
                                #if MILO_INTERNAL_ARCH_X86
                                {"kdf-hkdf-hmac-sha-1-160-hw-x86-v-1",   primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_1>>>>>},
                                {"kdf-hkdf-hmac-sha-1-160-hw-x86-v-2",   primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_2>>>>>},
                                {"kdf-hkdf-hmac-sha-2-224-hw-x86-v-1",   primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>>>},
                                {"kdf-hkdf-hmac-sha-2-256-hw-x86-v-1",   primitive_kdf<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>>>}
                                #endif
                            }
                            /*
                             * @formatter:on
                             */
                        )
                    );
                }
            },
            {
                "kdf-pbkdf-2",
                [&]()
                {
                    invoke<
                        parameter::group<
                            parameter::parameter<"ikm-size", size_t, parameter::convert<memory_type>>,
                            parameter::parameter<"salt-size", size_t, parameter::convert<memory_type>>,
                            parameter::parameter<"iterations", size_t>,
                            parameter::parameter<"key-size", size_t, parameter::convert<memory_type>>
                        >
                    >(
                        pattern,
                        declare<decltype(primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<>>>>)>(
                            /*
                             * @formatter:off
                             */
                            {
                                {"kdf-pbkdf-2-hmac-sha-1-160",              primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-224",              primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_224<>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-256",              primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-384",              primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_384<>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-512",              primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512<>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-512-224",          primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_224<>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-512-256",          primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_256<>>>>}
                            },
                            {
                                {"kdf-pbkdf-2-hmac-sha-1-160-sw",           primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_sw>>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-224-sw",           primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-256-sw",           primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-384-sw",           primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_384<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-512-sw",           primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-512-224-sw",       primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-512-256-sw",       primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>>},
                                #if MILO_INTERNAL_ARCH_X86
                                {"kdf-pbkdf-2-hmac-sha-1-160-hw-x86-v-1",   primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_1>>>>>},
                                {"kdf-pbkdf-2-hmac-sha-1-160-hw-x86-v-2",   primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_2>>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-224-hw-x86-v-1",   primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>>>},
                                {"kdf-pbkdf-2-hmac-sha-2-256-hw-x86-v-1",   primitive_kdf<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>>>}
                                #endif
                            }
                            /*
                             * @formatter:on
                             */
                        )
                    );
                }
            },
            {
                "cipher",
                [&]()
                {
                    invoke<
                        parameter::group<
                            parameter::parameter<"bytes-size", size_t, parameter::convert<memory_type>>
                        >
                    >(
                        pattern,
                        declare<decltype(primitive_cipher_encrypt<milo::primitive::cipher::chacha_20<>>)>(
                            /*
                             * @formatter:off
                             */
                            {
                                {"cipher-chacha-20-encrypt", primitive_cipher_encrypt<milo::primitive::cipher::chacha_20<>>},
                                {"cipher-chacha-20-decrypt", primitive_cipher_decrypt<milo::primitive::cipher::chacha_20<>>},
                            },
                            {
                                {"cipher-chacha-20-encrypt-sw", primitive_cipher_encrypt<milo::primitive::cipher::chacha_20<milo::option::impl_runtime<milo::primitive::detail::cipher_chacha_20_impl_sw>>>},
                                {"cipher-chacha-20-decrypt-sw", primitive_cipher_decrypt<milo::primitive::cipher::chacha_20<milo::option::impl_runtime<milo::primitive::detail::cipher_chacha_20_impl_sw>>>},
                            }
                            /*
                             * @formatter:on
                             */
                        )
                    );
                }
            },
        };
        
        for (const auto& [id, callback]: benchmarks)
        {
            callback();
        }
    }
    
    auto
    run(
    ) -> void
    {
        options = {
            .repeats_warmup = app::args.parameter<size_t>(
                "repeats-warmup",
                128
            ),
            .repeats_measure = app::args.parameter<size_t>(
                "repeats-measure",
                1024
            )
        };
        
        auto benchmarks = app_callmap<void()>{
            {"primitive", primitive}
        };
        
        benchmarks[app::args.command()]();
        
        if (!app::options.suppress)
        {
            if (app::options.verbose)
            {
                std::cout << type::to_json(app::context) << std::endl;
            }
            else
            {
                std::cout << type::to_json(app::context["result"]) << std::endl;
            }
        }
    }
}
