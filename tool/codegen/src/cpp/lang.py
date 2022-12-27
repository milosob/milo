def gen_beg(
) -> str:
    return '\n\n'


def gen_end(
) -> str:
    return ''


def gen_break(
) -> str:
    return 'break;'


def gen_continue(
) -> str:
    return 'continue;'


def gen_goto(
        a_label: str
) -> str:
    return f'goto {a_label};'


def gen_newline(
) -> str:
    return '\n'


def gen_include(
        a_includes
) -> tuple:
    return tuple(
        [
            f'#include {include}'
            for include in a_includes
        ]
    )


def gen_body(
        a_body,
        a_ident: int = 0
) -> str:
    pad: str = ' ' * a_ident * 4
    code: str = ''

    for part in a_body:
        part_type = type(part)
        if part_type is list:
            # noinspection PyTypeChecker
            code += gen_body(
                part,
                a_ident + 1
            )
            continue
        elif part_type is tuple:
            # noinspection PyTypeChecker
            code += gen_body(
                part,
                a_ident
            )
            continue
        else:
            code += pad + part + '\n'
    return code


def gen_block(
        a_body,
        end: str = ''
) -> tuple:
    return (
        '{',
        a_body if type(a_body) is list else [a_body],
        '}' + end,
    )


def gen_loop_for(
        a_expression: list[str],
        a_body
) -> tuple:
    expression_len = len(a_expression)
    expression: list[str] = a_expression + ['', '', '']

    fill_1 = ' ' if expression_len > 1 else ''
    fill_2 = ' ' if expression_len > 2 else ''

    return (
        f'for ({expression[0]};{fill_1}{expression[1]};{fill_2}{expression[2]})',
        gen_block(
            a_body
        )
    )


def gen_loop_for_range(
        a_range_decl,
        a_range_expr,
        a_body
) -> tuple:
    return (
        f'for (auto&& {a_range_decl} :{a_range_expr})',
        gen_block(
            a_body
        )
    )


def gen_loop_while(
        a_expression: str,
        a_body
) -> tuple:
    assert a_expression

    return (
        f'while ({a_expression})',
        gen_block(
            a_body
        )
    )


def gen_if(
        a_expression: str,
        a_body,
        a_body_else=None
) -> tuple:
    assert a_expression

    code = (
        f'if ({a_expression})',
        gen_block(
            a_body
        )
    )

    if a_body_else:
        code = code + (
            'else',
            gen_block(
                a_body_else
            )
        )

    return code


def gen_type(
        a_type: str,
        a_name: str,
        a_definition=None
) -> tuple:
    return (
        f'{a_type} {a_name}',
        gen_block(
            a_definition if a_definition is not None else [],
            end=';'
        )
    )


def gen_str(
        a_str: str,
        wrap: int = 64,
        end: str = ''
) -> tuple:
    chunks = [
        f'"{a_str[i: i + wrap]}"'
        for i in range(0, len(a_str), wrap)
    ]

    if len(chunks) == 0:
        chunks.append(end)
    else:
        chunks[len(chunks) - 1] += end

    return tuple(chunks)


def gen_bytes_to_hex_str(
        a_bytes: bytes,
        wrap: int = 16,
        end: str = ''
) -> tuple:
    parts = [
        a_bytes[i: i + wrap]
        for i in range(0, len(a_bytes), wrap)
    ]

    chunks = []
    for chunk in parts:
        s = '"'
        for byte in chunk:
            s += f'\\x{byte:02x}'
        s += '"'
        chunks.append(s)

    if len(chunks) == 0:
        chunks.append('""' + end)
    else:
        chunks[len(chunks) - 1] += end

    return tuple(chunks)


def gen_struct(
        a_name: str,
        a_definition=None
) -> tuple:
    return gen_type(
        'struct',
        a_name,
        a_definition
    )


def gen_class(
        a_name: str,
        a_definition=None
) -> tuple:
    return gen_type(
        'class',
        a_name,
        a_definition
    )


def gen_function(
        a_name: str,
        a_args,
        a_body,
        **kwargs
) -> tuple:
    returns = 'auto'
    constexpr = ''
    noexcept = 'noexcept(false)'
    static = ()

    try:
        returns = kwargs['a_returns']
    except KeyError:
        pass

    try:
        constexpr = 'constexpr' if kwargs['a_constexpr'] else constexpr
    except KeyError:
        pass

    try:
        noexcept = 'noexcept(true)' if kwargs['a_noexcept'] else noexcept
    except KeyError:
        pass

    try:
        static = ('static',) if kwargs['a_static'] else static
    except KeyError:
        pass

    return (
        static,
        f'{constexpr} auto'.strip(),
        f'{a_name}(',
        a_args,
        f') {noexcept} -> {returns}',
        gen_block(
            a_body
        )
    )


def gen_main(
        a_body
) -> tuple:
    return gen_function(
        'main',
        [
            '[[maybe_unused]]',
            'int a_argc,',
            '[[maybe_unused]]',
            'const char* a_argv[]'
        ],
        a_body,
        a_noexcept=False,
        a_returns='int'
    )


def gen_main_argless(
        a_body
) -> tuple:
    return gen_function(
        'main',
        [],
        a_body,
        a_noexcept=False,
        a_returns='int'
    )
