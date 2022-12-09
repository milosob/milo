## Key derivation functions

This file describes key derivation functions.

### Index

- [Index](#index)
- [Algorithms](#algorithms)
  - [Hkdf](#algorithms-hkdf)
  - [Pbkdf-2](#algorithms-pbkdf-2)
- [Key derivation](#key-derivation)
  - [Using pointers](#key-derivation-using-pointers)
  - [Using containers](#key-derivation-using-containers)
  - [Using oneliners](#key-derivation-using-oneliners)
- [More](#more)

### Algorithms

Supported:

- `hkdf`
- `pbkdf-2`

Headers and types:

- `<milo/primitive/kdf/apie.h>`
    - `milo::primitive::kdf::apie<hkdf_type>`
- `<milo/primitive/kdf/hkdf.h>`
    - `milo::primitive::kdf::hkdf<hmac_type>`
- `<milo/primitive/mac/pbkdf.h>`
    - `milo::primitive::kdf::pbkdf_2<prf_type>`

All implementations of the algorithms share the same interface.

Differences that may occur:

- Instance declaration.
- Instance initialization.
- Derived key.
- Derived key limits.

Key derivation algorithms support streaming derivation
where with each call to `derive(...)` next part of the key is returned.

No allocation and copy of by pointer provided data takes place.
With this in mind, callee is responsible for keeping the pointers alive while using kdf instances.
However, not all pointers have to be maintained, for certain algorithms
during initialization some sub-products are calculated.

Pointers that must remain valid:

- `hkdf`
    - info
- `pbkdf_2`
    - ikm
    - salt

#### Algorithms Hkdf

```c++
#include <milo/primitive/hash/sha.h>
#include <milo/primitive/mac/hmac.h>
#include <milo/primitive/kdf/hkdf.h>

/*
 * Hkdf class template requires a hmac type as the template parameter.
 */
milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<>>> hkdf;

hkdf.initialize(
    ikm_ptr,
    ikm_size,
    salt_ptr,
    salt_size,
    info_ptr,
    info_size
);
```

#### Algorithms Pbkdf-2

```c++
#include <milo/primitive/hash/sha.h>
#include <milo/primitive/mac/hmac.h>
#include <milo/primitive/kdf/pbkdf.h>

/*
 * Pbkdf-2 class template requires a pseudo random function type as the template parameter.
 * Hmac is a compatible pseudo-random function.
 */
milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<>>> pbkdf_2;

pbkdf_2.initialize(
    ikm_ptr,
    ikm_size,
    salt_ptr,
    salt_size,
    iterations
);
```

### Key derivation

#### Key derivation using pointers

```c++
#include <iostream>
#include <string_view>

#include <milo/primitive/codec/apie.h>
#include <milo/primitive/codec/base.h>
#include <milo/primitive/hash/sha.h>
#include <milo/primitive/mac/hmac.h>
#include <milo/primitive/kdf/hkdf.h>
#include <milo/primitive/kdf/pbkdf.h>


template<typename t_kdf>
void derive_example(t_kdf& a_kdf)
{
    char key_a[32];
    char key_b[64];
    
    /*
     * Derive.
     */
    auto key_a_size = a_kdf.derive(key_a, 32);
    auto key_b_size = a_kdf.derive(key_b, 32);
    
    std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(std::span(key_a, key_a_size)) << "\n";
    std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(std::span(key_b, key_b_size)) << "\n";
}

int main()
{
    using namespace std::literals;
    
    std::cout << "Derive with hkdf:\n";
    {
        using hkdf_type = milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<>>>;
        
        auto ikm  = "ikm"sv;
        auto salt = "salt"sv;
        auto info = "info"sv;
        
        hkdf_type hkdf;
        hkdf.initialize(ikm.data(), ikm.size(), salt.data(), salt.size(), info.data(), info.size());
        derive_example(hkdf);
    }
    
    std::cout << "Derive with pbkdf-2:\n";
    {
        using pbkdf_2_type = milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<>>>;
        
        auto ikm        = "ikm"sv;
        auto salt       = "salt"sv;
        auto iterations = 1000;
        
        pbkdf_2_type pbkdf_2;
        pbkdf_2.initialize(ikm.data(), ikm.size(), salt.data(), salt.size(), iterations);
        derive_example(pbkdf_2);
    }
    
    return 0;
}
```

#### Key derivation using containers

```c++
#include <iostream>
#include <string_view>

#include <milo/primitive/codec/apie.h>
#include <milo/primitive/codec/base.h>
#include <milo/primitive/hash/sha.h>
#include <milo/primitive/mac/hmac.h>
#include <milo/primitive/kdf/apie.h>
#include <milo/primitive/kdf/hkdf.h>
#include <milo/primitive/kdf/pbkdf.h>


template<typename t_kdf>
void derive_example(t_kdf& a_kdf)
{
    std::array<char, 32> key_array{};
    std::vector<char> key_vector;
    
    /*
     * Derive by reference, up to capacity or requested size.
     */
    auto key_array_size = a_kdf.derive(key_array, 32);
    
    /*
     * Derive by reference up to requested size.
     */
    auto key_vector_size = a_kdf.derive(key_vector, 128);
    
    /*
     * Get by value, but dynamically allocated.
     */
    auto key_returned_by_value_1 = a_kdf.derive(32);
    
    /*
     * Get by value, but fixed.
     */
    auto key_returned_by_value_2 = a_kdf.template derive<std::array<char, 32>>();
    
    std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(key_array) << "\n";
    std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(key_vector) << "\n";
    std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(key_returned_by_value_1) << "\n";
    std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(key_returned_by_value_2) << "\n";
}

int main()
{
    using namespace std::literals;
    
    std::cout << "Derive with hkdf:\n";
    {
        using hkdf_type = milo::primitive::kdf::apie<milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<>>>>;
        
        auto ikm  = "ikm"sv;
        auto salt = "salt"sv;
        auto info = "info"sv;
        
        hkdf_type hkdf(ikm, salt, info);
        derive_example(hkdf);
        
        /*
         * Derive example uses streaming derivation.
         */
        std::cout << "Key:\n";
        std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(milo::primitive::kdf::derive<hkdf_type::impl_type>(ikm, salt, info, 224)) << "\n\n" ;
    }
    
    std::cout << "Derive with pbkdf-2:\n";
    {
        using pbkdf_2_type = milo::primitive::kdf::apie<milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<>>>>;
        
        auto ikm        = "ikm"sv;
        auto salt       = "salt"sv;
        auto iterations = 1000;
        
        pbkdf_2_type pbkdf_2(ikm, salt, iterations);
        derive_example(pbkdf_2);
        
        /*
         * Derive example uses streaming derivation.
         */
        std::cout << "Key:\n";
        std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(milo::primitive::kdf::derive<pbkdf_2_type::impl_type>(ikm, salt, iterations, 224)) << "\n" ;
        
        return 0;
    }
}
```

#### Key derivation using oneliners

```c++
#include <iostream>
#include <string_view>

#include <milo/primitive/codec/apie.h>
#include <milo/primitive/codec/base.h>
#include <milo/primitive/hash/sha.h>
#include <milo/primitive/mac/hmac.h>
#include <milo/primitive/kdf/apie.h>
#include <milo/primitive/kdf/hkdf.h>
#include <milo/primitive/kdf/pbkdf.h>

int main()
{
    using namespace std::literals;
    
    using hkdf_type = milo::primitive::kdf::hkdf<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<>>>;
    using pbkdf_2_type = milo::primitive::kdf::pbkdf_2<milo::primitive::mac::hmac<milo::primitive::hash::sha_2_256<>>>;
    
    std::cout << "Derive with hkdf:\n";
    std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(milo::primitive::kdf::derive<hkdf_type>("ikm"sv, "salt"sv, "info"sv, 64)) << "\n" ;
    std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(milo::primitive::kdf::derive<hkdf_type>("ikm"sv, "salt"sv, "info"sv, 32)) << "\n" ;
    std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(milo::primitive::kdf::derive<hkdf_type>("ikm"sv, "hello"sv, "world"sv, 32)) << "\n" ;
    
    std::cout << "Derive with pbkdf-2:\n";
    std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(milo::primitive::kdf::derive<pbkdf_2_type>("ikm"sv, "salt"sv, 100, 64)) << "\n" ;
    std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(milo::primitive::kdf::derive<pbkdf_2_type>("ikm"sv, "salt"sv, 1000, 32)) << "\n" ;
    std::cout << milo::primitive::codec::encode<milo::primitive::codec::base_16<>, std::string>(milo::primitive::kdf::derive<pbkdf_2_type>("ikm"sv, "hello"sv, 5000, 32)) << "\n" ;
    
    return 0;
}
```

### More

Most likely there are more overloads as needed.
All available options are listed in the header files in the examples above.
All methods to be used come with documentation in code.
