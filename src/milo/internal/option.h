

#pragma once


#include <milo/meta.h>

#include <milo/internal/base.h>


namespace milo::internal
{
    template<
        typename t_type
    >
    struct option_oftype
    {
        struct properties
        {
            using option [[maybe_unused]] = int;
            
            using option_oftype [[maybe_unused]] = int;
        };
        
        using type [[maybe_unused]] = t_type;
    };
    
    template<
        typename t_type,
        t_type t_value
    >
    struct option_ofvalue
    {
        struct properties
        {
            using option [[maybe_unused]] = int;
            
            using option_ofvalue [[maybe_unused]] = int;
        };
        
        static
        constexpr t_type value = t_value;
    };
    
    template<
        typename t_option
    >
    struct option_oftype_is
        : meta::asbool<
            (
                requires
                {
                    typename t_option::properties::option;
                    typename t_option::properties::option_oftype;
                    typename t_option::type;
                }
            ),
            t_option
        >
    {
    };
    
    template<
        typename t_option
    >
    constexpr auto option_oftype_is_v = option_oftype_is<t_option>::value;
    
    template<
        typename t_option
    >
    struct option_ofvalue_is
        : meta::asbool<
            (
                requires
                {
                    typename t_option::properties::option;
                    typename t_option::properties::option_ofvalue;
                    t_option::value;
                }
            ),
            t_option
        >
    {
    };
    
    template<
        typename t_option
    >
    constexpr auto option_ofvalue_is_v = option_ofvalue_is<t_option>::value;
    
    template<
        template<typename> typename t_solver,
        typename... t_options
    >
    struct option_query
    {
    private:
        
        using expression_type = meta::disjunction<
            t_solver<t_options>...
        >;
        
        static_assert(expression_type::value);
    
    public:
        
        using type = typename expression_type::type;
    };
    
    template<
        typename t_properties,
        typename t_type
    >
    struct option_ofvalue_suite
    {
    private:
        
        template<
            t_type t_value
        >
        struct option_impl
            : option_ofvalue<t_type, t_value>
        {
            struct properties
                : t_properties,
                  option_ofvalue<t_type, t_value>::properties
            {
            };
        };
    
    public:
        
        template<
            t_type t_value
        >
        using option = option_impl<t_value>;
        
        template<
            typename t_option
        >
        using solver = meta::asbool<
            (
                requires
                {
                    requires option_ofvalue_is_v<t_option>;
                    requires meta::same<
                        meta::del_constant<
                            decltype(t_option::value)
                        >,
                        t_type
                    >;
                    requires meta::same<
                        t_option,
                        option<t_option::value>
                    >;
                }
            ),
            t_option
        >;
        
        template<
            typename... t_options
        >
        using query = typename option_query<
            solver,
            t_options...
        >::type;
        
        template<
            typename... t_options
        >
        static
        constexpr auto query_v = query<
            t_options...
        >::value;
        
        template<
            t_type t_default,
            typename... t_options
        >
        using query_default = query<
            t_options...,
            option<t_default>
        >;
        
        template<
            t_type t_default,
            typename... t_options
        >
        static
        constexpr auto query_default_v = query_v<
            t_options...,
            option<t_default>
        >;
    };
    
    template<
        typename t_properties
    >
    struct option_oftype_suite
    {
    private:
        
        template<
            typename t_type
        >
        struct option_impl
            : option_oftype<t_type>
        {
            struct properties
                : t_properties,
                  option_oftype<t_type>::properties
            {
            };
        };
    
    public:
        
        template<
            typename t_type
        >
        using option = option_impl<t_type>;
        
        template<
            typename t_option
        >
        using solver = meta::asbool<
            (
                requires
                {
                    requires option_oftype_is_v<t_option>;
                    requires meta::same<
                        t_option,
                        option<typename t_option::type>
                    >;
                }
            ),
            t_option
        >;
        
        template<
            typename... t_options
        >
        using query = typename option_query<
            solver,
            t_options...
        >::type;
        
        template<
            typename... t_options
        >
        using query_t = typename query<
            t_options...
        >::type;
        
        template<
            typename t_default,
            typename... t_options
        >
        using query_default = query<
            t_options...,
            option<t_default>
        >;
        
        template<
            typename t_default,
            typename... t_options
        >
        using query_default_t = query_t<
            t_options...,
            option<t_default>
        >;
    };
    
    /*
     * Start of digest options.
     */
    
    struct option_digest_size_id
    {
    };
    
    using option_digest_size_suite = option_ofvalue_suite<
        option_digest_size_id,
        size_t
    >;
    
    /*
     * Start of impl options.
     */
    
    struct option_impl_chooser_id
    {
    };
    
    using option_impl_chooser_suite = option_oftype_suite<
        option_impl_chooser_id
    >;
    
    struct option_impl_invoker_id
    {
    };
    
    using option_impl_invoker_suite = option_oftype_suite<
        option_impl_invoker_id
    >;
    
    struct option_impl_cpltime_id
    {
    };
    
    using option_impl_cpltime_suite = option_oftype_suite<
        option_impl_cpltime_id
    >;
    
    struct option_impl_runtime_id
    {
    };
    
    using option_impl_runtime_suite = option_oftype_suite<
        option_impl_runtime_id
    >;

    struct option_impl_enforce_id
    {
    };
    
    using option_impl_enforce_suite = option_oftype_suite<
        option_impl_enforce_id
    >;
}
