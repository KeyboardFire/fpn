#include <stdlib.h>

#include "util.h"

void fpn_clear(struct val val) {
    BIND(val, data) { mpq_clear(data); } OR { mpfr_clear(data); }
    free(val.data);
}
