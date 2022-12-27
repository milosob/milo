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
        assert 'from' in test_vector
        assert 'to' in test_vector

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
                    '<milo/primitive/codec/test.h>'
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
                    'const milo::container::chars_const_view_dynamic from;',
                    '',
                    'const milo::container::chars_const_view_dynamic to;'
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
                                        '.from =',
                                        src.cpp.lang.gen_str(
                                            t_vector['from'],
                                            end='_cv,'
                                        ),
                                        '.to =',
                                        src.cpp.lang.gen_bytes_to_hex_str(
                                            bytes.fromhex(
                                                t_vector['to']
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
                    'milo::primitive::codec::test<t_impl>::decode',
                    [
                        'from,',
                        'to'
                    ]
                ]
            ),
            '',
            src.cpp.milo.gen_test_primitive_macros(),
            '',
            src.cpp.milo.gen_test_primitive_main(
                test_impl['cpltime'],
                test_impl['runtime']
            ),
            ''
        ]
    )

    return code
