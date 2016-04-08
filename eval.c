#include "eval.h"

struct error apply(struct value car, struct value cdr, struct env** env, struct value* ret) {
    INIT_TRY();
    struct value f;
    TRY(eval(car, env, &f));
    if (f.type == PRIM) {
        return f.prim(cdr, env, ret);
    } else if (f.type == LAMBDA) {
        descend(env);
        struct value val;
        struct value args = cdr;
       
        if (f.lambda.num_args != 0) {
            for (int i = 0; i < f.lambda.num_args; ++i) {
                if (args.type != PAIR)
                    return MALFORMED_EXPR(args);
                if (null(args))
                    return WRONG_NUM_ARGS("lambda", f.lambda.num_args, i);
                TRY(eval(args.pair->car, env, &val));
                insert(*env, f.lambda.args[i], val);
                args = args.pair->cdr;
            }
        }

        if (!null(args)) {
            int i = 0;
            while (!null(args)) {
                if (args.type != PAIR)
                    return MALFORMED_EXPR(args);
                args = args.pair->cdr;
                ++i;
            }

            return WRONG_NUM_ARGS("lambda", f.lambda.num_args, f.lambda.num_args + i);
        }

        for (int i = 0; i < f.lambda.body_len; ++i)
            TRY(eval(f.lambda.body[i], env, &val));

        *ret = val;

        return SUCCESS;
    } else {
        return WRONG_TYPE("apply", f);
    }
}

struct error eval(struct value val, struct env** env, struct value* ret) {
    struct value* tmp;

    switch (val.type) {
        case INT:
            *ret = val;
            return SUCCESS;
        case REAL:
            *ret = val;
            return SUCCESS;
        case SYMBOL:
            tmp = lookup(*env, val.symbol);
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

