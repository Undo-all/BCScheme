#include "env.h"

struct env* new_env(void) {
    struct env* env = malloc(sizeof(struct env));
    env->trie = malloc(sizeof(struct trie));
    init_trie(env->trie);
    env->up = NULL;
    return env;
}

struct value* lookup(struct env* env, char* sym) {
    struct value* val;
    for (struct env* iter = env; iter != NULL; iter = iter->up) {
        if ((val = lookup_trie(iter->trie, sym)) != NULL)
            return val;
    }

    return NULL;
}

void insert(struct env* env, char* sym, struct value val) {
    insert_trie(env->trie, sym, val); 
}

void descend(struct env** env) {
    struct env* lower = new_env();
    lower->up = *env;
    *env = lower;
}

void ascend(struct env** env) {
    struct env* upper = (*env)->up;
    free_trie((*env)->trie);
    free(*env);
    *env = upper;
}

