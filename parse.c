#include "parse.h"

#define SUCCESS \
    ((struct parse_error) { .code = 0 })
#define UNCLOSED_PARENS(p) \
    ((struct parse_error) { .code = 1, .pos = p })
#define UNEXPECTED_CHAR(p,c) \
    ((struct parse_error) { .code = 2, .pos = p, .c = c })

static inline bool reserved(char c) {
    return c == '(' || c == ')' || c == '\'' || c == '\0' || isspace(c);
}

struct parse_error parse_list(char* str, int* loc, struct value* ret) {
    // Just started list + end of string = unclosed parens
    if (str[*loc] == '\0')
        return UNCLOSED_PARENS(*loc);

    if (str[*loc] == ')') {
        ++*loc;
        *ret = SCMNIL;
    }
    
    struct value list = SCMNIL;
    struct value val;
    struct parse_error err;
    // We simply consume values until the end of the list, and accumulate them.
    while (str[*loc] != ')') {
        if (!PARSED(err = parse_value(str, loc, &val)))
            return err;
        if (val.type == VOID)
            return UNCLOSED_PARENS(*loc);
        list = cons(val, list);
    }
    
    // We built up the elements in reverse order, so now we have to reverse
    // them.
    struct value rev = SCMNIL; 
    while (list.pair != NULL) {
        rev = cons(list.pair->car, rev);
        list = list.pair->cdr;
    }

    ++*loc; // Consume input.
    *ret = rev;
    return SUCCESS;
}

struct parse_error parse_value(char* str, int* loc, struct value* ret) {
    // Skip whitespace preceeding value
    while (isspace(str[*loc])) ++*loc;
    char c = str[*loc];

    if (c == '\0') {
        // Void is unparsable, so we return it on failure.
        *ret = SCMVOID;
        return SUCCESS;
    } else if (c == '(') {
        ++*loc;
        return parse_list(str, loc, ret);
    } else if (c == ')') {
        // If we find a ) and we're not in parse_list, it must be unexpected.
        return (struct parse_error) { .code = 2, .pos = *loc, .c = ')' };
    } else if (c == '\'') {
        // '<exp> -> (quote <exp>)
        *loc += 1;
        struct value val;
        parse_value(str, loc, &val);
        *ret = cons(SCMSYM("quote"), cons(val, SCMNIL));
        return SUCCESS;
    } else {
        // Before you say anything, yes, those are gotos. I apologize.
        // That being said, I feel like the use of gotos in this particular
        // scanario actually made the code much cleaner, and reduced overhead.
        double real;
        double exp;
        int num = 0;

        int i;
        for (i = *loc; !reserved(str[i]); ++i) {
            if (!isdigit(str[i])) {
                // If we come across a '.', and it's been digits so far, we
                // jump to the real number parser.
                if (str[i] == '.')
                    goto real;
                // If there's some other random character, that means that it
                // can't be a number, so it must be a symbol.
                else
                    goto symbol;
            } else {
                // Build the number in the standard way.
                num = (num * 10) + (str[i] - '0');
            }
        }

        *loc = i; // Consume input.
        
        *ret = SCMINT(num);
        return SUCCESS;

real:   i += 1;
        real = (double) num;
        exp = 0.1;
        for (; !reserved(str[i]); ++i) {
            if (!isdigit(str[i])) {
                goto symbol;
            } else {
                // Given the fact that this is all being parsed AFTER the
                // decimal place, we start adding 1/10, 1/100, etc. of the the
                // next digits in the string.
                real += exp * (str[i] - '0');
                exp /= 10;
            }
        }

        *loc = i; // Consume input.

        *ret = SCMREAL(real);
        return SUCCESS;
       
symbol: for (; !reserved(str[i]); ++i); // Get the end of the symbol.
        char* symbol = GC_MALLOC(sizeof(char) * (i+1)); // Allocate the symbol.
        memcpy(symbol, str + *loc, i); // Copy symbol from string.
        symbol[i-*loc] = '\0'; // Null terminate symbol.
        *loc = i; // Consume input.
        *ret = SCMSYM(symbol);
        return SUCCESS;
    }
    
    *ret = SCMVOID;
    return SUCCESS;
}

struct parse_error parse(char* str, struct value** vals, int* len) {
    int n = 0;
    int loc = 0;
    *vals = malloc(sizeof(struct value));
    struct value val;
    struct parse_error err;

    for (;;) {
        if (!PARSED(err = parse_value(str, &loc, &val)))
            return err;
        
        if (val.type == VOID) // End of string
            break;
        
        // Make room for newly parsed value and append.
        *vals = realloc(*vals, sizeof(struct value) * ++n);
        (*vals)[n-1] = val;
    }

    *len = n; 
    return SUCCESS;
}


void display_parse_error(struct parse_error err) {
    fprintf(stderr, "PARSE ERROR: ");
    switch (err.code) {
        case 0:
            break;
        case 1:
            fprintf(stderr, "unmatched parenthesis at character %d\n", err.pos);
            break;
        case 2:
            fprintf(stderr, "unexpected character at %d: %c\n", err.pos, err.c);
            break;
    }
}

