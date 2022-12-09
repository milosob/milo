## Authenticated encryption with associated data

This file describes authenticated encryption with associated data.

### Index

- [Index](#index)
- [Algorithms](#algorithms)
  - [Chacha-20-Poly-1305](#algorithms-chacha-20-poly-1305)
- [Encryption and decryption](#encryption-and-decryption)
  - [Using pointers](#encryption-and-decryption-using-pointers)
  - [Using containers](#encryption-and-decryption-using-containers)
  - [Using oneliners](#encryption-and-decryption-using-oneliners)
- [More](#more)

### Algorithms

Supported:

- `chacha-20-poly-1305`

Headers and types:

- `<milo/primitive/aead/apie.h>`
  - `milo::primitive::aead::apie<impl_type>`
- `<milo/primitive/aead/chacha.h>`
  - `milo::primitive::aead::chacha_20_poly_1305<>`

All implementations of the algorithms share the same interface.

Differences that may occur:

- Instance declaration.
- Instance initialization.
- Key size.
- Iv size.
- Resulting digest.

Constants:

```c++
using alg_type = ...;

/*
 * Size of the digest.
 */
constexpr auto digest_size = alg_type::digest_size;

/*
 * Size of the key.
 */
constexpr auto key_size = alg_type::key_size;

/*
 * Size of the iv.
 */
constexpr auto iv_size = alg_type::iv_size;
```

#### Algorithms chacha-20-poly-1305

```c++
#include <milo/primitive/aead/chacha.h>

milo::primitive::aead::chacha_20_poly_1305<> chacha_20_poly_1305;
```

- `digest_size = 16 bytes`
- `key_size = 32 bytes`
- `iv_size = 12 bytes`

### Encryption and decryption

#### Encryption and decryption using pointers

```c++
#include <numeric>
#include <iostream>
#include <string>
#include <string_view>

#include <milo/primitive/codec/apie.h>
#include <milo/primitive/codec/base.h>
#include <milo/primitive/aead/chacha.h>

int main()
{
    using namespace std::literals;
    
    using aead_type = milo::primitive::aead::chacha_20_poly_1305<>;
    
    auto key          = std::array<char, aead_type::key_size>{};
    auto iv           = std::array<char, aead_type::iv_size>{};
    auto message      = "Example message to encrypt."sv;
    auto aad          = "Example message to authenticate."sv;
    auto ciphertext   = std::string();
    auto plaintext    = std::string();
    auto digest_enc   = std::array<char, aead_type::digest_size>{};
    auto digest_dec   = std::array<char, aead_type::digest_size>{};
    
    std::iota(key.begin(), key.end(), 0x01);
    std::iota(iv.begin(), iv.end(), 0x01);
    
    aead_type aead;
    
    /*
     * Initialize with key and iv.
     */
    aead.initialize(key.data(), key.size(), iv.data(), iv.size());
    
    /*
     * Can be invoked multiple times to authenticate longer messages.
     */
    aead.aad(aad.data(), aad.size());
    aead.aad(aad.data(), aad.size());
    
    /*
     * Encrypt the message.
     * Encryption works like update.
     * Subsequent calls will still encrypt the message.
     * Decryption works the same way.
     */
    ciphertext.resize(aead.encrypt_size(message.data(), message.size()));
    ciphertext.resize(aead.encrypt(message.data(), message.size(), ciphertext.data()));
    
    /*
     * Must be finalized before getting digest.
     */
    aead.finalize();
    
    /*
     * Get the digest.
     * Can be invoked multiple times.
     */
    aead.digest(digest_enc.data());
    
    /*
     * Reinitialize with key and iv.
     */
    
    aead.initialize(key.data(), key.size(), iv.data(), iv.size());
    aead.aad(aad.data(), aad.size());
    aead.aad(aad.data(), aad.size());
    
    plaintext.resize(aead.decrypt_size(ciphertext.data(), ciphertext.size()));
    plaintext.resize(aead.decrypt(ciphertext.data(), ciphertext.size(), plaintext.data()));
    
    aead.finalize();
    aead.digest(digest_dec.data());
    
    std::cout << "Reference message: "    << message << "\n";
    std::cout << "Decrypted message: " << plaintext << "\n";
    
    /*
     * Digest authentication during decryption is only supported via apie wrapper.
     * Apie verification ensures constant time comparisons.
     * Here, the default comparison is used without such security guarantees.
     */
    
    std::cout << "Encryption mac: " << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(digest_enc) << "\n";
    std::cout << "Decryption mac: " << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(digest_dec) << "\n";
    std::cout << "Authentication: " << std::boolalpha << (digest_enc == digest_dec) << "\n";
    
    return 0;
}
```

#### Encryption and decryption using containers

```c++
#include <numeric>
#include <iostream>
#include <string>
#include <string_view>

#include <milo/primitive/aead/apie.h>
#include <milo/primitive/aead/chacha.h>

int main()
{
    using namespace std::literals;
    
    using aead_type = milo::primitive::aead::apie<milo::primitive::aead::chacha_20_poly_1305<>>;
    
    auto key     = std::array<char, aead_type::key_size>{};
    auto iv      = std::array<char, aead_type::iv_size>{};
    auto message = "Example message to encrypt."sv;
    auto aad     = "Example message to authenticate."sv;
    
    std::iota(key.begin(), key.end(), 0x01);
    std::iota(iv.begin(), iv.end(), 0x01);
    
    aead_type aead(key, iv);
    
    /*
     * Authenticate additional data.
     */
    aead.aad(aad);
    aead.aad(aad);
    
    /*
     * Encrypt the message.
     */
    auto ciphertext = aead.encrypt(message);
    
    /*
     * Get the digest.
     */
    auto digest = aead.digest();
    
    /*
     * Must be re-initialized before reuse.
     */
    aead.initialize(key, iv);
    
    /*
     * Authenticate additional data.
     */
    aead.aad(aad);
    aead.aad(aad);
    
    /*
     * Decrypt the ciphertext to string.
     */
    auto plaintext = aead.decrypt<std::string>(ciphertext);
    
    /*
     * Verify the digest.
     * Get error by reference.
     */
    
    milo::error error;
    
    aead.verify(digest, error);
    
    if (error)
    {
        std::cout << "Unexpected decryption error.\n";
    }
    
    /*
     * Verify the digest.
     * Get error by exception.
     */
    
    try
    {
        aead.verify(digest);
    }
    catch (const milo::error& error)
    {
        std::cout << "Unexpected decryption error.\n";
    }
    
    std::cout << "Reference message: " << message << "\n";
    std::cout << "Decrypted message: " << plaintext << "\n";
    
    /*
     * Force an error by modifying the digest.
     */
    try
    {
        digest[0] += 1;
        aead.verify(digest);
    }
    catch (const milo::error& error)
    {
        std::cout << "Expected decryption error.\n";
    }
    
    return 0;
}
```

#### Encryption and decryption using oneliners

```c++
#include <numeric>
#include <iostream>
#include <string>
#include <string_view>

#include <milo/primitive/aead/apie.h>
#include <milo/primitive/aead/chacha.h>

int main()
{
    using namespace std::literals;
    
    using aead_type = milo::primitive::aead::chacha_20_poly_1305<>;
    
    auto key     = std::array<char, aead_type::key_size>{};
    auto iv      = std::array<char, aead_type::iv_size>{};
    auto aad     = "Example message to authenticate."sv;
    auto message = "Example message to encrypt."sv;
    
    std::iota(key.begin(), key.end(), 0x01);
    std::iota(iv.begin(), iv.end(), 0x01);
    
    do
    {
        /*
         * Encrypt and decrypt.
         * Get error by reference.
         */
        
        milo::error error;
        
        auto [ciphertext, mac] = milo::primitive::aead::encrypt<aead_type>(key, iv, aad, message);
        auto plaintext         = milo::primitive::aead::decrypt<aead_type>(key, iv, aad, ciphertext, mac, error);
        
        if (error)
        {
            std::cout << "Unexpected decryption error.\n";
            break;
        }
    
        std::cout << "Reference message: " << message << "\n";
        std::cout << "Decrypted message: " << std::string_view((const char*)(plaintext.data()), plaintext.size()) << "\n";
    }
    while (false);
    
    try
    {
        /*
         * Encrypt and decrypt.
         * Get error by exception.
         * To achieve this, omit the optional error reference parameter.
         * This time decrypt directly to string.
         * The plaintext type is passed as the second template parameter of the decryption method.
         * Encryption to variable of custom type is also possible, works the same way.
         */
        
        auto [ciphertext, mac] = milo::primitive::aead::encrypt<aead_type, std::string>(key, iv, aad, message);
        auto plaintext         = milo::primitive::aead::decrypt<aead_type, std::string>(key, iv, aad, ciphertext, mac);
    
        std::cout << "Reference message: " << message << "\n";
        std::cout << "Decrypted message: " << plaintext << "\n";
    }
    catch (const milo::error& error)
    {
        std::cout << "Unexpected decryption error.\n";
    }
    
    try
    {
        /*
         * Encrypt and decrypt.
         * Get error by exception.
         * This time enforce decryption error.
         */
        
        auto [ciphertext, mac] = milo::primitive::aead::encrypt<aead_type>(key, iv, aad, message);
        mac[0]                += 1;
        auto plaintext         = milo::primitive::aead::decrypt<aead_type, std::string>(key, iv, aad, ciphertext, mac);
        
        std::cout << "Reference message: " << message << "\n";
        std::cout << "Decrypted message: " << plaintext << "\n";
    }
    catch (const milo::error& error)
    {
        std::cout << "Expected decryption error.\n";
    }
    
    return 0;
}
```

### More

Most likely there are more overloads as needed.
All available options are listed in the header files in the examples above.
All methods to be used come with documentation in code.
