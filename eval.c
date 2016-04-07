#include "eval.h"

struct error apply(struct value car, struct value cdr, struct env* env, struct value* ret) {
    INIT_TRY();
    struct value f;
    TRY(eval(car, env, &f));
    if (f.type != PRIM)
        return WRONG_TYPE("apply", f);

    return f.prim(cdr, env, ret);
}

struct error eval(struct value val, struct env* env, struct value* ret) {
    struct value* tmp;

    switch (val.type) {
        case INT:
            *ret = val;
            return SUCCESS;
        case REAL:
            *ret = val;
            return SUCCESS;
        case SYMBOL:
            tmp = lookup(env, val.symbol);
            if (tmp == NULL) {
                return NOT_IN_SCOPE(val.symbol);
            } else {
                *ret = *tmp;
                return SUCCESS;
            }
        case PRIM:
            *ret = val;
            break;
        case PAIR:
            if (val.pair->cdr.type != PAIR)
                return MALFORMED_EXPR(val);
            else
                return apply(val.pair->car, val.pair->cdr, env, ret);
        case VOID:
            *ret = val;
            return SUCCESS;
    }
    
    // Lol
    return SUCCESS;
}

