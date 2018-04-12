#include "fpn.h"

struct fpn *fpn_init() {
    struct fpn *fpn = malloc(sizeof *fpn);
    fpn->stack = malloc(MAX_EXTRA * sizeof *fpn->stack);
    fpn->stackSize = MAX_EXTRA;
    fpn->bufSize = 0;
    return fpn;
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
