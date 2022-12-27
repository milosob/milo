import src.cpp.lang


def gen_test_primitive_core(
        a_expr
) -> tuple:
    return (
        'template<',
        [
            'typename t_impl'
        ],
        '>',
        src.cpp.lang.gen_function(
            'test',
            [],
            [
                src.cpp.lang.gen_loop_for_range(
                    'test_vector',
                    'test_vectors',
                    [
                        'auto result = ' + a_expr[0] + '(',
                        [
                            f'test_vector.{member}'
                            for member in a_expr[1]
                        ],
                        ');',
                        '',
                        src.cpp.lang.gen_if(
                            '!result',
                            'return test_vector.id;'
                        )
                    ]
                ),
                '',
                'return 0;'
            ],
            a_constexpr=True,
            a_noexcept=True,
            a_returns='size_t'
        )
    )


def gen_test_primitive_macros(
) -> tuple:
    return (
        '#ifdef MILO_TEST_CONSTEXPR',
        '#define TEST_CPLTIME(a_impl) {static_assert(test<a_impl>() == 0, "cpltime error of " #a_impl);}',
        '#else',
        '#define TEST_CPLTIME(a_impl) {static_assert(true, "cpltime error of " #a_impl);}',
        '#endif',
        '#define TEST_RUNTIME(a_impl) {volatile auto test_cb = test<a_impl>;'
        'if (test_cb() != 0){std::cerr << "runtime error of " #a_impl "\\n";return 1;}}'
    )


def gen_test_primitive_main(
        a_cpltime: list,
        a_runtime: list
) -> tuple:
    return (
        src.cpp.lang.gen_main_argless(
            [
                'using namespace milo::primitive;',
                '',
                tuple([
                    f'TEST_CPLTIME({impl});'
                    for impl in a_cpltime
                ]),
                '',
                tuple([
                    f'TEST_RUNTIME({impl});'
                    for impl in a_runtime
                ]),
                '',
                f'return {"0" if len(a_cpltime) != 0 or len(a_runtime) != 0 else "1"};'
            ]
        )
    )
