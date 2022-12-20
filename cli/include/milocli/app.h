
#pragma once


#include <algorithm>
#include <charconv>
#include <functional>
#include <numeric>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include <milocli/dep.h>
#include <milocli/error.h>
#include <milocli/type.h>


namespace milocli
{
    class app_args
    {
    public:
        
        using arg_type = std::string_view;
        
        using args_type = std::vector<arg_type>;
    
    private:
        
        args_type m_args;
        
        args_type::iterator m_args_beg;
        
        args_type::iterator m_args_end;
        
        args_type::iterator m_args_cur;
    
    public:
        
        constexpr app_args(
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
        }
    
    public:
        
        constexpr auto
        raw(
        ) -> std::string
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
        
        constexpr auto
        args(
        ) const -> const args_type&
        {
            return m_args;
        }
        
        constexpr auto
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
                throw error();
            }
            
            m_args_cur = it + 1;
            
            return *it;
        }
        
        constexpr auto
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
        constexpr auto
        parameter(
            arg_type a_name
        ) const -> t_value
        requires
        requires
        {
            requires milo::meta::integral<t_value> ||
                     milo::meta::same<t_value, std::string, std::string_view>;
        }
        {
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
                    throw error();
                }
            }
            
            if (it == m_args_end)
            {
                throw error();
            }
            else
            {
                auto pos = it->find_first_of('=');
                
                if (pos == arg_type::npos)
                {
                    throw error();
                }
                
                if (pos + 1 >= it->size())
                {
                    return value_type();
                }
                
                auto value_beg = it->data() + pos + 1;
                auto value_end = it->data() + it->size();
                
                if constexpr (milo::meta::integral<value_type>)
                {
                    auto value = value_type();
                    auto result = std::from_chars(
                        value_beg,
                        value_end,
                        value
                    );
                    
                    if (result.ec != decltype(result.ec)())
                    {
                        throw error();
                    }
                    
                    return value;
                }
                
                if constexpr (milo::meta::same<value_type, std::string, std::string_view>)
                {
                    return value_type(
                        value_beg,
                        value_end
                    );
                }
                
                /*
                 * Missing conversion.
                 */
                
                return value_type();
            }
        }
    };
    
    using app_context = type::object;
    
    template<
        typename t_signature
    >
    using app_callmap = std::map<
        std::string_view,
        std::function<t_signature>
    >;
}
