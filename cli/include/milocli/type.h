
#pragma once


#include <any>
#include <functional>
#include <sstream>
#include <string>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <utility>

#include <milo/meta.h>


namespace milocli::type
{
    class array;
    
    class object;
    
    class var
    {
    private:
        
        struct var_impl
        {
            std::any m_val;
            
            std::function<size_t(const std::any&)> m_hasher;
            
            var_impl() = default;
            
            template<
                typename t_val
            >
            var_impl(
                const t_val& a_val
            ) :
                m_val(a_val),
                m_hasher(
                    [](const std::any& a_any) -> size_t
                    {
                        if constexpr (
                            requires{
                                {
                                std::hash<t_val>()
                                };
                            })
                        {
                            /*
                             * No infinite recursion because var cannot store var.
                             */
                            return std::hash<t_val>()(*std::any_cast<t_val>(&a_any)) ^
                                   (typeid(t_val).hash_code() << 1);
                        }
                        else
                        {
                            return 0;
                        }
                    }
                )
            {
            }
        };
    
    private:
        
        var_impl m_var;
    
    public:
        
        var();
        
        var(var&&) = default;
        
        var(const var&) = default;
        
        ~var() = default;
    
    public:
        
        template<
            size_t t_size
        >
        var(
            const char (& a_val)[t_size]
        ) :
            m_var(std::string_view(a_val))
        {
        }
        
        template<
            typename t_val
        >
        var(
            const t_val& a_val
        )
        requires milo::meta::same<
            t_val,
            std::string,
            std::string_view
        > :
            m_var(a_val)
        {
        }
        
        template<
            std::signed_integral t_val
        >
        var(
            t_val a_val
        ) :
            m_var(static_cast<long>(a_val))
        {
        }
        
        template<
            std::unsigned_integral t_val
        >
        var(
            t_val a_val
        ) :
            m_var(static_cast<long unsigned>(a_val))
        {
        }
        
        template<
            std::floating_point t_val
        >
        var(
            t_val a_val
        ) :
            m_var(static_cast<double>(a_val))
        {
        }
        
        var(
            bool a_val
        ) :
            m_var(a_val)
        {
        }
        
        /*
         * Indirect constructors.
         */
        
        var(
            std::initializer_list<std::tuple<var, var>> a_val
        );
        
        var(
            std::initializer_list<std::tuple<var>> a_val
        );
        
        template<
            typename t_val
        >
        var(
            const std::vector<t_val>& a_val
        );
        
        var(
            const object& a_val
        );
    
    public:
        
        auto
        operator =(
            var&& a_object
        ) -> var& = default;
        
        auto
        operator =(
            const var& a_object
        ) -> var& = default;
    
    public:
        
        auto
        type(
        ) const -> const std::type_info&
        {
            return m_var.m_val.type();
        }
        
        auto
        hash(
        ) const -> size_t
        {
            return m_var.m_hasher(
                m_var.m_val
            );
        }
        
        template<
            typename t_type
        >
        auto
        is(
        ) const -> bool
        {
            return type() == typeid(t_type);
        }
        
        template<
            typename t_type
        >
        auto
        as(
        ) const -> const t_type&
        {
            return *std::any_cast<t_type>(&m_var.m_val);
        }
        
        template<
            typename t_type
        >
        auto
        as(
        ) -> t_type&
        {
            return *std::any_cast<t_type>(&m_var.m_val);
        }
    
    private:
        
        template<
            typename t_var
        >
        auto
        compare(
            const t_var& a_var
        ) const -> bool
        {
            if constexpr (
                std::same_as<t_var, std::string> ||
                std::same_as<t_var, std::string_view>
                )
            {
                if (is<std::string_view>())
                {
                    return as<std::string_view>() == a_var;
                }
                
                if (is<std::string>())
                {
                    return as<std::string>() == a_var;
                }
            }
            
            if constexpr (
                std::integral<t_var> ||
                std::floating_point<t_var>
                )
            {
                if (is<long>())
                {
                    return as<long>() == a_var;
                }
                
                if (is<long unsigned>())
                {
                    return as<long unsigned>() == a_var;
                }
                
                if (is<double>())
                {
                    return as<double>() == a_var;
                }
            }
            
            if constexpr (
                std::same_as<t_var, bool>
                )
            {
                if (is<bool>())
                {
                    return as<bool>() == a_var;
                }
            }
            
            return false;
        }
        
