#pragma once

#include <gc.h>
#include <stdlib.h>

#include "scheme.h"

struct trie {
    struct value* val;
    struct trie* children[89];
};

void init_trie(struct trie*);
struct value* lookup_trie(struct trie*, char*);
void insert_trie(struct trie*, char*, struct value);
void free_trie(struct trie*);

