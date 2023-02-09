## Roadmap

This is a loose list of features that are planned to be added to the library.

Always assume that constexpr version will be available.
No eta but updates will be incremental.

----

- `aes-128-gcm`, `aes-192-gcm`, `aes-256-gcm`
    - Software and hardware assisted for x86-64.
- BigInteger implementation.
    - Mostly implemented but due to high generalization and flexibility:
        - Support for different base types of limbs.
        - Customizable radix width.
        - Cross template parametrized type arithmetic.
            - For example `bigint<640> = bigint<1024> * bigint<384>` is a valid expression with upper part truncation.
        - Radix casts.
    - It is difficult to design a unit test methodology with the desired level of coverage.
- `chacha-20` backend with `avx-2`.
- `poly-1305` block parallelization.
- Support for reference chacha-20 with 64 bit nonce.
- Support for chacha variants with 8 and 12 rounds.