        auto
        compare(
            const var& a_var
        ) const -> bool
        {
            if (a_var.is<std::string_view>())
            {
                return compare(a_var.as<std::string_view>());
            }
            
            if (a_var.is<std::string>())
            {
                return compare(a_var.as<std::string>());
            }
            
            if (a_var.is<long>())
            {
                return compare(a_var.as<long>());
            }
            
            if (a_var.is<long unsigned>())
            {
                return compare(a_var.as<long unsigned>());
            }
            
            if (a_var.is<double>())
            {
                return compare(a_var.as<double>());
            }
            
            if (a_var.is<bool>())
            {
                return compare(a_var.as<bool>());
            }
            
            return false;
        }
    
    public:
        
        operator size_t(
        ) const
        {
            if (is<long>())
            {
                return size_t(as<long>());
            }
            
            if (is<long unsigned>())
            {
                return size_t(as<long unsigned>());
            }
            
            throw std::logic_error("Attempt to convert to size_t non-size_t convertible type.");
        }
        
        operator bool(
        ) const
        {
            if (is<bool>())
            {
                return as<bool>();
            }
            
            if (is<long>())
            {
                return as<long>() != 0;
            }
            
            if (is<long unsigned>())
            {
                return as<long unsigned>() != 0;
            }
            
            throw std::logic_error("Attempt to convert to bool non-bool convertible type.");
        }
        
        operator std::string(
        ) const
        {
            if (is<std::string>())
            {
                return as<std::string>();
            }
            
            if (is<std::string_view>())
            {
                return std::string(as<std::string_view>());
            }
            
            if (is<long>())
            {
                return std::to_string(as<long>());
            }
            
            if (is<long unsigned>())
            {
                return std::to_string(as<long unsigned>());
            }
            
            if (is<double>())
            {
                return std::to_string(as<double>());
            }
            
            if (is<bool>())
            {
                return as<bool>() ? std::string("true") : std::string("false");
            }
            
            throw std::logic_error("Attempt to convert to string non-string convertible type.");
        }
        
        template<
            typename t_type
        >
        operator t_type&(
        )
        {
            return as<t_type>();
        }
        
        template<
            typename t_type
        >
        operator const t_type&(
        ) const
        {
            try
            {
                return as<t_type>();
            }
            catch (const std::bad_any_cast& error)
            {
                throw std::logic_error("Attempt to convert non-compatible type.");
            }
        }
    
    public:
        
        template<
            typename... t_val
        >
        auto
        operator [](
            t_val&& ... a_val
        ) -> var&;
        
        template<
            typename... t_val
        >
        auto
        operator [](
            t_val&& ... a_val
        ) const -> const var&;
    
    public:
        
        friend auto
        operator ==(
            const var& a_a,
            const var& a_b
        ) -> bool
        {
            return a_a.compare(a_b);
        }
        
        template<
            size_t t_size
        >
        friend auto
        operator ==(
            const var& a_a,
            const char (& a_b)[t_size]
        ) -> bool
        {
            return a_a.compare(std::string_view(a_b));
        }
        
        friend auto
        operator ==(
            const var& a_a,
            const std::string& a_b
        ) -> bool
        {
            return a_a.compare(a_b);
        }
        
        friend auto
        operator ==(
            const var& a_a,
            const char& a_b
        ) -> bool
        {
            return a_a.compare(a_b);
        }
        
        friend auto
        operator ==(
            const var& a_a,
            const char unsigned& a_b
        ) -> bool
        {
            return a_a.compare(a_b);
        }
        
