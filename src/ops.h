#ifndef __OPS_H__
#define __OPS_H__

#include "fpn.h"

void fpn_op_add(struct fpn *fpn);
void fpn_op_print(struct fpn *fpn);
char* fpn_op_const(struct fpn *fpn, char *code);

#endif
