project(
        milo-test-primitive
        VERSION ${PROJECT_VERSION}
        LANGUAGES CXX
)

file(
        GLOB
        HASH_TESTS
        hash_*.cpp
)

file(
        GLOB
        MAC_TESTS
        mac_*.cpp
)

file(
        GLOB
        KDF_TESTS
        kdf_*.cpp
)

file(
        GLOB
        CIPHER_TESTS
        cipher_*.cpp
)

file(
        GLOB
        AEAD_TESTS
        aead_*.cpp
)

file(
        GLOB
        CODEC_TESTS
        codec_*.cpp
)

add_tests(
        ${HASH_TESTS}
        ${MAC_TESTS}
        ${KDF_TESTS}
        ${CIPHER_TESTS}
        ${AEAD_TESTS}
        ${CODEC_TESTS}
)
