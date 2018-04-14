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
#define BIND(val, x) do { \
    mpfr_t x; x[0] = *(__mpfr_struct*)(val).data; \
    if (val.type == RATIONAL) { mpq_t x; x[0] = *(__mpq_struct*)(val).data;
#define OR } else
#define UNBIND } while (0);

#define RATIONAL 1
#define FLOAT 2
struct val {
    void *data;
    char type;
};

#define FPN_MAX_EXTRA 30
struct fpn {
    struct val *stack;
    int stackSize;
    int bufSize;
};

struct fpn *fpn_init();
void fpn_run(struct fpn *fpn, char *code);
void fpn_destroy(struct fpn *fpn);

#endif
