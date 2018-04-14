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

#include "fpn.h"

void fpn_push(struct fpn *fpn, void *data, char type) {
    if (fpn->stackSize == fpn->bufSize) {
        fpn->stack = realloc(fpn->stack,
                (fpn->bufSize += FPN_MAX_EXTRA) * sizeof *fpn->stack);
    }
    fpn->stack[fpn->stackSize].data = data;
    fpn->stack[fpn->stackSize].type = type;
    ++fpn->stackSize;
}

void fpn_push_q(struct fpn *fpn, mpq_t q) { fpn_push(fpn, q, RATIONAL); }
void fpn_push_f(struct fpn *fpn, mpfr_t f) { fpn_push(fpn, f, FLOAT); }

struct fpn *fpn_init() {
    struct fpn *fpn = malloc(sizeof *fpn);
    fpn->stack = malloc(FPN_MAX_EXTRA * sizeof *fpn->stack);
    fpn->stackSize = 0;
    fpn->bufSize = FPN_MAX_EXTRA;
    return fpn;
}

void fpn_run(struct fpn *fpn, char *code) {
    while (*code) {
        switch (*code) {
            case '1': {
                mpq_t q;
                mpq_init(q);
                mpq_set_ui(q, 1, 1);
                fpn_push_q(fpn, q);
                break;
            }
            case '+':
                break;
            case 'p':
                // TODO make this fail if the stack is empty
                BIND(fpn->stack[fpn->stackSize-1], val) {
                    mpq_out_str(stdout, 10, val);
                } OR {
                    mpfr_out_str(stdout, 10, 0, val, MPFR_RNDN);
                } UNBIND;
                puts("");
                break;
        }
        ++code;
    }
}

void fpn_destroy(struct fpn *fpn) {
    for (int i = 0; i < fpn->stackSize; ++i) {
        BIND(fpn->stack[i], val) {
            mpq_clear(val);
        } OR {
            mpfr_clear(val);
        } UNBIND;
    }
    free(fpn->stack);
    free(fpn);
}
