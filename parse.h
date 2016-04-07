#pragma once
#include <gc.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "scheme.h"

#define PARSED(x) ((x).code==0)

// Nicely packs to 64 bits.
struct parse_error {
    uint8_t code;
    uint32_t pos; 
    char c; 
};

struct parse_error parse_value(char*, int*, struct value*);
struct parse_error parse(char*, struct value**, int*);
void display_parse_error(struct parse_error);

