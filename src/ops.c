/*
 * fpn - an interactive unit-aware RPN calculator
 * Copyright (C) 2018  Keyboard Fire <andy@keyboardfire.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ops.h"
#include "util.h"

// utility functions used in operator implementations

void fpn_push(struct fpn *fpn, void *data, char type) {
    if (fpn->stackSize == fpn->bufSize) {
        fpn->stack = realloc(fpn->stack,
                (fpn->bufSize += FPN_MIN_EXTRA) * sizeof *fpn->stack);
    }
    fpn->stack[fpn->stackSize].data = data;
    fpn->stack[fpn->stackSize].type = type;
    ++fpn->stackSize;
}

void fpn_push_q(struct fpn *fpn, mpq_t q) { fpn_push(fpn, q, RATIONAL); }
void fpn_push_f(struct fpn *fpn, mpfr_t f) { fpn_push(fpn, f, FLOAT); }

void fpn_pop(struct fpn *fpn) {
    fpn_clear(fpn->stack[--fpn->stackSize]);
    if (fpn->bufSize - fpn->stackSize > FPN_MAX_EXTRA) {
        fpn->stack = realloc(fpn->stack,
                (fpn->stackSize + FPN_MIN_EXTRA) * sizeof *fpn->stack);
    }
}

// operators begin here

void fpn_op_add(struct fpn *fpn) {
    CHECK(2, "+");
    BIND(ARG2, a) {
        BIND(ARG1, b) {
            mpq_add(a, a, b);
        } OR {
            mpfr_add_q(b, b, a, fpn->round);
            struct val tmp = ARG2;
            ARG2 = ARG1;
            ARG1 = tmp;
        }
    } OR {
        BIND(ARG1, b) {
            mpfr_add_q(a, a, b, fpn->round);
        } OR {
            mpfr_add(a, a, b, fpn->round);
        }
    }
    fpn_pop(fpn);
}

void fpn_op_sub(struct fpn *fpn) {
    CHECK(2, "-");
    BIND(ARG2, a) {
        BIND(ARG1, b) {
            mpq_sub(a, a, b);
        } OR {
            mpfr_sub_q(b, b, a, fpn->round);
            mpfr_neg(b, b, fpn->round);
            struct val tmp = ARG2;
            ARG2 = ARG1;
            ARG1 = tmp;
        }
    } OR {
        BIND(ARG1, b) {
            mpfr_sub_q(a, a, b, fpn->round);
        } OR {
            mpfr_sub(a, a, b, fpn->round);
        }
    }
    fpn_pop(fpn);
}

void fpn_op_mul(struct fpn *fpn) {
    CHECK(2, "*");
    BIND(ARG2, a) {
        BIND(ARG1, b) {
            mpq_mul(a, a, b);
        } OR {
            mpfr_mul_q(b, b, a, fpn->round);
            struct val tmp = ARG2;
            ARG2 = ARG1;
            ARG1 = tmp;
        }
    } OR {
        BIND(ARG1, b) {
            mpfr_mul_q(a, a, b, fpn->round);
        } OR {
            mpfr_mul(a, a, b, fpn->round);
        }
    }
    fpn_pop(fpn);
}

void fpn_op_div(struct fpn *fpn) {
    CHECK(2, "/");
    BIND(ARG2, a) {
        BIND(ARG1, b) {
            mpq_div(a, a, b);
        } OR {
            mpfr_div_q(b, b, a, fpn->round);
            mpfr_ui_div(b, 1, b, fpn->round);
            struct val tmp = ARG2;
            ARG2 = ARG1;
            ARG1 = tmp;
        }
    } OR {
        BIND(ARG1, b) {
            mpfr_div_q(a, a, b, fpn->round);
        } OR {
            mpfr_div(a, a, b, fpn->round);
        }
    }
    fpn_pop(fpn);
}

void fpn_op_print(struct fpn *fpn) {
    CHECK(1, "p");
    BIND(ARG1, val) {
        mpq_out_str(stdout, 10, val);
    } OR {
        mpfr_out_str(stdout, 10, 0, val, fpn->round);
    }
    puts("");
}

void fpn_op_dump(struct fpn *fpn) {
    printf("[");
    for (int i = 0; i < fpn->stackSize; ++i) {
        if (i) printf(" ");
        BIND(fpn->stack[i], val) {
            mpq_out_str(stdout, 10, val);
        } OR {
            mpfr_out_str(stdout, 10, 0, val, fpn->round);
        }
    }
    puts("]");
}

void pick(struct fpn *fpn, long n) {
    CHECK(n, "pick [generic]");
    BIND(ARG(n), val) {
        mpq_ptr copy = malloc(sizeof *copy);
        mpq_init(copy);
        mpq_set(copy, val);
        fpn_push(fpn, copy, RATIONAL);
    } OR {
        mpfr_ptr copy = malloc(sizeof *copy);
        mpfr_init(copy);
        mpfr_set(copy, val, fpn->round);
        fpn_push(fpn, copy, FLOAT);
    }
}

void roll(struct fpn *fpn, long n) {
    CHECK(n, "roll [generic]");
    struct val tmp = ARG(n);
    memmove(fpn->stack + fpn->stackSize - n,
            fpn->stack + fpn->stackSize - n + 1,
            (n - 1) * sizeof(*fpn->stack));
    fpn->stack[fpn->stackSize - 1] = tmp;
}

void del(struct fpn *fpn, long n) {
    CHECK(n, "del [generic]");
    roll(fpn, n);
    fpn_pop(fpn);
}

void fpn_op_dup(struct fpn *fpn)  { pick(fpn, 1); }
void fpn_op_drop(struct fpn *fpn) { del(fpn, 1);  }
void fpn_op_swap(struct fpn *fpn) { roll(fpn, 2); }

void fpn_op_over(struct fpn *fpn) { pick(fpn, 2); }
void fpn_op_nip(struct fpn *fpn)  { del(fpn, 2);  }
void fpn_op_rot(struct fpn *fpn)  { roll(fpn, 3); }

void fpn_op_pick(struct fpn *fpn) {
    CHECK(1, "pick");
    BIND_INT(ARG1, count, "pick");
    fpn_pop(fpn);
    pick(fpn, count);
}
void fpn_op_del(struct fpn *fpn) {
    CHECK(1, "del");
    BIND_INT(ARG1, count, "del");
    fpn_pop(fpn);
    del(fpn, count);
}
void fpn_op_roll(struct fpn *fpn) {
    CHECK(1, "roll");
    BIND_INT(ARG1, count, "roll");
    fpn_pop(fpn);
    roll(fpn, count);
}

char* fpn_op_const(struct fpn *fpn, char *code) {
    int isFloat = 0;
    char *end, old;
    for (end = code; *end; ++end) {
        if (*end == '.') isFloat = 1;
        else if (*end < '0' || '9' < *end) break;
    }
    old = *end;
    *end = 0;

    if (isFloat) {
        mpfr_ptr f = malloc(sizeof *f);
        mpfr_init(f);
        mpfr_set_str(f, code, 10, fpn->round); // TODO assert?
        fpn_push_f(fpn, f);
    } else {
        mpq_ptr q = malloc(sizeof *q);
        mpq_init(q);
        mpq_set_str(q, code, 10); // TODO assert?
        fpn_push_q(fpn, q);
    }

    *end = old;
    return end;
}
