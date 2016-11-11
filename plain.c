#include <stdio.h>
#include <stdlib.h>
#include "bright.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s [+-]AMOUNT\n", argv[0]);
        return 1;
    }

    if (argv[1][0] == '+') {
        inc_brightness(atoi(&argv[1][1]));
    } else if (argv[1][0] == '-') {
        inc_brightness(-atoi(&argv[1][1]));
    } else {
        set_brightness(atoi(argv[1]));
    }

    return 0;
}
