#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "eval.h"
#include "parse.h"
#include "env.h"
#include "scheme.h"
#include "primatives.h"

int main(void) {
    GC_INIT();
    
    struct env* env = new_env();
    
    insert(env, "quote", SCMPRIM(scm_quote));
    insert(env, "cons", SCMPRIM(scm_cons));
    insert(env, "car", SCMPRIM(scm_car));
    insert(env, "cdr", SCMPRIM(scm_cdr));

    // A read-eval loop!
    char* buff;
    size_t bufflen = 0;

    while (true) {
        printf("> ");
        fflush(stdout);
        if (getline(&buff, &bufflen, stdin) == -1)
            break;
        
        /*
        int len;
        fdisplay(stdout, eval(parse(buff, &len)[0], &env));
        printf("\n");
        */
        
        struct parse_error parse_err;

        struct value* vals;
        int len;
        if (PARSED(parse_err = parse(buff, &vals, &len))) {
            struct value ret;
            struct error err = eval(vals[0], &env, &ret);
            if (!SUCCEEDED(err)) {
                display_error(err);
            } else {
                fdisplay(stdout, ret);
                printf("\n");
            }
        } else {
            display_parse_error(parse_err);
            printf("\n");
        }
    }

    return 0;
}

