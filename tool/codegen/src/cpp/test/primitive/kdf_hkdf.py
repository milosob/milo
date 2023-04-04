import src.cpp.lang
import src.cpp.milo


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
        assert 'info' in test_vector
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
                    '<milo/internal.h>',
                    '<milo/literal.h>'
                ]
            ),
            '',
            src.cpp.lang.gen_include(
                test_include
            ),
            src.cpp.lang.gen_include(
                [
                    '<milo/primitive/kdf/test.h>'
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
                    'const milo::container::chars_const_view_dynamic info;',
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
                                        '.info =',
                                        src.cpp.lang.gen_bytes_to_hex_str(
                                            bytes.fromhex(
                                                t_vector['info']
                                            ),
                                            end='_cv,'
                                        ),
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
            src.cpp.milo.gen_test_primitive_core(
                [
                    'milo::primitive::kdf::test::derive<t_impl>',
                    [
                        'ikm,',
                        'salt,',
                        'info,',
                        'key'
                    ]
                ]
            ),
            '',
            src.cpp.milo.gen_test_primitive_macros(
                a_namespace='kdf',
                a_differential=True
            ),
            '',
            src.cpp.milo.gen_test_primitive_main(
                test_impl['cpltime'],
                test_impl['runtime'],
                a_differential=True
            ),
            ''
        ]
    )

    return code
