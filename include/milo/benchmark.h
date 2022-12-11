

#pragma once


#include <chrono>
#include <ostream>

#include <milo/internal.h>


namespace milo::benchmark
{
    class result
    {
    public:
        
        uint64_t m_time;
    
    public:
        
        constexpr result() noexcept(true) = default;
        
        constexpr result(result&& object) noexcept(true) = default;
        
        constexpr result(const result& object) noexcept(true) = default;
        
        constexpr ~result() noexcept(true) = default;
    
    public:
        
        explicit
        constexpr result(
            uint64_t a_time
        ) noexcept(true)
        {
            m_time = a_time;
        }
    
    public:
        
        constexpr auto
        operator =(
            const result& object
        ) noexcept(true) -> result& = default;
    };
    
    template<
        typename t_invocable,
        typename... t_args
    >
    constexpr auto
    run(
        t_invocable&& a_invocable,
        size_t a_repeats,
        t_args&& ... a_args
    ) noexcept(meta::invocable_noexcept<t_invocable, t_args...>) -> result
    {
        using clock_type = std::chrono::steady_clock;
        using guard_type = meta::invocable_result<t_invocable, t_args...>;
        
        static_assert(meta::integral<guard_type>);
        
        guard_type guard [[maybe_unused]] = 0;
        guard_type volatile guard_vol [[maybe_unused]];
        
        for (size_t i = 0; i < 32; i += 1)
        {
            guard += a_invocable(
                internal::forward<
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
                internal::forward<
                    t_args
                >(
                    a_args
                )...
            );
        }
        
        auto end = clock_type::now();
        
        guard_vol = guard;
        
        return result(
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count()
        );
    }
}
