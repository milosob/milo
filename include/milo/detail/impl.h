

#pragma once


#include <milo/arch.h>
#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/option.h>
#include <milo/traits.h>
#include <milo/utility.h>


namespace milo::detail
{
    using impl_domain_strict = arch::active::ise::strict;
    
    using impl_domain_native = arch::active::ise::native;
    
    using impl_domain_runtime = arch::active::ise::runtime;
    
    template<
        typename t_domain,
        typename t_chooser,
        typename t_impl,
        typename... t_impls
    >
    static
    constexpr auto
    impl_choose_hook(
    ) noexcept(true)
    {
        if constexpr (concepts::differ<t_impl, void>)
        {
            if constexpr (
                (!
                    requires{
                        typename t_impl::requirements::arch;
                    })
                )
            {
                return t_chooser::template hook<t_impl>();
            }
            else
            {
                if (t_domain::template impl<t_impl>())
                {
                    return t_chooser::template hook<t_impl>();
                }
            }
        }
        
        if constexpr (sizeof...(t_impls) > 0)
        {
            return impl_choose_hook<
                t_domain,
                t_chooser,
                t_impls...
            >();
        }
        else
        {
            utility::unreachable();
        }
    }
    
    template<
        typename t_domain,
        typename t_chooser,
        typename... t_impls
    >
    struct impl_choose_type
        :
            traits::disjunction<
                traits::boolean<
                    typename t_chooser::template type<t_impls>,
                    []() constexpr noexcept(true)
                    {
                        if constexpr (concepts::differ<t_impls, void>)
                        {
                            if constexpr (
                                (!
                                    requires{
                                        typename t_impls::requirements::arch;
                                    })
                                )
                            {
                                return true;
                            }
                            else
                            {
                                return t_domain::template impl<t_impls>();
                            }
                        }
                        
                        return false;
                    }()
                >...
            >
    {
    };
    
    template<
        typename t_domain,
        typename t_chooser,
        typename... t_impls
    >
    concept impl_choose_assert =
    requires
    {
        requires sizeof...(t_impls) > 0;
        requires concepts::same<
            t_domain,
            impl_domain_strict,
            impl_domain_native,
            impl_domain_runtime
        >;
        requires impl_choose_type<
            impl_domain_strict,
            t_chooser,
            t_impls...
        >::value;
    };
    
    template<
        typename... t_impls
    >
    using impl_cpltime = traits::args<t_impls...>;
    
    template<
        typename t_domain,
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
    using impl_runtime = traits::args<t_impls...>;
    
    template<
        typename t_domain,
        typename t_chooser,
        typename... t_impls
    >
    requires
    requires
    {
        requires impl_choose_assert<
            impl_domain_strict,
            t_chooser,
            t_impls...
        >;
    }
    struct impl_runtime_reader
    {
        using impl = typename impl_choose_type<
            t_domain,
            t_chooser,
            t_impls...
        >::type;
    };
    
    template<
        typename t_chooser,
        typename... t_impls
    >
    requires
    requires
    {
        requires impl_choose_assert<
            impl_domain_strict,
            t_chooser,
            t_impls...
        >;
    }
    struct impl_runtime_reader<
        impl_domain_runtime,
        t_chooser,
        t_impls...
    >
    {
        inline
        static
        const auto impl = impl_choose_hook<
            impl_domain_runtime,
            t_chooser,
            t_impls...
        >();
    };
    
    template<
        typename t_domain,
        typename t_chooser,
        typename t_invoker,
        typename t_cpltime,
        typename t_runtime,
        typename... t_options
    >
    struct impl_proxy
    {
    private:
        
        using domain = option_impl_domain_suite::query_default_t<t_domain, t_options...>;
        
        using chooser = option_impl_chooser_suite::query_default_t<t_chooser, t_options...>;
        
        using invoker = option_impl_invoker_suite::query_default_t<t_invoker, t_options...>;
        
        using cpltime_override = option_impl_cpltime_suite::query_default_t<void, t_options...>;
        
        using runtime_override = option_impl_runtime_suite::query_default_t<void, t_options...>;
        
        using cpltime_selected = typename traits::args_reader<t_cpltime, impl_cpltime_reader, domain, chooser>::type;
        
        using runtime_selected = typename traits::args_reader<t_runtime, impl_runtime_reader, domain, chooser>::type;
    
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
            if MILO_CONSTEVAL
            {
                if constexpr (concepts::differ<cpltime_override, void>)
                {
                    return invoker::template type<
                        t_id,
                        cpltime_override
                    >(
                        utility::forward<
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
                        utility::forward<
                            t_args
                        >(
                            a_args
                        )...
                    );
                }
            }
            else
            {
                if constexpr (concepts::differ<runtime_override, void>)
                {
                    return invoker::template type<
                        t_id,
                        runtime_override
                    >(
                        utility::forward<
                            t_args
                        >(
                            a_args
                        )...
                    );
                }
                else
                {
                    if constexpr (concepts::same<
                        domain,
                        impl_domain_strict,
                        impl_domain_native
                    >)
                    {
                        return invoker::template type<
                            t_id,
                            typename runtime_selected::impl
                        >(
                            utility::forward<
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
                            utility::forward<
                                t_args
                            >(
                                a_args
                            )...
                        );
                    }
                }
            }
        }
    };
}
