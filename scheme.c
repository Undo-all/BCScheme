#include "scheme.h"

struct value cons(struct value x, struct value y) {
    struct pair* pair = GC_MALLOC(sizeof(struct pair));
    pair->car = x;
    pair->cdr = y;
    return (struct value) { .type = PAIR, .pair = pair };
}

bool null(struct value x) {
    return x.type == PAIR && x.pair == NULL;
}

void fdisplay_pair(FILE* f, struct value car, struct value cdr) {
    fdisplay(f, car);
    
    if (cdr.type != PAIR) {
        fprintf(f, " . ");
        fdisplay(f, cdr);
        fprintf(f, ")");
    } else if (cdr.pair == NULL) {
        fprintf(f, ")");
    } else {
        fprintf(f, " ");
        fdisplay_pair(f, cdr.pair->car, cdr.pair->cdr);
    }
}

void fdisplay(FILE* f, struct value val) {
    switch (val.type) {
        case INT:
            fprintf(f, "%d", val.intv);
            break;
        case REAL:
            fprintf(f, "%f", val.real);
            break;
        case SYMBOL:
            fprintf(f, "%s", val.symbol);
            break;
        case PRIM:
            fprintf(f, "<primative>");
        case PAIR:
            if (val.pair == NULL) {
                fprintf(f, "()");
            } else {
                fprintf(f, "(");
                fdisplay_pair(f, val.pair->car, val.pair->cdr);
            }

            break;
        case VOID:
            break;
    }
}

