#pragma once

#include <gc.h>
#include <stdlib.h>

#include "scheme.h"

// This environment structure is modeled as a trie for SPEEEEED
struct env {
    struct value* val;
    // 89 printable characters, not including whitespace, parens, and quotes.
    // RIP space efficiency
    struct env* children[89];
};

void init_env(struct env*);
struct value* lookup(struct env*, char*);
void insert(struct env*, char*, struct value);
void free_env(struct env*);

