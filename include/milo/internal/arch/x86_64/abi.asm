; asmsyntax=nasm

%ifdef MILO_INTERNAL_ABI_UNIX
    %define abi_arg_0 rdi
    %define abi_arg_1 rsi
    %define abi_arg_2 rdx
    %define abi_arg_3 rcx
    %define abi_arg_4 r8
    %define abi_arg_5 r9
%endif

%ifdef MILO_INTERNAL_ABI_WIND
    %define abi_arg_0 rcx
    %define abi_arg_1 rdx
    %define abi_arg_2 r8
    %define abi_arg_3 r9
%endif
