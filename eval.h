#pragma once

#include <string.h>

#include "env.h"
#include "error.h"
#include "scheme.h"

struct error eval(struct value, struct env*, struct value*);

