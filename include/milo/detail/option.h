

#pragma once


#include <milo/traits.h>


namespace milo::detail
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
        : traits::boolean<
            t_option,
            (
                requires
                {
                    typename t_option::properties::option;
                    typename t_option::properties::option_oftype;
                    typename t_option::type;
                }
            )
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
        : traits::boolean<
            t_option,
            (
                requires
                {
                    typename t_option::properties::option;
                    typename t_option::properties::option_ofvalue;
                    t_option::value;
                }
            )
        >
    {
    };
    
    template<
        typename t_option
    >
    constexpr auto option_ofvalue_is_v = option_ofvalue_is<t_option>::value;
    
    template<
        template<typename> typename t_solver,
        typename t_default,
        typename... t_options
    >
    struct option_query
    {
    private:
        
        using expression_type = traits::disjunction<
            t_solver<t_options>...,
            t_solver<t_default>
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
        using solver = traits::boolean<
            t_option,
            (
                requires
                {
                    requires option_ofvalue_is_v<t_option>;
                    requires traits::is_same_v<
                        traits::del_constant_t<
                            decltype(t_option::value)
                        >,
                        t_type
                    >;
                    requires traits::is_same_v<
                        t_option,
                        option<t_option::value>
                    >;
                }
            )
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
        using solver = traits::boolean<
            t_option,
            (
                requires
                {
                    requires option_oftype_is_v<t_option>;
                    requires traits::is_same_v<
                        t_option,
                        option<typename t_option::type>
                    >;
                }
            )
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
    };
    
    struct option_digest_size_id
    {
    };
    
    using option_digest_size_suite = option_ofvalue_suite<
        option_digest_size_id,
        size_t
    >;
    
    struct option_impl_scope_id
    {
    };
    
    using option_impl_scope_suite = option_oftype_suite<
        option_impl_scope_id
    >;
    
    struct option_impl_constexpr_id
    {
    };
    
    using option_impl_constexpr_suite = option_oftype_suite<
        option_impl_constexpr_id
    >;
    
    struct option_impl_runtime_id
    {
    };
    
    using option_impl_runtime_suite = option_oftype_suite<
        option_impl_runtime_id
    >;
}
