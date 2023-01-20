

#pragma once


#include <milo/app/app.h>
#include <milo/app/dep.h>
#include <milo/app/type.h>


namespace milo::app::parameter
{
    template<
        typename t_to
    >
    struct convert
    {
        using type = t_to;
        
        template<
            typename t_from
        >
        auto
        operator ()(
            const t_from& a_from
        )
        {
            return type(a_from);
        }
    };
    
    struct convert_no
    {
        using type = void;
        
        template<
            typename t_from
        >
        auto
        operator ()(
            const t_from& a_from
        )
        {
            return a_from;
        }
    };
    
    template<
        size_t t_size
    >
    struct name
    {
        char value[t_size]{};
        
        constexpr name(
            const char(& a_chars)[t_size]
        )
        {
            for (size_t i = 0; i < t_size; ++i)
            {
                value[i] = a_chars[i];
            }
        }
    };
    
    template<
        name t_name,
        typename t_read,
        typename t_convert = convert_no
    >
    struct parameter
    {
        static
        constexpr auto name = t_name;
        
        using read_type = t_read;
        
        using convert_type = t_convert;
        
        using value_type = typename meta::conditional<
            meta::same<convert_type, convert_no>,
            read_type,
            typename convert_type::type
        >::type;
    };
    
    template<
        typename... t_parameters
    >
    using group = meta::args<t_parameters...>;
    
    template<
        typename t_invoke,
        typename t_config,
        size_t t_index,
        typename t_parameter,
        typename... t_parameters
    >
    auto
    parse(
        t_invoke& a_invoke,
        t_config& a_config
    )
    {
        using read_type = typename t_parameter::read_type;
        using convert_type = typename t_parameter::convert_type;
        
        auto name = std::string_view(t_parameter::name.value);
        auto value = app::args.parameter<read_type>(name);
        
        if constexpr (meta::same<convert_type, void>)
        {
            std::get<t_index>(a_invoke) = value;
        }
        else
        {
            std::get<t_index>(a_invoke) = convert_type{}(value);
        }
        
        a_config[name] = value;
        
        if constexpr (sizeof...(t_parameters))
        {
            parse<
                t_invoke,
                t_config,
                t_index + 1,
                t_parameters...
            >(
                a_invoke,
                a_config
            );
        }
    }
    
    template<
        typename t_config,
        typename... t_parameters
    >
    auto
    parse(
        [[maybe_unused]]
        group<t_parameters...> a_ignore,
        t_config& a_config
    )
    {
        using invoke_type = std::tuple<typename t_parameters::value_type...>;
        using config_type = t_config;
        
        invoke_type invoke;
        
        if constexpr (sizeof...(t_parameters) > 0)
        {
            parse<
                invoke_type,
                config_type,
                0,
                t_parameters...
            >(
                invoke,
                a_config
            );
        }
        
        return invoke;
    }
}
