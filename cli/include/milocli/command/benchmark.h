
#pragma once


#include <chrono>
#include <iostream>
#include <regex>

#include <milocli/app.h>
#include <milocli/dep.h>
#include <milocli/pattern.h>
#include <milocli/type.h>


namespace milocli::command::detail
{
    template<
        typename t_invocable,
        typename... t_args
    >
    MILO_INTERNAL_INLINE
    constexpr auto
    benchmark_run(
        size_t a_repeats,
        t_invocable&& a_invocable,
        t_args&& ... a_args
    ) noexcept(milo::meta::invocable_noexcept<t_invocable, t_args...>) -> uint64_t
    {
        using clock_type = std::chrono::steady_clock;
        using guard_type = milo::meta::invocable_result<t_invocable, t_args...>;
        
        static_assert(milo::meta::integral<guard_type>);
        
        guard_type guard [[maybe_unused]] = 0;
        guard_type volatile guard_vol [[maybe_unused]];
        
        for (size_t i = 0; i < 32; i += 1)
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
        
        for (size_t i = 0; i < a_repeats; i += 1)
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
    constexpr auto
    benchmark_primitive_hash_call_core(
        t_impl& impl,
        const char* a_ptr,
        size_t a_size
    ) noexcept(true)
    {
        char digest[t_impl::digest_size];
        
        impl.initialize();
        impl.update(
            a_ptr,
            a_size
        );
        impl.finalize();
        impl.digest(
            digest
        );
        
        return digest[0];
    }
    
    template<
        milo::meta::primitive::hash t_impl
    >
    constexpr auto
    benchmark_primitive_hash_call(
        size_t a_repeats,
        const std::string& a_message
    )
    {
        t_impl impl;
        
        return benchmark_run(
            a_repeats,
            benchmark_primitive_hash_call_core<t_impl>,
            impl,
            a_message.data(),
            a_message.size()
        );
    }
    
    auto
    benchmark_primitive_hash(
        app_args& a_args,
        type::object& a_config,
        type::object& a_result
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
            {"sha-1-160",            benchmark_primitive_hash_call<milo::primitive::hash::sha_1_160<>>},
            {"sha-1-160-sw",         benchmark_primitive_hash_call<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_sw>>>},
            {"sha-1-160-hw", benchmark_primitive_hash_call<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_1>>>},
            {"sha-1-160-hw-x86-v-1", benchmark_primitive_hash_call<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_1>>>},
            {"sha-1-160-hw-x86-v-2", benchmark_primitive_hash_call<milo::primitive::hash::sha_1_160<milo::option::impl_runtime<milo::primitive::detail::hash_sha_1_160_impl_hw_x86_v_2>>>},
            {"sha-2-224",            benchmark_primitive_hash_call<milo::primitive::hash::sha_2_224<>>},
            {"sha-2-224-sw",         benchmark_primitive_hash_call<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>},
            {"sha-2-224-hw-x86-v-1", benchmark_primitive_hash_call<milo::primitive::hash::sha_2_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>},
            {"sha-2-256",            benchmark_primitive_hash_call<milo::primitive::hash::sha_2_256<>>},
            {"sha-2-256-sw",         benchmark_primitive_hash_call<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_sw>>>},
            {"sha-2-256-hw-x86-v-1", benchmark_primitive_hash_call<milo::primitive::hash::sha_2_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_256_impl_hw_x86_v_1>>>},
            {"sha-2-384",            benchmark_primitive_hash_call<milo::primitive::hash::sha_2_384<>>},
            {"sha-2-384-sw",         benchmark_primitive_hash_call<milo::primitive::hash::sha_2_384<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>},
            {"sha-2-512",            benchmark_primitive_hash_call<milo::primitive::hash::sha_2_512<>>},
            {"sha-2-512-sw",         benchmark_primitive_hash_call<milo::primitive::hash::sha_2_512<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>},
            {"sha-2-512-224",        benchmark_primitive_hash_call<milo::primitive::hash::sha_2_512_224<>>},
            {"sha-2-512-224-sw",     benchmark_primitive_hash_call<milo::primitive::hash::sha_2_512_224<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>},
            {"sha-2-512-256",        benchmark_primitive_hash_call<milo::primitive::hash::sha_2_512_256<>>},
            {"sha-2-512-256-sw",     benchmark_primitive_hash_call<milo::primitive::hash::sha_2_512_256<milo::option::impl_runtime<milo::primitive::detail::hash_sha_2_512_impl_sw>>>}
            /*
             * @formatter:on
             */
        };
        
        /*
         * Benchmark result.
         */
        auto& result = a_result["primitive"]["hash"];
        
        /*
     * Benchmark control.
         */
        auto message_size = a_args.parameter<size_t>("message-size");
        auto pattern = a_args.command();
        
        /*
         * Benchmark parameters.
         */
        auto repeats = a_config["repeats"].as<size_t>();
        auto message = std::string(
            message_size,
        ' '
        );
        
        auto invoke = [&](
            auto& a_name,
            auto& a_benchmark
        )
        {
            auto nanoseconds = a_benchmark(
                repeats,
                message
            );
            
            auto duration_total = nanoseconds;
            auto duration_average = double(duration_total) / double(repeats);
            
            auto processed_bytes = repeats * message_size;
            auto throughput_gigabytes_per_second = double(processed_bytes) / duration_total;
            auto throughput_megabytes_per_second = double(processed_bytes * 1000) / duration_total;
            
            result[a_name] = {
                {
                    "config",
                    {
                        {"repeats", repeats},
                        {"message_size", message_size}
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
        };
        
        for (const auto& [name, benchmark]: benchmarks)
        {
            if (pattern_match_star(
                pattern,
                name
            ))
            {
                invoke(
                    name,
                    benchmark
                );
            }
        }
    }
    
    auto
    benchmark_primitive(
        app_args& a_args,
        type::object& a_config,
        type::object& a_result
    ) -> void
    {
        auto benchmarks = app_callmap<void(
            app_args&,
            type::object&,
            type::object&
        )>{
            {"hash", benchmark_primitive_hash}
        };
        
        benchmarks[a_args.command()](
            a_args,
            a_config,
            a_result
        );
    }
}

namespace milocli::command
{
    auto
    benchmark(
        app_args& a_args,
        app_context& a_context
    ) -> void
    {
        auto& result = a_context["result"]["benchmark"];
        auto config = type::object();
        
        config["repeats"] = a_args.parameter<size_t>("repeats");
        
        auto benchmarks = app_callmap<void(
            app_args&,
            type::object&,
            type::object&
        )>{
            {"primitive", detail::benchmark_primitive}
        };
        
        benchmarks[a_args.command()](
            a_args,
            config,
            result
        );
        
        if (a_context["config"]["verbose"])
        {
            std::cout << type::to_json(a_context) << std::endl;
        }
        else
        {
            std::cout << type::to_json(a_context["result"]) << std::endl;
        }
    }
}
