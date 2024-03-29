
#pragma once


#include <milo/app/app.h>
#include <milo/app/dep.h>
#include <milo/app/pattern.h>
#include <milo/app/parameter.h>
#include <milo/app/type.h>


namespace milo::app::command::benchmark
{
    using memory_type = container::chars_dynamic;
    
    struct
    {
        uint64_t cpu_clock;
        
        uint64_t repeats_warm;
        
        uint64_t repeats_time;
    } options;
    
    template<
        typename t_invocable,
        typename... t_args
    >
    MILO_INTERNAL_ATTRIBUTE_INLINE(true)
    auto
    measure(
        size_t a_repeats_warm,
        size_t a_repeats_time,
        t_invocable&& a_invocable,
        t_args&& ... a_args
    ) noexcept(true) -> uint64_t
    {
        using clock_type = std::chrono::steady_clock;
        using guard_type = meta::invocable_result<t_invocable, t_args...>;
        
        static_assert(meta::integral<guard_type>);
        
        guard_type guard [[maybe_unused]] = 0;
        guard_type volatile guard_vol [[maybe_unused]];
        
        for (size_t i = 0; i < a_repeats_warm; i += 1)
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
        
        for (size_t i = 0; i < a_repeats_time; i += 1)
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
    MILO_INTERNAL_ATTRIBUTE_INLINE(true)
    auto
    measure(
        t_invocable&& a_invocable,
        t_args&& ... a_args
    ) noexcept(true) -> uint64_t
    {
        return measure(
            size_t(options.repeats_warm),
            size_t(options.repeats_time),
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
        typename t_container = std::vector<
            std::pair<
                std::string_view,
                std::function<t_signature>
            >
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
                container.end(),
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
        
        auto cpu_clock = options.cpu_clock;
        auto repeats_warm = options.repeats_warm;
        auto repeats_time = options.repeats_time;
        auto config = type::object{
            {
                "benchmark",
                {
                    {"repeats-warm", repeats_warm},
                    {"repeats-time", repeats_time}
                }}
        };
        
        if (cpu_clock != 0)
        {
            config["benchmark"]["cpu-clock"] = cpu_clock;
        }
        
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
            
            auto duration_nanoseconds_per_call = double(nanoseconds) / double(repeats_time);
            
            auto& result = results[name];
            result["config"] = config;
            result["metrics"]["duration"] = {
                {"nanoseconds_per_call", duration_nanoseconds_per_call}
            };
            
            if (processed != 0)
            {
                auto throughput_gigabytes_per_second = double(processed) / double(nanoseconds);
                auto throughput_megabytes_per_second = double(processed * 1000) / double(nanoseconds);
                
                result["metrics"]["throughput"] = {
                    {"gigabytes_per_second", throughput_gigabytes_per_second},
                    {"megabytes_per_second", throughput_megabytes_per_second}
                };
            }
            
            if (processed != 0 && cpu_clock != 0)
            {
                auto billion = 1000 * 1000 * 1000;
                auto cpu_cycles_per_call = double(cpu_clock * nanoseconds) / double(billion * repeats_time);
                auto cpu_cycles_per_byte = double(cpu_clock * nanoseconds) / double(billion * processed);
                
                result["metrics"]["cpu"] = {
                    {"cycles_per_call", cpu_cycles_per_call},
                    {"cycles_per_byte", cpu_cycles_per_byte}
                };
            }
        }
    }
    
    template<
        meta::primitive::codec t_impl
    >
    MILO_INTERNAL_ATTRIBUTE_INLINE(true)
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
        meta::primitive::codec t_impl
    >
    auto
    primitive_codec_encode(
        const memory_type& a_bytes
    )
    {
        auto encoded = primitive::codec::encode<
            t_impl,
            memory_type
        >(
            a_bytes
        );
        auto decoded = primitive::codec::decode<
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
            options.repeats_time * a_bytes.size()
        );
    }
    
    template<
        meta::primitive::codec t_impl
    >
    MILO_INTERNAL_ATTRIBUTE_INLINE(true)
    auto
    primitive_codec_decode_call(
        const char* a_from_ptr,
        size_t a_from_size,
        char* a_to_ptr,
        error& a_error
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
        meta::primitive::codec t_impl
    >
    auto
    primitive_codec_decode(
        const memory_type& a_bytes
    )
    {
        error error;
        
        auto encoded = primitive::codec::encode<
            t_impl,
            memory_type
        >(
            a_bytes
        );
        auto decoded = primitive::codec::decode<
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
            options.repeats_time * a_bytes.size()
        );
    }
    