        friend auto
        operator ==(
            const var& a_a,
            const int& a_b
        ) -> bool
        {
            return a_a.compare(a_b);
        }
        
        friend auto
        operator ==(
            const var& a_a,
            const int unsigned& a_b
        ) -> bool
        {
            return a_a.compare(a_b);
        }
        
        friend auto
        operator ==(
            const var& a_a,
            const long& a_b
        ) -> bool
        {
            return a_a.compare(a_b);
        }
        
        friend auto
        operator ==(
            const var& a_a,
            const long unsigned& a_b
        ) -> bool
        {
            return a_a.compare(a_b);
        }
        
        friend auto
        operator ==(
            const var& a_a,
            const float& a_b
        ) -> bool
        {
            return a_a.compare(a_b);
        }
        
        friend auto
        operator ==(
            const var& a_a,
            const double& a_b
        ) -> bool
        {
            return a_a.compare(a_b);
        }
        
        friend auto
        operator ==(
            const var& a_a,
            const bool& a_b
        ) -> bool
        {
            return a_a.compare(a_b);
        }
    
    public:
        
        friend auto
        operator <<(
            std::ostream& a_ostream,
            const var& a_val
        ) -> std::ostream&
        {
            return a_ostream << std::string(a_val);
        }
    };
}

template<>
struct std::hash<milocli::type::var>
{
    auto
    operator ()(
        milocli::type::var const& a_var
    ) const noexcept -> size_t
    {
        return a_var.hash();
    }
};

namespace milocli::type
{
    class array
    {
    public:
        
        using storage_type = std::vector<var>;
        
        using initializer_type = std::initializer_list<storage_type::value_type>;
    
    private:
        
        storage_type m_storage;
    
    public:
        
        array() = default;
        
        array(array&& a_array) = default;
        
        array(const array& a_array) = default;
        
        ~array() = default;
    
    public:
        
        array(
            std::initializer_list<std::tuple<var>> a_init
        )
        {
            for (const auto& [val]: a_init)
            {
                m_storage.push_back(val);
            }
        }
        
        template<
            typename t_val
        >
        array(
            const std::vector<t_val>& a_init
        )
        {
            for (const auto& val: a_init)
            {
                m_storage.push_back(var(val));
            }
        }
    
    public:
        
        auto
        operator [](
            const var& a_key
        ) -> var&
        {
            return m_storage[size_t(a_key)];
        }
        
        auto
        operator [](
            const var& a_key
        ) const -> const var&
        {
            return m_storage[size_t(a_key)];
        }
    
    public:
        
        auto
        storage(
        ) -> storage_type&
        {
            return m_storage;
        }
        
        auto
        storage(
        ) const -> const storage_type&
        {
            return m_storage;
        }
    };
    
    class object
    {
    public:
        
        using storage_type = std::unordered_map<var, var>;
    
    private:
        
        storage_type m_storage;
    
    public:
        
        object() = default;
        
        object(object&& a_object) = default;
        
        object(const object& a_object) = default;
        
        ~object() = default;
    
    public:
        
        object(
            std::initializer_list<std::tuple<var, var>> a_init
        )
        {
            for (const auto& [key, val]: a_init)
            {
                m_storage[key] = val;
            }
        }
    
    public:
        
        auto
        operator =(
            object&& a_object
        ) -> object& = default;
        
        auto
        operator =(
            const object& a_object
        ) -> object& = default;
    
    public:
        
        auto
        operator [](
            const var& a_key
        ) -> var&
        {
            return m_storage[a_key];
        }
        
        auto
        operator [](
            const var& a_key
        ) const -> const var&
        {
            return m_storage.at(a_key);
        }
    
    public:
        
        auto
        storage(
        ) -> storage_type&
        {
            return m_storage;
        }
        
        auto
        storage(
        ) const -> const storage_type&
        {
            return m_storage;
        }
    };
    
    var::var(
    ) :
        m_var(object())
    {
    }
    
    var::var(
        std::initializer_list<std::tuple<var, var>> a_val
    ) :
        m_var(object(a_val))
    {
    }
    
