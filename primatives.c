#include "primatives.h"

// Implementation of some lisp primatives.

// Because of the fact that in the implementation of apply, it doesn't evaluate
// the arguments of the function before passing them to the primative, special
// forms can be implemented as primatives. This improves speed because they can
// be stored in the environment (implemented with a trie) instead of having to 
// manually strcmp a symbol value with a bunch of special forms.
// The disadvantage of such a system is that the implementation of the
// primatives themselves can get rather messy, but this is helped by a whole
// bunch of helper functions.

bool endofargs(struct value args) {
    return args.pair == NULL;
}

// Counts the number of arguments for generation of error messages.
struct error countargs(struct value args, int* ret) {
    if (args.type != PAIR)
        return MALFORMED_EXPR(args);
    
    int num = 0;
    while (args.pair != NULL) {
        if (args.type != PAIR)
            return MALFORMED_EXPR(args);
        args = args.pair->cdr;
        ++num;
    }

    *ret = num;
    return SUCCESS;
}

// Gets a single argument, automatically checking for malformed expressions
// (e.g. (+ 2 . 3)), and if the argument list has ended.
// Also, error handling == very long function declarations
struct error getarg(struct value* args, char* name, int* num, int expected, struct env** env, struct value* ret) {
    if (args->type != PAIR)
        return MALFORMED_EXPR(*args);
    
    if (args->pair == NULL)
        return WRONG_NUM_ARGS(name, expected, *num);
    
    struct error err;  
    if (!SUCCEEDED(err = eval(args->pair->car, env, ret)))
        return err;
    
    ++*num;
    *args = args->pair->cdr;
    return SUCCESS;
}

// Asserts that the end of an arguments list has been reached.
struct error endargs(struct value args, char* name, int num, int expected) {
    INIT_TRY();

    if (args.type != PAIR)
        return MALFORMED_EXPR(args);
    if (!endofargs(args)) {
        int actual;
        TRY(countargs(args, &actual));
        return WRONG_NUM_ARGS(name, expected, actual + num);
    }
    
    return SUCCESS;
}

struct error scm_quote(struct value args, struct env** env, struct value* ret) {
    if (args.pair == NULL)
        return WRONG_NUM_ARGS("quote", 1, 0);

    if (args.pair->cdr.type != PAIR)
        return MALFORMED_EXPR(args);

    if (args.pair->cdr.pair != NULL) {
        INIT_TRY();
        int actual;
        TRY(countargs(args, &actual));
        return WRONG_NUM_ARGS("quote", 1, 1 + actual);
    }

    *ret = args.pair->car;
    return SUCCESS;
}

struct error scm_cons(struct value args, struct env** env, struct value* ret) {
    INIT_TRY();
    int num = 0;
    struct value x, y;
    TRY(getarg(&args, "cons", &num, 2, env, &x));
    TRY(getarg(&args, "cons", &num, 2, env, &y));
    TRY(endargs(args, "cons", num, 2));
    *ret = cons(x, y);
    return SUCCESS;
}

struct error scm_car(struct value args, struct env** env, struct value* ret) {
    INIT_TRY();
    int num = 0;
    struct value x;
    TRY(getarg(&args, "car", &num, 1, env, &x));
    TRY(endargs(args, "car", num, 1));
    ASSERT_TYPE("car", x, PAIR);
    if (x.pair == NULL)
        return EMPTY_LIST("car");
    *ret = x.pair->car;
    return SUCCESS;
}

struct error scm_cdr(struct value args, struct env** env, struct value* ret) {
    INIT_TRY();
    int num = 0;
    struct value x;
    TRY(getarg(&args, "cdr", &num, 1, env, &x));
    TRY(endargs(args, "cdr", num, 1));
    ASSERT_TYPE("cdr", x, PAIR);
    if (x.pair == NULL)
        return EMPTY_LIST("cdr");
    *ret = x.pair->cdr;
    return SUCCESS;
}

/*
struct value scm_add(struct value args, struct env** env) {
    if (args.type != PAIR)
        ERROR("malformed expression");
    
    int intsum = 0;
    while (true) {
        if (args.pair == NULL)
            return SCMINT(intsum);

        struct value val = eval(args.pair->car, env);

        if (val.type == INT) {
            intsum += val.intv;
        } else if (val.type == REAL) {
            double realsum = (double) intsum;
            while (true) {
                if (args.pair->cdr.type != PAIR)
                    ERROR("malformed expression");
                args = args.pair->cdr;
                val = args.pair->car;

                if (val.pair == NULL)
                    return SCMREAL(realsum);

                if (val.type == INT)
                    realsum += val.intv;
                else if (val.type == REAL)
                    realsum += val.real;
                else
                    ERROR("attempt to pass non-number to +");
            }
        } else {
            ERROR("attempt to pass non-number to +");
        }
        
        if (args.pair->cdr.type == PAIR) 
            args = args.pair->cdr;
    }
}
*/
