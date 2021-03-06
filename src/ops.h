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

#ifndef __OPS_H__
#define __OPS_H__

#include "fpn.h"

#define FORALL_OPS(_) \
    _(add)   \
    _(sub)   \
    _(mul)   \
    _(div)   \
    _(print) \
    _(dump)  \
    _(dup)   \
    _(drop)  \
    _(swap)  \
    _(over)  \
    _(nip)   \
    _(rot)   \
    _(pick)  \
    _(del)   \
    _(roll)

#define defop(op) void fpn_op_ ## op(struct fpn*);
FORALL_OPS(defop)

char* fpn_op_const(struct fpn *fpn, char *code);

#endif
