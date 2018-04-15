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

#include "fpn.h"
#include "ops.h"
#include "util.h"

struct fpn *fpn_init() {
    struct fpn *fpn = malloc(sizeof *fpn);
    fpn->stack = malloc(FPN_MIN_EXTRA * sizeof *fpn->stack);
    fpn->stackSize = 0;
    fpn->bufSize = FPN_MIN_EXTRA;
    fpn->round = MPFR_RNDN;
    return fpn;
}

void fpn_run(struct fpn *fpn, char *code) {
    while (*code) {
        if ('0' <= *code && *code <= '9') {
            code = fpn_op_const(fpn, code);
        } else switch (*code) {
            case '+': fpn_op_add(fpn); ++code; break;
            case '-': fpn_op_sub(fpn); ++code; break;
            case '*': fpn_op_mul(fpn); ++code; break;
            case '/': fpn_op_div(fpn); ++code; break;
            case 'p': fpn_op_print(fpn); ++code; break;
            default: ++code;
        }
    }
}

void fpn_destroy(struct fpn *fpn) {
    for (int i = 0; i < fpn->stackSize; ++i) fpn_clear(fpn->stack[i]);
    free(fpn->stack);
    free(fpn);
}
