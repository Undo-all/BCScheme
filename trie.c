#include "trie.h"

// Yeah... if you wanna know how this one works (and you don't know already), 
// you'll have to visit wikipedia 'cause I ain't commenting dis shit.

void init_trie(struct trie* trie) {
    trie->val = NULL;
    for (int i = 0; i < 89; ++i) trie->children[i] = NULL;
}

int translate(char c) {
    if (c == '!')
        return 0;
    else if (c < '\'')
        return c - '"';
    else
        return c - '"' - 2;
}

struct value* lookup_trie(struct trie* trie, char* sym) {
    struct trie* iter = trie;
    for (int i = 0; sym[i] != '\0'; ++i)
        if (iter->children[translate(sym[i])] == NULL)
            return NULL;
        else
            iter = iter->children[translate(sym[i])];

    return iter->val;
}

void insert_trie(struct trie* trie, char* sym, struct value val) {
    int index;
    struct trie* iter = trie;

    for (int i = 0; sym[i] != '\0'; ++i) {
        index = translate(sym[i]);
        if (iter->children[index] == NULL) {
            int j;
            for (j = i; sym[j+1] != '\0'; ++j) {
                index = translate(sym[j]);
                iter->children[index] = malloc(sizeof(struct trie));
                init_trie(iter->children[index]);
                iter = iter->children[index];
            }
            
            index = translate(sym[j]);
            iter->children[index] = malloc(sizeof(struct trie));
            init_trie(iter->children[index]);
            iter->children[index]->val = GC_MALLOC(sizeof(struct value));
            *iter->children[index]->val = val;
            return;
        } else {
            iter = iter->children[index];
        }
    }

    iter->val = GC_MALLOC(sizeof(struct value));
    *iter->val = val;
}

void free_allocated_trie(struct trie* trie) {
    for (int i = 0; i < 89; ++i)
        if (trie->children[i] != NULL)
            free_allocated_trie(trie->children[i]);
    GC_FREE(trie->val);
    free(trie);
}

void free_trie(struct trie* trie) {
    for (int i = 0; i < 89; ++i)
        if (trie->children[i] != NULL)
            free_allocated_trie(trie->children[i]);
}

