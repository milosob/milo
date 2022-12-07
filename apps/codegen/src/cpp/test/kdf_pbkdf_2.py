import src.cpp.lang


def gen(
    a_config: dict
) -> str:
    test_impl = a_config['impl']
    test_include = a_config['include']

    test_vectors: list
    test_vectors = []

    for test in a_config['vectors']:
        test_vectors += test['data']

    for test_vector in test_vectors:
        assert 'id' in test_vector
        assert 'ikm' in test_vector
        assert 'salt' in test_vector
        assert 'iterations' in test_vector
        assert 'key' in test_vector

    code = src.cpp.lang.gen_body(
        [
            '',
            '',
            src.cpp.lang.gen_include(
                [
                    '<iostream>'
                ]
            ),
            '',
            src.cpp.lang.gen_include(
                [
                    '<milo/inner.h>',
                    '<milo/literal.h>'
                ]
            ),
            '',
            src.cpp.lang.gen_include(
                test_include
            ),
            src.cpp.lang.gen_include(
                [
                    '<milo/crypto/kdf/test.h>'
                ]
            ),
            '',
            '',
            'using namespace milo::literal;',
            '',
            src.cpp.lang.gen_struct(
                'test_vector',
                [
                    'const size_t id;',
                    '',
                    'const milo::container::chars_const_view_dynamic ikm;',
                    '',
                    'const milo::container::chars_const_view_dynamic salt;',
                    '',
                    'const uint32_t iterations;',
                    '',
                    'const milo::container::chars_const_view_dynamic key;'
                ]
            ),
            '',
            f'constexpr milo::container::array<test_vector, {len(test_vectors)}> test_vectors',
            [
                src.cpp.lang.gen_block(
                    [
                        (
                            'test_vector',
                            [
                                src.cpp.lang.gen_block(
                                    [
                                        f'.id = {t_vector["id"]},',
                                        '.ikm =',
                                        src.cpp.lang.gen_bytes_to_hex_str(
                                            bytes.fromhex(
                                                t_vector['ikm']
                                            ),
                                            end='_cv,'
                                        ),
                                        '.salt =',
                                        src.cpp.lang.gen_bytes_to_hex_str(
                                            bytes.fromhex(
                                                t_vector['salt']
                                            ),
                                            end='_cv,'
                                        ),
                                        f'.iterations = {t_vector["iterations"]},',
                                        '.key =',
                                        src.cpp.lang.gen_bytes_to_hex_str(
                                            bytes.fromhex(
                                                t_vector['key']
                                            ),
                                            end='_cv'
                                        )
                                    ],
                                    end=','
                                )
                            ]
                        )
                        for t_vector in test_vectors
                    ],
                    end=';'
                )
            ],
            '',
            src.cpp.lang.gen_function_constexpr(
                'test',
                [],
                'size_t',
                [
                    'size_t test_vectors_size = test_vectors.size();',
                    'size_t test_vectors_failed = 0;',
                    '',
                    src.cpp.lang.gen_loop_for(
                        ['size_t i = 0', 'i < test_vectors_size', 'i += 1'],
                        [
                            f'auto result = milo::crypto::kdf::test<{test_impl}>::derive(',
                            [
                                'test_vectors[i].ikm,',
                                'test_vectors[i].salt,',
                                'test_vectors[i].iterations,',
                                'test_vectors[i].key'
                            ],
                            ');',
                            '',
                            src.cpp.lang.gen_if(
                                'result',
                                src.cpp.lang.gen_continue()
                            ),
                            '',
                            'if MILO_CONSTEVAL',
                            src.cpp.lang.gen_block(
                                [
                                    'return test_vectors[i].id;'
                                ]
                            ),
                            'else',
                            src.cpp.lang.gen_block(
                                [
                                    src.cpp.lang.gen_if(
                                        'test_vectors_failed == 0',
                                        'std::cerr << "Tests that failed:\\n";'
                                    ),
                                    '',
                                    'std::cerr << "  - " << test_vectors[i].id << "\\n";'
                                ]
                            ),
                            '',
                            'test_vectors_failed += 1;'
                        ]
                    ),
                    '',
                    'return test_vectors_failed;'
                ],
                True
            ),
            '',
            src.cpp.lang.gen_main_paramless(
                [
                    '#ifdef MILO_TESTS_CONSTEXPR',
                    'static_assert(test() == 0);',
                    '#endif',
                    '',
                    'volatile auto test_cb = test;',
                    '',
                    'return test_cb() > 0;'
                ]
            ),
            ''
        ]
    )

    return code
