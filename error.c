#include "error.h"

void display_error(struct error err) {
    fprintf(stderr, "ERROR: ");

    switch (err.code) {
        case 0:
            break;
        case 1:
            fprintf(stderr, "not in scope: %s\n", err.symbol);
            break;
        case 2:
            fprintf( stderr
                   , "wrong number of arguments to %s (expected %d, got %d)\n"
                   , err.numargs.to
                   , err.numargs.expected
                   , err.numargs.actual
                   );
            break;
        case 3:
            fprintf(stderr, "malformed expression: ");
            fdisplay(stderr, err.malformed);
            fprintf(stderr, "\n");
            break;
        case 4:
            fprintf(stderr, "wrong type to %s: ", err.wrong_type.to);
            fdisplay(stderr, err.wrong_type.val);
            fprintf(stderr, "\n");
            break;
    }
}

