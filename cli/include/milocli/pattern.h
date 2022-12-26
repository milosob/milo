

#pragma once


#include <milocli/dep.h>


namespace milocli
{
    template<
        typename t_pattern,
        typename t_message
    >
    auto
    pattern_match_star(
        const t_pattern& a_pattern,
        const t_message& a_message
    ) noexcept(true) -> bool
    requires
    requires
    {
        requires milo::meta::same<t_pattern, std::string, std::string_view>;
        requires milo::meta::same<t_message, std::string, std::string_view>;
    }
    {
        if (a_pattern == "*")
        {
            return true;
        }
        
        if (a_pattern.empty() && a_message.empty())
        {
            return true;
        }
        
        if (a_pattern.empty() && !a_message.empty())
        {
            return false;
        }
        
        size_t message_pos = 0;
        size_t pattern_pos = 0;
        
        while (true)
        {
            auto pattern_pos_beg = a_pattern.find_first_not_of(
                '*',
                pattern_pos
            );
            
            if (pattern_pos_beg == t_pattern::npos)
            {
                if (pattern_pos < a_pattern.size())
                {
                    return a_pattern.ends_with('*');
                }
                else
                {
                    return a_message.size() == message_pos;
                }
            }
            
            auto pattern_pos_end = a_pattern.find_first_of(
                '*',
                pattern_pos_beg
            );
            
            if (pattern_pos_end == t_pattern::npos)
            {
                pattern_pos_end = a_pattern.size();
            }
            
            auto pattern = a_pattern.data() + pattern_pos_beg;
            auto pattern_size = pattern_pos_end - pattern_pos_beg;
            
            pattern_pos = pattern_pos_beg;
            message_pos = a_message.find(
                pattern,
                message_pos,
                pattern_size
            );
            
            if (message_pos == t_message::npos)
            {
                return false;
            }
            
            pattern_pos += pattern_size;
            message_pos += pattern_size;
        }
    }
}
