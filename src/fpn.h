#ifndef __FPN_H__
#define __FPN_H__

#include <stdlib.h>

#include <gmp.h>
#include <mpfr.h>

// I'm very sorry
#define BIND(val, x) do { \
    mpfr_t x; x[0] = *(__mpfr_struct*)(val).data; \
    if (val.type == RATIONAL) { mpq_t x; x[0] = *(__mpq_struct*)(val).data;
#define OR }
#define UNBIND } while (0);

#define RATIONAL 1
#define FLOAT 2
struct val {
    void *data;
    char type;
};

#define MAX_EXTRA 30
struct fpn {
    struct val *stack;
    int stackSize;
    int bufSize;
};

struct fpn *fpn_init();
void fpn_destroy(struct fpn *fpn);

#endif
