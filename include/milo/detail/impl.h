

#pragma once


#include <milo/arch.h>
#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/traits.h>
#include <milo/utility.h>


namespace milo::detail
{
    using impl_scope_strict = arch::active::ise::strict;
    
    using impl_scope_native = arch::active::ise::native;
    
    using impl_scope_runtime = arch::active::ise::runtime;
    
    template<
        typename t_scope,
        typename t_selector,
        typename t_impl,
        typename... t_impls
    >
    static
    constexpr auto
    impl_select_hook_basic(
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
                return t_selector::template hook<t_impl>();
            }
            else
            {
                if (t_scope::template impl<t_impl>())
                {
                    return t_selector::template hook<t_impl>();
                }
            }
        }
        
        if constexpr (sizeof...(t_impls) > 0)
        {
            return impl_select_hook_basic<
                t_scope,
                t_selector,
                t_impls...
            >();
        }
        else
        {
            utility::unreachable();
        }
    }
    
    template<
        typename t_scope,
        typename t_selector,
        typename... t_impls
    >
    struct impl_select_type_basic
        :
            traits::disjunction<
                traits::boolean<
                    typename t_selector::template type<t_impls>,
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
                                return t_scope::template impl<t_impls>();
                            }
                        }
                        
                        return false;
                    }()
                >...
            >
    {
    };
    
    template<
        typename t_scope,
        typename t_selector,
        typename... t_impls
    >
    concept impl_select_assert =
    requires
    {
        requires sizeof...(t_impls) > 0;
        requires concepts::same<
            t_scope,
            impl_scope_strict,
            impl_scope_native,
            impl_scope_runtime
        >;
        requires impl_select_type_basic<
            impl_scope_strict,
            t_selector,
            t_impls...
        >::value;
    };
    
    template<
        typename t_scope,
        typename t_selector,
        typename... t_impls
    >
    requires
    requires
    {
        requires impl_select_assert<
            t_scope,
            t_selector,
            t_impls...
        >;
    }
    struct impl_select_type
        : impl_select_type_basic<
            t_scope,
            t_selector,
            t_impls...
        >
    {
    };
    
    template<
        typename t_scope,
        typename t_selector,
        typename... t_impls
    >
    static
    constexpr auto
    impl_select_hook(
    ) noexcept(true)
    requires
    requires
    {
        requires impl_select_assert<
            t_scope,
            t_selector,
            t_impls...
        >;
    }
    {
        return impl_select_hook_basic<
            t_scope,
            t_selector,
            t_impls...
        >();
    }
    
    template<
        typename t_scope,
        typename t_selector,
        typename... t_impls
    >
    struct impl_select;
    
    template<
        typename t_selector,
        typename... t_impls
    >
    struct impl_select<
        impl_scope_strict,
        t_selector,
        t_impls...
    >
    {
        using impl = typename impl_select_type_basic<
            impl_scope_strict,
            t_selector,
            t_impls...
        >::type;
    };
    
    template<
        typename t_selector,
        typename... t_impls
    >
    struct impl_select<
        impl_scope_native,
        t_selector,
        t_impls...
    >
    {
        using impl = typename impl_select_type_basic<
            impl_scope_native,
            t_selector,
            t_impls...
        >::type;
    };
    
    template<
        typename t_selector,
        typename... t_impls
    >
    struct impl_select<
        impl_scope_runtime,
        t_selector,
        t_impls...
    >
    {
        inline
        static
        const auto impl = impl_select_hook_basic<
            impl_scope_runtime,
            t_selector,
            t_impls...
        >();
    };
    
    template<
        typename t_scope,
        typename t_selector,
        typename t_invoker,
        typename t_constexpr,
        typename t_enforced,
        typename... t_impls
    >
    requires
    requires
    {
        requires impl_select_assert<
            t_scope,
            t_selector,
            t_impls...,
            t_constexpr
        >;
    }
    struct impl_proxy
    {
        using im_selected = impl_select<t_scope, t_selector, t_impls..., t_constexpr>;
    
        using im_constexpr = t_constexpr;
    
        using im_enforced = t_enforced; // TODO Forcing implementation is not implemented.
    
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
                return t_invoker::template type<
                    t_id,
                    im_constexpr
                >(
                    utility::forward<t_args>(
                        a_args
                    )...
                );
            }
            else
            {
                if constexpr (concepts::same<t_scope, impl_scope_strict, impl_scope_native>)
                {
                    return t_invoker::template type<
                        t_id,
                        typename im_selected::type
                    >(
                        utility::forward<t_args>(
                            a_args
                        )...
                    );
                }
                else
                {
                    return t_invoker::template hook<
                        t_id
                    >(
                        im_selected::impl,
                        utility::forward<t_args>(
                            a_args
                        )...
                    );
                }
            }
        }
    };
}