    var::var(
        std::initializer_list<std::tuple<var>> a_val
    ) :
        m_var(array(a_val))
    {
    }
    
    template<
        typename t_val
    >
    var::var(
        const std::vector<t_val>& a_val
    ):
        m_var(array(a_val))
    {
    }
    
    var::var(
        const object& a_val
    ) :
        m_var(a_val)
    {
    }
    
    template<
        typename... t_val
    >
    auto
    var::operator [](
        t_val&& ... a_val
    ) -> var&
    {
        auto v = var(std::forward<t_val>(a_val)...);
        
        if (is<object>())
        {
            return as<object>()[v];
        }
        
        if (is<array>())
        {
            return as<array>()[v];
        }
        
        throw std::logic_error("Attempt to call [] operator on non index-able type.");
    }
    
    template<
        typename... t_val
    >
    auto
    var::operator [](
        t_val&& ... a_val
    ) const -> const var&
    {
        auto v = var(std::forward<t_val>(a_val)...);
        
        if (is<object>())
        {
            return as<object>()[v];
        }
        
        if (is<array>())
        {
            return as<array>()[v];
        }
        
        throw std::logic_error("Attempt to call [] operator on non index-able type.");
    }
    
    auto
    to_json(
        std::stringstream& a_stream,
        const array& a_array
    ) -> void;
    
    auto
    to_json(
        std::stringstream& a_stream,
        const object& a_object
    ) -> void;
    
    auto
    to_json(
        std::stringstream& a_stream,
        const object& a_object
    ) -> void
    {
        auto& obj = a_object.storage();
        auto obj_size = obj.size();
        
        a_stream << '{';
        
        for (const auto& [key, item]: obj)
        {
            obj_size -= 1;
            
            if (key.is<object>() || key.is<array>())
            {
                throw std::logic_error("Attempt to use json-serializable type as key.");
            }
            
            a_stream << '\"' << key << "\":";
            
            if (item.is<object>())
            {
                to_json(
                    a_stream,
                    item.as<object>()
                );
            }
            else if (item.is<array>())
            {
                to_json(
                    a_stream,
                    item.as<array>()
                );
            }
            else if (item.is<std::string_view>() || item.is<std::string>())
            {
                a_stream << '\"' << item << "\"";
            }
            else
            {
                a_stream << item;
            }
            
            if (obj_size > 0)
            {
                a_stream << ',';
            }
        }
        
        a_stream << '}';
    }
    
    auto
    to_json(
        std::stringstream& a_stream,
        const array& a_array
    ) -> void
    {
        auto& arr = a_array.storage();
        auto arr_size = arr.size();
        
        a_stream << '[';
        
        for (const auto& item: arr)
        {
            arr_size -= 1;
            
            if (item.is<object>())
            {
                to_json(
                    a_stream,
                    item.as<object>()
                );
            }
            else if (item.is<array>())
            {
                to_json(
                    a_stream,
                    item.as<array>()
                );
            }
            else if (item.is<std::string_view>() || item.is<std::string>())
            {
                a_stream << '\"' << item << "\"";
            }
            else
            {
                a_stream << item;
            }
            
            if (arr_size > 0)
            {
                a_stream << ',';
            }
        }
        
        a_stream << ']';
    }
    
    auto
    to_json(
        const array& a_array
    ) -> std::string
    {
        std::stringstream json;
        
        to_json(
            json,
            a_array
        );
        
        return json.str();
    }
    
    auto
    to_json(
        const object& a_object
    ) -> std::string
    {
        std::stringstream json;
        
        to_json(
            json,
            a_object
        );
        
        return json.str();
    }
    
    auto
    to_json(
        const var& a_var
    ) -> std::string
    {
        if (a_var.is<object>())
        {
            return to_json(
                a_var.as<object>()
            );
        }
        else if (a_var.is<array>())
        {
            return to_json(
                a_var.as<array>()
            );
        }
        else
        {
            throw std::logic_error("Attempt to json serialize non serializable type.");
        }
    }
}
