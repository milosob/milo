
#pragma once


#include <algorithm>
#include <charconv>
#include <functional>
#include <numeric>
#include <map>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

#include <milo/app/dep.h>
#include <milo/app/type.h>


namespace milo::app
{
    namespace detail
    {
        class app_args
        {
        public:
            
            using arg_type = std::string_view;
            
            using args_type = std::vector<arg_type>;
            
            using commands_type = std::stack<std::string>;
        
        private:
            
            args_type m_args;
            
            args_type::iterator m_args_beg;
            
            args_type::iterator m_args_end;
            
            args_type::iterator m_args_cur;
            
            commands_type m_commands;
        
        public:
            
            app_args() noexcept(true) = default;
            
            app_args(
                int a_argc,
                char** a_argv
            ) noexcept(false):
                m_args(
                    a_argv + 1,
                    a_argv + a_argc
                ),
                m_args_beg(
                    m_args.begin()
                ),
                m_args_end(
                    m_args.end()
                ),
                m_args_cur(
                    m_args.begin()
                )
            {
                m_commands.emplace(
                    MILO_NAME " application"
                );
            }
        
        public:
            
            auto
            dump(
            ) const -> std::string
            {
                auto str = std::accumulate(
                    m_args.begin(),
                    m_args.end(),
                    std::string{},
                    [](
                        std::string a,
                        std::string_view b
                    )
                    {
                        return a.append(b).append(" ");
                    }
                );
                
                str.pop_back();
                
                return str;
            }
            
            auto
            args(
            ) const -> const args_type&
            {
                return m_args;
            }
            
            auto
            command(
            ) -> arg_type
            {
                auto it = std::find_if(
                    m_args_cur,
                    m_args_end,
                    [](auto& a_item)
                    {
                        return !a_item.starts_with('-');
                    }
                );
                
                if (it == m_args_end)
                {
                    throw std::logic_error(
                        std::string(
                            "Error. Expected a command token. "
                            "Commands do not start with - or --. "
                            "Please check argument list and try again. "
                        )
                    );
                }
                
                m_args_cur = it + 1;
                m_commands.push(
                    std::string(*it) + " command"
                );
                
                return *it;
            }
            
            auto
            peek(
            ) -> arg_type
            {
                auto it = std::find_if(
                    m_args_cur,
                    m_args_end,
                    [](auto& a_item)
                    {
                        return !a_item.starts_with('-');
                    }
                );
                
                if (it == m_args_end)
                {
                    throw std::logic_error(
                        std::string(
                            "Error. Expected a command token. "
                            "Commands do not start with - or --. "
                            "Please check argument list and try again. "
                        )
                    );
                }
                
                return *it;
            }
            
            auto
            option(
                arg_type a_name
            ) const -> bool
            {
                auto it = m_args_cur;
                
                for (; it != m_args_end; it += 1)
                {
                    if (it->starts_with("--"))
                    {
                        if (it->substr(2) == a_name)
                        {
                            return true;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
                
                return false;
            }
            
            template<
                typename t_value
            >
            auto
            parameter(
                arg_type a_name
            ) const -> t_value
            requires
            requires
            {
                requires meta::integral<t_value> ||
                         meta::same<t_value, std::string, std::string_view>;
            }
            {
                /*
                 * TODO
                 *  Typeid type to name map for defined conversions for more robust error feedback.
                 */
                
                using value_type = t_value;
                
                auto it = m_args_cur;
                
                for (; it != m_args_end; it += 1)
                {
                    if (it->starts_with("--"))
                    {
                        if (it->substr(2).starts_with(a_name))
                        {
                            break;
                        }
                    }
                    else
                    {
                        throw std::logic_error(
                            std::string(
                                "Error. Missing --" +
                                std::string(a_name) +
                                " parameter of " +
                                m_commands.top() +
                                "."
                            )
                        );
                    }
                }
                
                if (it == m_args_end)
                {
                    throw std::logic_error(
                        std::string("Error. Missing --") +
                        std::string(a_name) +
                        " parameter of " +
                        m_commands.top() +
                        "."
                    
                    );
                }
                else
                {
                    auto pos = it->find_first_of('=');
                    
                    if (pos == arg_type::npos)
                    {
                        throw std::logic_error(
                            std::string("Error. Missing value for --") +
                            std::string(a_name) +
                            " parameter of " +
                            m_commands.top() +
                            ". " +
                            std::string("To specify missing value try --") + std::string(a_name) + "=<value>."
                        );
                    }
                    
                    if (pos + 1 >= it->size())
                    {
                        return value_type();
                    }
                    
                    auto value_beg = it->data() + pos + 1;
                    auto value_end = it->data() + it->size();
                    
                    if constexpr (meta::integral<value_type>)
                    {
                        auto value = value_type();
                        auto result = std::from_chars(
                            value_beg,
                            value_end,
                            value
                        );
                        
                        if (result.ec != decltype(result.ec)())
                        {
                            throw std::logic_error(
                                std::string("Error. Value conversion failure for --") +
                                std::string(a_name) +
                                " parameter of " +
                                m_commands.top() +
                                ". " +
                                std::string("Conversion from ") + std::string(
                                    value_beg,
                                    value_end
                                ) + " to type of type " + typeid(value_type).name() + " failed."
                            );
                        }
                        
                        return value;
                    }
                    
                    if constexpr (meta::same<value_type, std::string, std::string_view>)
                    {
                        return value_type(
                            value_beg,
                            value_end
                        );
                    }
                    
                    /*
                     * Missing conversion.
                     */
                    
                    throw std::logic_error(
                        std::string("Error. Value conversion failure for --") +
                        std::string(a_name) +
                        " parameter of " +
                        m_commands.top() +
                        ". " +
                        std::string("Conversion from ") + std::string(
                            value_beg,
                            value_end
                        ) + " to type of type " + typeid(value_type).name() + " is undefined."
                    );
                }
            }
            
            template<
                typename t_value
            >
            auto
            parameter(
                arg_type a_name,
                const t_value& a_default
            ) const -> t_value
            {
                try
                {
                    return parameter<
                        t_value
                    >(
                        a_name
                    );
                }
                catch (...)
                {
                    return a_default;
                }
            }
        };
        
        using app_context = type::object;
        
        struct app_options
        {
            bool verbose = false;
            
            bool advanced = false;
            
            bool suppress = false;
        };
    }
    
    template<
        typename t_signature
    >
    using app_callmap = std::map<
        std::string_view,
        std::function<t_signature>
    >;
    
    namespace app
    {
        detail::app_args args;
        
        detail::app_context context;
        
        detail::app_options options;
    }
}
