#pragma once

#include "eval.h"
#include "scheme.h"

struct error scm_quote(struct value, struct env**, struct value*);
struct error scm_add(struct value, struct env**, struct value*);
struct error scm_cons(struct value, struct env**, struct value*);
struct error scm_car(struct value, struct env**, struct value*);
struct error scm_cdr(struct value, struct env**, struct value*);