    template<
        meta::primitive::hash t_impl
    >
    MILO_INTERNAL_ATTRIBUTE_INLINE(true)
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
        meta::primitive::hash t_impl
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
            options.repeats_time * a_message.size()
        );
    }
    
    template<
        meta::primitive::mac t_impl
    >
    MILO_INTERNAL_ATTRIBUTE_INLINE(true)
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
        meta::primitive::mac t_impl
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
            options.repeats_time * a_message.size()
        );
    }
    
    template<
        meta::primitive::kdf_hkdf t_impl
    >
    MILO_INTERNAL_ATTRIBUTE_INLINE(true)
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
        meta::primitive::kdf_hkdf t_impl
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
            options.repeats_time * a_key.size()
        );
    }
    
    template<
        meta::primitive::kdf_pbkdf_2 t_impl
    >
    MILO_INTERNAL_ATTRIBUTE_INLINE(true)
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
        meta::primitive::kdf_pbkdf_2 t_impl
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
            options.repeats_time * a_key.size()
        );
    }
    
    template<
        meta::primitive::cipher t_impl
    >
    MILO_INTERNAL_ATTRIBUTE_INLINE(true)
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
        meta::primitive::cipher t_impl
    >
    auto
    primitive_cipher_encrypt(
        const memory_type& a_message
    )
    {
        t_impl impl;
        
        container::chars_static<t_impl::key_size> key;
        container::chars_static<t_impl::iv_size> iv;
        
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
        
        auto ciphertext = primitive::cipher::encrypt<
            t_impl,
            memory_type
        >(
            key,
            iv,
            a_message
        );
        
        auto plaintext = primitive::cipher::decrypt<
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
            options.repeats_time * a_message.size()
        );
    }
    
    template<
        meta::primitive::cipher t_impl
    >
    MILO_INTERNAL_ATTRIBUTE_INLINE(true)
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
        meta::primitive::cipher t_impl
    >
    auto
    primitive_cipher_decrypt(
        const memory_type& a_bytes
    )
    {
        t_impl impl;
        
        container::chars_static<t_impl::key_size> key;
        container::chars_static<t_impl::iv_size> iv;
        
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
        
        auto ciphertext = primitive::cipher::encrypt<
            t_impl,
            memory_type
        >(
            key,
            iv,
            a_bytes
        );
        
        auto plaintext = primitive::cipher::decrypt<
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
            options.repeats_time * a_bytes.size()
        );
    }
    
    template<
        meta::primitive::aead t_impl
    >
    MILO_INTERNAL_ATTRIBUTE_INLINE(true)
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
        meta::primitive::aead t_impl
    >
    auto
    primitive_aead_encrypt(
        const memory_type& a_aad,
        const memory_type& a_bytes
    )
    {
        t_impl impl;
        
        container::chars_static<t_impl::key_size> key;
        container::chars_static<t_impl::iv_size> iv;
        container::chars_static<t_impl::digest_size> digest;
        
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
        
        auto [ciphertext, mac] = primitive::aead::encrypt<
            t_impl,
            memory_type
        >(
            key,
            iv,
            a_aad,
            a_bytes
        );
        
        auto plaintext = primitive::aead::decrypt<
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
            options.repeats_time * (a_aad.size() + a_bytes.size())
        );
    }
    
    template<
        meta::primitive::aead t_impl
    >
    MILO_INTERNAL_ATTRIBUTE_INLINE(true)
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
        meta::primitive::aead t_impl
    >
    auto
    primitive_aead_decrypt(
        const memory_type& a_aad,
        const memory_type& a_bytes
    )
    {
        t_impl impl;
        
        container::chars_static<t_impl::key_size> key;
        container::chars_static<t_impl::iv_size> iv;
        container::chars_static<t_impl::digest_size> digest;
        
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
        
        auto [ciphertext, mac] = primitive::aead::encrypt<
            t_impl,
            memory_type
        >(
            key,
            iv,
            a_aad,
            a_bytes
        );
        
        auto plaintext = primitive::aead::decrypt<
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
            options.repeats_time * (a_aad.size() + a_bytes.size())
        );
    }
    
    auto
    primitive(
    ) -> void
    {
        auto pattern = app::args.command();
        auto benchmarks = std::vector<
            std::pair<
                std::string_view,
                std::function<void()>
            >
        >{
            /*
             * @formatter:off
             */
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
                        declare<decltype(primitive_codec_encode<primitive::codec_any>)>(
                            {
                                {"codec-base-16-encode",                    primitive_codec_encode<primitive::codec_base_16>},
                                {"codec-base-16-decode",                    primitive_codec_decode<primitive::codec_base_16>},
                                {"codec-base-64-encode",                    primitive_codec_encode<primitive::codec_base_64>},
                                {"codec-base-64-decode",                    primitive_codec_decode<primitive::codec_base_64>},
                            },
                            {
                            }
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
                        declare<decltype(primitive_hash<primitive::hash_any>)>(
                            {
                                {"hash-sha-1-160",                          primitive_hash<primitive::hash_sha_1_160>},
                                {"hash-sha-2-224",                          primitive_hash<primitive::hash_sha_2_224>},
                                {"hash-sha-2-256",                          primitive_hash<primitive::hash_sha_2_256>},
                                {"hash-sha-2-384",                          primitive_hash<primitive::hash_sha_2_384>},
                                {"hash-sha-2-512",                          primitive_hash<primitive::hash_sha_2_512>},
                                {"hash-sha-2-512-224",                      primitive_hash<primitive::hash_sha_2_512_224>},
                                {"hash-sha-2-512-256",                      primitive_hash<primitive::hash_sha_2_512_256>},
                            },
                            {
                                {"hash-sha-1-160-sw",                       primitive_hash<primitive::hash_sha_1_160_sw>},
                                {"hash-sha-2-224-sw",                       primitive_hash<primitive::hash_sha_2_224_sw>},
                                {"hash-sha-2-256-sw",                       primitive_hash<primitive::hash_sha_2_256_sw>},
                                {"hash-sha-2-384-sw",                       primitive_hash<primitive::hash_sha_2_384_sw>},
                                {"hash-sha-2-512-sw",                       primitive_hash<primitive::hash_sha_2_512_sw>},
                                {"hash-sha-2-512-224-sw",                   primitive_hash<primitive::hash_sha_2_512_224_sw>},
                                {"hash-sha-2-512-256-sw",                   primitive_hash<primitive::hash_sha_2_512_256_sw>},
                                #if MILO_INTERNAL_ARCH_X86_64
                                {"hash-sha-1-160-hw-x86-64-ni",             primitive_hash<primitive::hash_sha_1_160_hw_x86_64_ni>},
                                {"hash-sha-2-224-hw-x86-64-ni",             primitive_hash<primitive::hash_sha_2_224_hw_x86_64_ni>},
                                {"hash-sha-2-256-hw-x86-64-ni",             primitive_hash<primitive::hash_sha_2_256_hw_x86_64_ni>}
                                #endif
                            }
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
                        declare<decltype(primitive_mac<primitive::mac_any>)>(
                            {
                                {"mac-hmac-sha-1-160",                      primitive_mac<primitive::mac_hmac_sha_1_160>},
                                {"mac-hmac-sha-2-224",                      primitive_mac<primitive::mac_hmac_sha_2_224>},
                                {"mac-hmac-sha-2-256",                      primitive_mac<primitive::mac_hmac_sha_2_256>},
                                {"mac-hmac-sha-2-384",                      primitive_mac<primitive::mac_hmac_sha_2_384>},
                                {"mac-hmac-sha-2-512",                      primitive_mac<primitive::mac_hmac_sha_2_512>},
                                {"mac-hmac-sha-2-512-224",                  primitive_mac<primitive::mac_hmac_sha_2_512_224>},
                                {"mac-hmac-sha-2-512-256",                  primitive_mac<primitive::mac_hmac_sha_2_512_256>},
                                {"mac-poly-1305",                           primitive_mac<primitive::mac_poly_1305>}
                            },
                            {
                                {"mac-hmac-sha-1-160-sw",                   primitive_mac<primitive::mac_hmac_sha_1_160_sw>},
                                {"mac-hmac-sha-2-224-sw",                   primitive_mac<primitive::mac_hmac_sha_2_224_sw>},
                                {"mac-hmac-sha-2-256-sw",                   primitive_mac<primitive::mac_hmac_sha_2_256_sw>},
                                {"mac-hmac-sha-2-384-sw",                   primitive_mac<primitive::mac_hmac_sha_2_384_sw>},
                                {"mac-hmac-sha-2-512-sw",                   primitive_mac<primitive::mac_hmac_sha_2_512_sw>},
                                {"mac-hmac-sha-2-512-224-sw",               primitive_mac<primitive::mac_hmac_sha_2_512_224_sw>},
                                {"mac-hmac-sha-2-512-256-sw",               primitive_mac<primitive::mac_hmac_sha_2_512_256_sw>},
                                {"mac-poly-1305-sw",                        primitive_mac<primitive::mac_poly_1305_sw>},
                                #if MILO_INTERNAL_ARCH_X86_64
                                {"mac-hmac-sha-1-160-hw-x86-64-ni",         primitive_mac<primitive::mac_hmac_sha_1_160_hw_x86_64_ni>},
                                {"mac-hmac-sha-2-224-hw-x86-64-ni",         primitive_mac<primitive::mac_hmac_sha_2_224_hw_x86_64_ni>},
                                {"mac-hmac-sha-2-256-hw-x86-64-ni",         primitive_mac<primitive::mac_hmac_sha_2_256_hw_x86_64_ni>}
                                #endif
                            }
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
                        declare<decltype(primitive_kdf<primitive::kdf_hkdf_any>)>(
                            {
                                {"kdf-hkdf-hmac-sha-1-160",                 primitive_kdf<primitive::kdf_hkdf_hmac_sha_1_160>},
                                {"kdf-hkdf-hmac-sha-2-224",                 primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_224>},
                                {"kdf-hkdf-hmac-sha-2-256",                 primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_256>},
                                {"kdf-hkdf-hmac-sha-2-384",                 primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_384>},
                                {"kdf-hkdf-hmac-sha-2-512",                 primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_512>},
                                {"kdf-hkdf-hmac-sha-2-512-224",             primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_512_224>},
                                {"kdf-hkdf-hmac-sha-2-512-256",             primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_512_256>}
                            },
                            {
                                {"kdf-hkdf-hmac-sha-1-160-sw",              primitive_kdf<primitive::kdf_hkdf_hmac_sha_1_160_sw>},
                                {"kdf-hkdf-hmac-sha-2-224-sw",              primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_224_sw>},
                                {"kdf-hkdf-hmac-sha-2-256-sw",              primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_256_sw>},
                                {"kdf-hkdf-hmac-sha-2-384-sw",              primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_384_sw>},
                                {"kdf-hkdf-hmac-sha-2-512-sw",              primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_512_sw>},
                                {"kdf-hkdf-hmac-sha-2-512-224-sw",          primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_512_224_sw>},
                                {"kdf-hkdf-hmac-sha-2-512-256-sw",          primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_512_256_sw>},
                                #if MILO_INTERNAL_ARCH_X86_64
                                {"kdf-hkdf-hmac-sha-1-160-hw-x86-64-ni",    primitive_kdf<primitive::kdf_hkdf_hmac_sha_1_160_hw_x86_64_ni>},
                                {"kdf-hkdf-hmac-sha-2-224-hw-x86-64-ni",    primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_224_hw_x86_64_ni>},
                                {"kdf-hkdf-hmac-sha-2-256-hw-x86-64-ni",    primitive_kdf<primitive::kdf_hkdf_hmac_sha_2_256_hw_x86_64_ni>}
                                #endif
                            }
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
                        declare<decltype(primitive_kdf<primitive::kdf_pbkdf_2_any>)>(
                            {
                                {"kdf-pbkdf-2-hmac-sha-1-160",              primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_1_160>},
                                {"kdf-pbkdf-2-hmac-sha-2-224",              primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_224>},
                                {"kdf-pbkdf-2-hmac-sha-2-256",              primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_256>},
                                {"kdf-pbkdf-2-hmac-sha-2-384",              primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_384>},
                                {"kdf-pbkdf-2-hmac-sha-2-512",              primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_512>},
                                {"kdf-pbkdf-2-hmac-sha-2-512-224",          primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_512_224>},
                                {"kdf-pbkdf-2-hmac-sha-2-512-256",          primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_512_256>}
                            },
                            {
                                {"kdf-pbkdf-2-hmac-sha-1-160-sw",           primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_1_160_sw>},
                                {"kdf-pbkdf-2-hmac-sha-2-224-sw",           primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_224_sw>},
                                {"kdf-pbkdf-2-hmac-sha-2-256-sw",           primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_256_sw>},
                                {"kdf-pbkdf-2-hmac-sha-2-384-sw",           primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_384_sw>},
                                {"kdf-pbkdf-2-hmac-sha-2-512-sw",           primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_512_sw>},
                                {"kdf-pbkdf-2-hmac-sha-2-512-224-sw",       primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_512_224_sw>},
                                {"kdf-pbkdf-2-hmac-sha-2-512-256-sw",       primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_512_256_sw>},
                                #if MILO_INTERNAL_ARCH_X86_64
                                {"kdf-pbkdf-2-hmac-sha-1-160-hw-x86-64-ni", primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_1_160_hw_x86_64_ni>},
                                {"kdf-pbkdf-2-hmac-sha-2-224-hw-x86-64-ni", primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_224_hw_x86_64_ni>},
                                {"kdf-pbkdf-2-hmac-sha-2-256-hw-x86-64-ni", primitive_kdf<primitive::kdf_pbkdf_2_hmac_sha_2_256_hw_x86_64_ni>}
                                #endif
                            }
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
                        declare<decltype(primitive_cipher_encrypt<primitive::cipher_any>)>(
                            {
                                {"cipher-chacha-20-encrypt",                primitive_cipher_encrypt<primitive::cipher_chacha_20>},
                                {"cipher-chacha-20-decrypt",                primitive_cipher_decrypt<primitive::cipher_chacha_20>},
                            },
                            {
                                {"cipher-chacha-20-sw-encrypt",             primitive_cipher_encrypt<primitive::cipher_chacha_20_sw>},
                                {"cipher-chacha-20-sw-decrypt",             primitive_cipher_decrypt<primitive::cipher_chacha_20_sw>},
                                #if MILO_INTERNAL_ARCH_X86_64
                                {"cipher-chacha-20-hw-x86-64-ssse-3-encrypt",   primitive_cipher_encrypt<primitive::cipher_chacha_20_hw_x86_64_ssse_3>},
                                {"cipher-chacha-20-hw-x86-64-ssse-3-decrypt",   primitive_cipher_decrypt<primitive::cipher_chacha_20_hw_x86_64_ssse_3>},
                                {"cipher-chacha-20-hw-x86-64-avx-2-encrypt",    primitive_cipher_encrypt<primitive::cipher_chacha_20_hw_x86_64_avx_2>},
                                {"cipher-chacha-20-hw-x86-64-avx-2-decrypt",    primitive_cipher_decrypt<primitive::cipher_chacha_20_hw_x86_64_avx_2>},
                                #endif
                            }
                        )
                    );
                }
            },
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
                        declare<decltype(primitive_aead_encrypt<primitive::aead_any>)>(
                            {
                                {"aead-chacha-20-poly-1305-encrypt",        primitive_aead_encrypt<primitive::aead_chacha_20_poly_1305>},
                                {"aead-chacha-20-poly-1305-decrypt",        primitive_aead_decrypt<primitive::aead_chacha_20_poly_1305>},
                            },
                            {
                                {"aead-chacha-20-sw-poly-1305-sw-encrypt",  primitive_aead_encrypt<primitive::aead_chacha_20_sw_poly_1305_sw>},
                                {"aead-chacha-20-sw-poly-1305-sw-decrypt",  primitive_aead_decrypt<primitive::aead_chacha_20_sw_poly_1305_sw>},
                            }
                        )
                    );
                }
            },
            /*
             * @formatter:on
             */
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
            .cpu_clock = app::args.parameter<uint64_t>(
                "cpu-clock",
                0
            ) * 1000000,
            .repeats_warm = app::args.parameter<uint64_t>(
                "repeats-warm",
                128
            ),
            .repeats_time = app::args.parameter<uint64_t>(
                "repeats-time",
                1024
            )
        };
        
        if (options.repeats_time == 0)
        {
            options.repeats_time = 1;
        }
        
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
