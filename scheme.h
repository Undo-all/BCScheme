#pragma once

#include <gc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Types
enum {
    INT = 0,
    REAL,
    SYMBOL,
    PRIM,
    PAIR,
    VOID, 
};

struct env;
struct error;

struct value {
    uint8_t type;
    union {
        int intv; 
        double real;
        char* symbol;
        struct error (*prim)(struct value, struct env**, struct value*);
        struct pair* pair; 
    };
};

struct pair {
    struct value car;
    struct value cdr;
};

// Constructors
#define SCMNIL ((struct value) { .type = PAIR, .pair = NULL })
#define SCMINT(n) ((struct value) { .type = INT, .intv = n })
#define SCMREAL(x) ((struct value) { .type = REAL, .real = x })
#define SCMSYM(s) ((struct value) { .type = SYMBOL, .symbol = s })
#define SCMPRIM(f) ((struct value) { .type = PRIM, .prim = f })
#define SCMVOID ((struct value) { .type = VOID })

struct value cons(struct value x, struct value y);
bool null(struct value x);

// Literally the most useful function of all time
void fdisplay(FILE*, struct value);

