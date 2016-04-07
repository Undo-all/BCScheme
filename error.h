#pragma once

#include <stdint.h>

#include "scheme.h"

// I think this is roughly the right way to do error handling? I can't think of
// any better ways, at least.

#define SUCCEEDED(x) ((x).code==0)
// Borrowing this one from rust.
// Also, ew.
#define INIT_TRY() struct error __err;
#define TRY(x) if (!SUCCEEDED(__err=(x))) return __err;
#define ASSERT_TYPE(n,x,t) if (x.type!=t) return WRONG_TYPE(n,x);

struct error {
    uint8_t code;
    union {
        char* symbol;

        struct {
            char* to;
            short expected;
            short actual;
        } numargs;

        struct value malformed;

        struct {
            char* to;
            struct value val; 
        } wrong_type;

        char* to;
    };
};

#define SUCCESS \
    ((struct error) { .code = 0 })
#define NOT_IN_SCOPE(s) \
    ((struct error) { .code = 1, .symbol = s })
#define WRONG_NUM_ARGS(t,e,a)               \
    ((struct error) { .code = 2             \
                    , .numargs.to = t       \
                    , .numargs.expected = e \
                    , .numargs.actual = a   \
                    })
#define MALFORMED_EXPR(e) \
    ((struct error) { .code = 3, .malformed = e })
#define WRONG_TYPE(n,e) \
    ((struct error) { .code = 4, .wrong_type.to = n, .wrong_type.val = e })
#define EMPTY_LIST(t) \
    ((struct error) { .code = 5, .to = t })

void display_error(struct error);

