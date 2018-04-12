#include <stdio.h>

#include "fpn.h"

int main(int argc, char* argv[]) {
    struct fpn *fpn = fpn_init();
    fpn_destroy(fpn);
    return 0;
}
