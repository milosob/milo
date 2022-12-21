
#pragma once


#include <chrono>
#include <iostream>

#include <milocli/app.h>
#include <milocli/dep.h>
#include <milocli/pattern.h>
#include <milocli/type.h>


namespace milocli::command::benchmark
{
    struct
    {
        size_t iterations;
    } config;
    
    template<
        typename t_invocable,
        typename... t_args
    >
    MILO_INTERNAL_INLINE
    auto
    measure(
        size_t a_iterations,
        t_invocable&& a_invocable,
        t_args&& ... a_args
    ) noexcept(milo::meta::invocable_noexcept<t_invocable, t_args...>) -> uint64_t
    {
        using clock_type = std::chrono::steady_clock;
        using guard_type = milo::meta::invocable_result<t_invocable, t_args...>;
        
        static_assert(milo::meta::integral<guard_type>);
        
        guard_type guard [[maybe_unused]] = 0;
        guard_type volatile guard_vol [[maybe_unused]];
        
        for (size_t i = 0; i < 128; i += 1)
        {
            guard += a_invocable(
                milo::internal::forward<
                    t_args
                >(
                    a_args
                )...
            );
        }
        
        guard_vol = guard;
        
        /*
         * TODO
         *  Consider adding fences around clock calls for reliability.
         */
        
        auto beg = clock_type::now();
        
        for (size_t i = 0; i < a_iterations; i += 1)
        {
            guard += a_invocable(
                milo::internal::forward<
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
        milo::meta::primitive::hash t_impl
    >
    MILO_INTERNAL_INLINE
    auto
    primitive_hash_call_core(
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
        milo::meta::primitive::hash t_impl
    >
    auto
    primitive_hash_call(
        size_t a_iterations,
        const std::string& a_message
    )
    {
        t_impl impl;
        
        return measure(
            a_iterations,
            primitive_hash_call_core<t_impl>,
            impl,
            a_message.data(),
            a_message.size()
        );
    }
    
    auto
    primitive_hash(
    ) -> void
    {
        auto benchmarks = app_callmap<
            uint64_t(
                size_t,
                const std::string&
            )
        >{
            /*
             * @formatter:off
             */
            {"sha-1-160",            primitive_hash_call<milo::primitive::hash::sha_1_160<>>},
            {"sha-2-224",            primitive_hash_call<milo::primitive::hash::sha_2_224<>>},
            {"sha-2-256",            primitive_hash_call<milo::primitive::hash::sha_2_256<>>},
            {"sha-2-384",            primitive_hash_call<milo::primitive::hash::sha_2_384<>>},
            {"sha-2-512",            primitive_hash_call<milo::primitive::hash::sha_2_512<>>},
            {"sha-2-512-224",        primitive_hash_call<milo::primitive::hash::sha_2_512_224<>>},
            {"sha-2-512-256",        primitive_hash_call<milo::primitive::hash::sha_2_512_256<>>}
            /*
             * @formatter:on
             */
        };
        
        if (app::options.developer)
        {
            benchmarks.insert(
                {
                    /*
                     * @formatter:off
                     */
                    {"sha-1-160-sw",         primitive_hash_call<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_sw>>>},
                    {"sha-2-224-sw",         primitive_hash_call<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>},
                    {"sha-2-256-sw",         primitive_hash_call<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>},
                    {"sha-2-384-sw",         primitive_hash_call<milo::primitive::hash::sha_2_384<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>},
                    {"sha-2-512-sw",         primitive_hash_call<milo::primitive::hash::sha_2_512<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>},
                    {"sha-2-512-224-sw",     primitive_hash_call<milo::primitive::hash::sha_2_512_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>},
                    {"sha-2-512-256-sw",     primitive_hash_call<milo::primitive::hash::sha_2_512_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>}
                    /*
                     * @formatter:on
                     */
                }
            );
            
            if constexpr (MILO_INTERNAL_ARCH_X86)
            {
                benchmarks.insert(
                    {
                        /*
                         * @formatter:off
                         */
                        {"sha-1-160-hw-x86-v-1", primitive_hash_call<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_1>>>},
                        {"sha-1-160-hw-x86-v-2", primitive_hash_call<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_2>>>},
                        {"sha-2-224-hw-x86-v-1", primitive_hash_call<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>},
                        {"sha-2-256-hw-x86-v-1", primitive_hash_call<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>}
                        /*
                         * @formatter:on
                         */
                    }
                );
            }
        }
        
        if (app::args.peek() == "list")
        {
            for (const auto& [name, _]: benchmarks)
            {
                std::cout << name << "\n";
            }
            
            std::cout << std::flush;
            
            return;
        }
        
        /*
         * Result.
         */
        auto& result = app::context["result"]["benchmark"]["primitive"]["hash"];
        
        /*
         * Configuration.
         */
        auto message_size = app::args.parameter<size_t>("message-size");
        auto pattern = app::args.command();
        
        /*
         * Parameters.
         */
        auto iterations = config.iterations;
        auto message = std::string(
            message_size,
            ' '
        );
        
        for (const auto& [name, benchmark]: benchmarks)
        {
            if (!pattern_match_star(
                pattern,
                name
            ))
            {
                continue;
            }
            
            auto nanoseconds = benchmark(
                iterations,
                message
            );
            
            auto duration_total = nanoseconds;
            auto duration_average = double(duration_total) / double(iterations);
            
            auto processed_bytes = iterations * message_size;
            auto throughput_gigabytes_per_second = double(processed_bytes) / double(duration_total);
            auto throughput_megabytes_per_second = double(processed_bytes * 1000) / double(duration_total);
            
            /*
             * @formatter:off
             */
            result[name] = {
                {
                    "config",
                    {
                        {
                            "benchmark",
                            {
                                {"iterations", iterations}
                            }},
                        {
                            "input",
                            {
                                {"message_size", message_size}
                            }}
                    }},
                {
                    "metric",
                    {
                        {
                         "duration",
                            {
                                {"average", duration_average},
                                {"total", duration_total}
                            }},
                        {
                         "throughput",
                            {
                                {"gigabytes_per_second", throughput_gigabytes_per_second},
                                {"megabytes_per_second", throughput_megabytes_per_second}
                            }}
                    }},
            };
            /*
             * @formatter:on
             */
        }
        
        if (app::options.verbose)
        {
            std::cout << type::to_json(app::context) << std::endl;
        }
        else
        {
            std::cout << type::to_json(app::context["result"]) << std::endl;
        }
    }
    
    template<
        milo::meta::primitive::mac t_impl
    >
    MILO_INTERNAL_INLINE
    auto
    primitive_mac_call_core(
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
        milo::meta::primitive::mac t_impl
    >
    auto
    primitive_mac_call(
        size_t a_iterations,
        const std::string& a_key,
        const std::string& a_message
    )
    {
        t_impl impl;
        
        return measure(
            a_iterations,
            primitive_mac_call_core<t_impl>,
            impl,
            a_key.data(),
            a_key.size(),
            a_message.data(),
            a_message.size()
        );
    }
    
    auto
    primitive_mac(
    ) -> void
    {
        auto benchmarks = app_callmap<
            uint64_t(
                size_t,
                const std::string&,
                const std::string&
            )
        >{
            /*
             * @formatter:off
             */
            {"hmac-sha-1-160",            primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<>>>},
            {"hmac-sha-2-224",            primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_224<>>>},
            {"hmac-sha-2-256",            primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<>>>},
            {"hmac-sha-2-384",            primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_384<>>>},
            {"hmac-sha-2-512",            primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512<>>>},
            {"hmac-sha-2-512-224",        primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_224<>>>},
            {"hmac-sha-2-512-256",        primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_256<>>>},
            {"poly-1305",                 primitive_mac_call<milo::primitive::mac::poly_1305<>>}
            /*
             * @formatter:on
             */
        };
        
        if (app::options.developer)
        {
            benchmarks.insert(
                {
                    /*
                     * @formatter:off
                     */
                    {"hmac-sha-1-160-sw",         primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_sw>>>>},
                    {"hmac-sha-2-224-sw",         primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>>},
                    {"hmac-sha-2-256-sw",         primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>>},
                    {"hmac-sha-2-384-sw",         primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_384<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>},
                    {"hmac-sha-2-512-sw",         primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>},
                    {"hmac-sha-2-512-224-sw",     primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>},
                    {"hmac-sha-2-512-256-sw",     primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_512_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>>},
                    {"poly-1305-sw",              primitive_mac_call<milo::primitive::mac::poly_1305<milo::option::impl_runtime<milo::primitive::detail::mac_poly_1305_impl_sw>>>}
                    /*
                     * @formatter:on
                     */
                }
            );
            
            if constexpr (MILO_INTERNAL_ARCH_X86)
            {
                benchmarks.insert(
                    {
                        /*
                         * @formatter:off
                         */
                        {"hmac-sha-1-160-hw-x86-v-1", primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_1>>>>},
                        {"hmac-sha-1-160-hw-x86-v-2", primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_2>>>>},
                        {"hmac-sha-2-224-hw-x86-v-1", primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>>},
                        {"hmac-sha-2-256-hw-x86-v-1", primitive_mac_call<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>>}
                        /*
                         * @formatter:on
                         */
                    }
                );
            }
        }
        
        if (app::args.peek() == "list")
        {
            for (const auto& [name, _]: benchmarks)
            {
                std::cout << name << "\n";
            }
            
            std::cout << std::flush;
            
            return;
        }
        
        /*
         * Result.
         */
        auto& result = app::context["result"]["benchmark"]["primitive"]["mac"];
        
        /*
         * Configuration.
         */
        auto key_size = app::args.parameter<size_t>("key-size");
        auto message_size = app::args.parameter<size_t>("message-size");
        auto pattern = app::args.command();
        
        /*
         * Parameters.
         */
        auto iterations = config.iterations;
        auto message = std::string(
            message_size,
            ' '
        );
        auto key = std::string(
            key_size,
            ' '
        );
        
        for (const auto& [name, benchmark]: benchmarks)
        {
            if (!pattern_match_star(
                pattern,
                name
            ))
            {
                continue;
            }
            
            auto nanoseconds = benchmark(
                iterations,
                key,
                message
            );
            
            auto duration_total = nanoseconds;
            auto duration_average = double(duration_total) / double(iterations);
            
            auto processed_bytes = iterations * message_size;
            auto throughput_gigabytes_per_second = double(processed_bytes) / double(duration_total);
            auto throughput_megabytes_per_second = double(processed_bytes * 1000) / double(duration_total);
            
            /*
             * @formatter:off
             */
            result[name] = {
                {
                    "config",
                    {
                        {
                            "benchmark",
                            {
                                {"iterations", iterations}
                            }},
                        {
                            "input",
                            {
                                {"key_size", key_size},
                                {"message_size", message_size}
                            }}
                    }},
                {
                    "metric",
                    {
                        {
                            "duration",
                            {
                                {"average", duration_average},
                                {"total", duration_total}
                            }},
                        {
                            "throughput",
                            {
                                {"gigabytes_per_second", throughput_gigabytes_per_second},
                                {"megabytes_per_second", throughput_megabytes_per_second}
                            }}
                    }},
            };
            /*
             * @formatter:on
             */
        }
        
        if (app::options.verbose)
        {
            std::cout << type::to_json(app::context) << std::endl;
        }
        else
        {
            std::cout << type::to_json(app::context["result"]) << std::endl;
        }
    }
    
    auto
    primitive(
    ) -> void
    {
        auto benchmarks = app_callmap<void()>{
            {"hash", primitive_hash},
            {"mac",  primitive_mac},
        };
        
        benchmarks[app::args.command()]();
    }
    
    auto
    run(
    ) -> void
    {
        config = {
            .iterations = app::args.parameter<size_t>(
                "iterations",
                16384
            )
        };
        
        auto benchmarks = app_callmap<void()>{
            {"primitive", primitive}
        };
        
        benchmarks[app::args.command()]();
    }
}
