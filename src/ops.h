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

void fpn_op_add(struct fpn *fpn);
void fpn_op_sub(struct fpn *fpn);
void fpn_op_mul(struct fpn *fpn);
void fpn_op_div(struct fpn *fpn);

void fpn_op_print(struct fpn *fpn);
void fpn_op_dump(struct fpn *fpn);

void fpn_op_dup(struct fpn *fpn);
void fpn_op_drop(struct fpn *fpn);
void fpn_op_swap(struct fpn *fpn);

void fpn_op_over(struct fpn *fpn);
void fpn_op_nip(struct fpn *fpn);
void fpn_op_rot(struct fpn *fpn);

void fpn_op_pick(struct fpn *fpn);
void fpn_op_del(struct fpn *fpn);
void fpn_op_roll(struct fpn *fpn);

char* fpn_op_const(struct fpn *fpn, char *code);

#endif
