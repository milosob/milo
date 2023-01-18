

#pragma once

#define MILO_INTERNAL_ATTRIBUTE_INLINE_true         __attribute__((always_inline)) inline
#define MILO_INTERNAL_ATTRIBUTE_INLINE_false        __attribute__((noinline))
#define MILO_INTERNAL_ATTRIBUTE_INLINE(a_bool)      MILO_INTERNAL_ATTRIBUTE_INLINE_##a_bool

#define MILO_INTERNAL_ATTRIBUTE_TARGET(a_args)      __attribute__((target(a_args)))


