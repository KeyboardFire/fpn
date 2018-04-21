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

#ifndef __FPN_H__
#define __FPN_H__

#include <gmp.h>
#include <mpfr.h>

// I'm very sorry
#define BIND(val, x) \
    mpfr_ptr x = (val).data; \
    if (val.type == RATIONAL) { mpq_ptr x = (val).data;
#define OR } else
#define BIND_INT(val, x, op) \
    long x; \
    BIND(val, tmp) { \
        if (mpz_cmp_si(mpq_denref(tmp), 1)) { \
            fprintf(stderr, op ": found non-integer on stack\n"); \
            return; \
        } else if (!mpz_fits_slong_p(mpq_numref(tmp))) { \
            fprintf(stderr, op ": found too-large integer on stack\n"); \
            return; \
        } else { \
            x = mpz_get_si(mpq_numref(tmp)); \
        } \
    } OR { \
        if (!mpfr_integer_p(tmp)) { \
            fprintf(stderr, op ": found non-integer on stack\n"); \
            return; \
        } else if (!mpfr_fits_slong_p(tmp, MPFR_RNDN)) { \
            fprintf(stderr, op ": found too-large integer on stack\n"); \
            return; \
        } else { \
            x = mpfr_get_si(tmp, MPFR_RNDN); \
        } \
    }
#define CHECK(n, op) if (fpn->stackSize < n) { \
            fprintf(stderr, op ": not enough operands " \
                    "(found %d, expected " #n ")\n", fpn->stackSize); \
            return; \
        }
#define ARG(n) (fpn->stack[fpn->stackSize-(n)])
#define ARG1 ARG(1)
#define ARG2 ARG(2)
#define ARG3 ARG(3)

#define RATIONAL 1
#define FLOAT 2
struct val {
    void *data;
    char type;
};

#define FPN_MIN_EXTRA 50
#define FPN_MAX_EXTRA 100
struct fpn {
    struct val *stack;
    int stackSize;
    int bufSize;
    int round;
};

struct fpn *fpn_init();
void fpn_run(struct fpn *fpn, char *code);
void fpn_destroy(struct fpn *fpn);

#endif
