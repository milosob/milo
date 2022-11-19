## Ciphers

This file describes ciphers.

### Index

- [Index](#index)
- [Algorithms](#algorithms)
    - [Chacha-20](#algorithms-chacha-20)
- [Encryption and decryption](#encryption-and-decryption)
    - [Using pointers](#encryption-and-decryption-using-pointers)
    - [Using containers](#encryption-and-decryption-using-containers)
    - [Using oneliners](#encryption-and-decryption-using-oneliners)
- [More](#more)

### Algorithms

Supported mac algorithms:

- `chacha_20`

Headers and types:

- `<milo/cipher/apie.h>`
    - `milo::cipher::apie<impl_type>`
- `<milo/cipher/chacha_20.h>`
  - `milo::cipher::chacha_20`

All implementations of the algorithms share the same interface.

Differences that may occur:

- Instance declaration.
- Instance initialization.
- Key size.
- Iv size.

Constants:

```c++
using alg_type = ...;

/*
 * Size of the key.
 */
constexpr auto key_size = alg_type::key_size;

/*
 * Size of the iv.
 */
constexpr auto iv_size = alg_type::iv_size;
```

#### Algorithms Chacha-20

```c++
#include <milo/cipher/chacha_20.h>

milo::cipher::chacha_20 chacha_20;
```

- `key_size = 32 bytes`
- `iv_size = 16 bytes`
    - The first 4 bytes are interpreted as a 32-bit little endian counter and the remainder are iv.

### Encryption and decryption

#### Encryption and decryption using pointers

```c++
#include <numeric>
#include <iostream>
#include <string>
#include <string_view>

#include <milo/cipher/chacha_20.h>

int main()
{
    using namespace std::literals;
    
    using cipher_type = milo::cipher::chacha_20;
    
    auto key        = std::array<char, cipher_type::key_size>{};
    auto iv         = std::array<char, cipher_type::iv_size>{};
    auto message    = "Example message to encrypt."sv;
    auto ciphertext = std::string();
    auto plaintext  = std::string();
    
    std::iota(key.begin(), key.end(), 0x01);
    std::iota(iv.begin(), iv.end(), 0x01);
    
    cipher_type cipher;
    
    /*
     * Initialize with key and iv.
     */
    cipher.initialize(key.data(), key.size(), iv.data(), iv.size());
    
    /*
     * Encrypt the message.
     */
    ciphertext.resize(cipher.encrypt_size(message.data(), message.size()));
    ciphertext.resize(cipher.encrypt(message.data(), message.size(), ciphertext.data()));
    
    /*
     * Initialize with key and iv.
     */
    cipher.initialize(key.data(), key.size(), iv.data(), iv.size());
    
    /*
     * Decrypt the ciphertext.
     */
    plaintext.resize(cipher.decrypt_size(ciphertext.data(), ciphertext.size()));
    plaintext.resize(cipher.decrypt(ciphertext.data(), ciphertext.size(), plaintext.data()));
    
    std::cout << "Reference message: " << message << "\n";
    std::cout << "Decrypted message: " << plaintext << "\n";
    
    return 0;
}
```

#### Encryption and decryption using containers

```c++
#include <numeric>
#include <iostream>
#include <string>
#include <string_view>

#include <milo/cipher/apie.h>
#include <milo/cipher/chacha_20.h>

int main()
{
    using namespace std::literals;
    
    using cipher_type = milo::cipher::apie<milo::cipher::chacha_20>;
    
    auto key        = std::array<char, cipher_type::key_size>{};
    auto iv         = std::array<char, cipher_type::iv_size>{};
    auto message    = "Example message to encrypt."sv;
    
    std::iota(key.begin(), key.end(), 0x01);
    std::iota(iv.begin(), iv.end(), 0x01);
    
    {
        std::cout << "\n";
        std::cout << "Example with fixed memory:\n";
        
        std::array<char, 1024> ciphertext;
        std::array<char, 1024> plaintext;
        
        cipher_type cipher(key, iv);
        
        auto ciphertext_size = cipher.encrypt(message, ciphertext);
        
        cipher.initialize(key, iv);
        
        auto plaintext_size = cipher.decrypt(std::span(ciphertext.data(), ciphertext_size), plaintext);
    
        std::cout << "Reference message: " << message << "\n";
        std::cout << "Decrypted message: " << std::string_view(plaintext.data(), plaintext_size) << "\n";
    }
    
    {
        std::cout << "\n";
        std::cout << "Example with dynamic memory returned by reference:\n";
        
        std::string ciphertext;
        std::string plaintext;
        
        cipher_type cipher(key, iv);
        
        cipher.encrypt(message, ciphertext);
        
        cipher.initialize(key, iv);
        
        cipher.decrypt(ciphertext, plaintext);
    
        std::cout << "Reference message: " << message << "\n";
        std::cout << "Decrypted message: " << std::string_view(plaintext.data(), plaintext.size()) << "\n";
    }
    
    {
        std::cout << "\n";
        std::cout << "Example with dynamic memory returned by value:\n";
        
        cipher_type cipher(key, iv);
        
        auto ciphertext = cipher.encrypt<std::string>(message);
        
        cipher.initialize(key, iv);
        
        auto plaintext = cipher.decrypt<std::string>(ciphertext);
    
        std::cout << "Reference message: " << message << "\n";
        std::cout << "Decrypted message: " << plaintext << "\n";
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

#include <milo/cipher/apie.h>
#include <milo/cipher/chacha_20.h>

int main()
{
    using namespace std::literals;
    
    using cipher_type = milo::cipher::chacha_20;
    
    auto key = std::array<char, cipher_type::key_size>{};
    auto iv = std::array<char, cipher_type::iv_size>{};
    auto message = "Example message to encrypt."sv;
    
    std::iota(key.begin(), key.end(), 0x01);
    std::iota(iv.begin(), iv.end(), 0x01);
    
    {
        std::cout << "Example with fixed memory:\n";
        
        std::array<char, 1024> ciphertext;
        std::array<char, 1024> plaintext;
        
        auto ciphertext_size = milo::cipher::encrypt<cipher_type>(key, iv, message, ciphertext);
        auto plaintext_size = milo::cipher::decrypt<cipher_type>(key, iv, std::span(ciphertext.data(), ciphertext_size), plaintext);
        
        std::cout << "Reference message: " << message << "\n";
        std::cout << "Decrypted message: " << std::string_view(plaintext.data(), plaintext_size) << "\n";
    }
    
    {
        std::cout << "\n";
        std::cout << "Example with dynamic memory returned by reference:\n";
        
        std::string ciphertext;
        std::string plaintext;
        
        milo::cipher::encrypt<cipher_type>(key, iv, message, ciphertext);
        milo::cipher::decrypt<cipher_type>(key, iv, ciphertext, plaintext);
        
        std::cout << "Reference message: " << message << "\n";
        std::cout << "Decrypted message: " << std::string_view(plaintext.data(), plaintext.size()) << "\n";
    }
    
    {
        std::cout << "\n";
        std::cout << "Example with dynamic memory returned by value:\n";
        
        auto ciphertext = milo::cipher::encrypt<cipher_type, std::string>(key, iv, message);
        auto plaintext = milo::cipher::decrypt<cipher_type, std::string>(key, iv, ciphertext);
        
        std::cout << "Reference message: " << message << "\n";
        std::cout << "Decrypted message: " << plaintext << "\n";
    }
    
    return 0;
}
```

### More

Most likely there are more overloads as needed.
All available options are listed in the header files in the examples above.
All methods to be used come with documentation in code.
