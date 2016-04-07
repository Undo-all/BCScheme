#pragma once

#include <stdlib.h>

#include "trie.h"
#include "scheme.h"

struct env {
    struct trie* trie;
    struct env* up;
};

struct env* new_env(void);
void insert(struct env*, char*, struct value);
struct value* lookup(struct env*, char*);
void descent(struct env**);
void ascent(struct env**);

