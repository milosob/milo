

#pragma once

namespace milo
{
    /**
     * Error.
     */
    class error
    {
    public:
    
        /**
         * Error code.
         */
        enum code
        {
            none = 0,
            size,
            aead_verify,
            codec_base_64_encode,
            codec_base_64_decode,
            codec_hex_encode,
            codec_hex_decode
        };
    
    private:
        
        code m_code = code::none;
    
    public:
        
        constexpr error() noexcept(true) = default;
        
        constexpr error(error&& a_object) noexcept(true) = default;
        
        constexpr error(const error& a_object) noexcept(true) = default;
        
        constexpr ~error() noexcept(true) = default;
    
    public:
        
        /**
         * Constructs object.
         *
         * @param a_code
         * Code.
         */
        constexpr error(
            code a_code
        ) noexcept(true)
            :
            m_code(a_code)
        {
        }
    
    public:
        
        constexpr auto
        operator =(error&& object) noexcept(true) -> error& = default;
        
        constexpr auto
        operator =(const error& object) noexcept(true) -> error& = default;
    
    public:
        
        /**
         * This function returns error as bool.
         *
         * @return
         * True if not-none, otherwise False.
         */
        explicit
        constexpr
        operator bool(
        ) const noexcept(true)
        {
            return m_code != code::none;
        }
        
        /**
         * This function returns error as code.
         *
         * @return
         * Code.
         */
        explicit
        constexpr
        operator code(
        ) const noexcept(true)
        {
            return m_code;
        }
        
        /**
         * This function compares error to code.
         *
         * @param a_code
         * Code.
         * @return
         * True if equal, otherwise False.
         */
        constexpr auto
        operator ==(
            code a_code
        ) const noexcept(true) -> bool
        {
            return m_code == a_code;
        }
    
    public:
        
        /**
         * This function clears error.
         */
        constexpr auto
        clear() noexcept(true) -> void
        {
            m_code = code::none;
        }
    };
}
