

#pragma once


#include <milo/meta.h>

#include <milo/internal/arch.h>
#include <milo/internal/option.h>
#include <milo/internal/utility.h>


namespace milo::internal
{
    template<
        typename t_impl
    >
    static
    constexpr auto
    impl_choose_check(
    ) noexcept(true) -> bool
    {
        return false;
    }
    
    template<
        typename t_impl
    >
    static
    constexpr auto
    impl_choose_check(
    ) noexcept(true) -> bool
    requires
    requires
    {
        requires meta::complex<t_impl>;
    }
    {
        return true;
    }
    
    template<
        typename t_chooser,
        typename t_impl,
        typename... t_impls
    >
    static
    constexpr auto
    impl_choose_hook(
    ) noexcept(true)
    {
        if constexpr (impl_choose_check<t_impl>())
        {
            if constexpr ((!
                requires
                {
                    typename t_impl::requirements::arch;
                }
            ))
            {
                return t_chooser::template hook<t_impl>();
            }
            else
            {
                if (arch_active::ise::check<t_impl>())
                {
                    return t_chooser::template hook<t_impl>();
                }
            }
        }
        
        if constexpr (sizeof...(t_impls) > 0)
        {
            return impl_choose_hook<
                t_chooser,
                t_impls...
            >();
        }
        else
        {
            /*
             * Unreachable.
             */
        }
    }
    
    template<
        typename t_chooser,
        typename... t_impls
    >
    struct impl_choose_type
        :
            meta::disjunction<
                meta::asbool<
                    impl_choose_check<t_impls>() &&
                    (
                        (!
                            requires
                            {
                                typename t_impls::requirements::arch;
                            }
                        )
                    ),
                    typename t_chooser::template type<t_impls>()
                >...
            >
    {
    };
    
    template<
        typename t_chooser,
        typename... t_impls
    >
    concept impl_choose_assert =
    requires
    {
        requires sizeof...(t_impls) > 0;
        requires impl_choose_type<
            t_chooser,
            t_impls...
        >::value;
    };
    
    template<
        typename... t_impls
    >
    using impl_cpltime = meta::args<t_impls...>;
    
    template<
        typename t_chooser,
        typename t_impl,
        typename... t_impls
    >
    struct impl_cpltime_reader
    {
        using impl = t_impl;
    };
    
    template<
        typename... t_impls
    >
    using impl_runtime = meta::args<t_impls...>;
    
    template<
        typename t_chooser,
        typename... t_impls
    >
    requires
    requires
    {
        requires impl_choose_assert<
            t_chooser,
            t_impls...
        >;
    }
    struct impl_runtime_reader
    {
        inline
        static
        const auto impl = impl_choose_hook<
            t_chooser,
            t_impls...
        >();
    };
    
    template<
        typename t_chooser,
        typename t_invoker,
        typename t_cpltime,
        typename t_runtime,
        typename... t_options
    >
    struct impl_proxy
    {
    private:
        
        using chooser = option_impl_chooser_suite::query_default_t<t_chooser, t_options...>;
        
        using invoker = option_impl_invoker_suite::query_default_t<t_invoker, t_options...>;
        
        using cpltime_override = option_impl_cpltime_suite::query_default_t<void, t_options...>;
        
        using cpltime_selected = typename meta::args_reader<
            t_cpltime,
            impl_cpltime_reader,
            chooser
        >::type;
        
        using runtime_prefer = option_impl_runtime_suite::query_default_t<void, t_options...>;
        
        using runtime_override = option_impl_enforce_suite::query_default_t<void, t_options...>;
        
        using runtime_selected = typename meta::args_reader<
            typename meta::args_push_front<
                t_runtime,
                runtime_prefer
            >::type,
            impl_runtime_reader,
            chooser
        >::type;
    
    public:
        
        template<
            unsigned t_id,
            typename... t_args
        >
        static
        constexpr auto
        invoke(
            t_args&& ... a_args
        ) noexcept(true)
        {
            if MILO_INTERNAL_CONSTEVAL
            {
                if constexpr (meta::complex<cpltime_override>)
                {
                    return invoker::template type<
                        t_id,
                        cpltime_override
                    >(
                        forward<
                            t_args
                        >(
                            a_args
                        )...
                    );
                }
                else
                {
                    return invoker::template type<
                        t_id,
                        typename cpltime_selected::impl
                    >(
                        forward<
                            t_args
                        >(
                            a_args
                        )...
                    );
                }
            }
            else
            {
                if constexpr (meta::complex<runtime_override>)
                {
                    return invoker::template type<
                        t_id,
                        runtime_override
                    >(
                        forward<
                            t_args
                        >(
                            a_args
                        )...
                    );
                }
                else
                {
                    return invoker::template hook<
                        t_id
                    >(
                        runtime_selected::impl,
                        forward<
                            t_args
                        >(
                            a_args
                        )...
                    );
                }
            }
        }
    };
}
