#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "setuid.c"

#define ROOT "/sys/class/backlight/intel_backlight"
#define BRIGHTNESS (ROOT "/brightness")
#define MAX_BRIGHTNESS (ROOT "/max_brightness")

static FILE *fopen_check(char *path, char *mode) {
    FILE *f = fopen(path, mode);

    if (f == NULL) {
        perror(path);
        exit(1);
    }

    return f;
}

static void check(int condition, char *message) {
    if (!condition) {
        if (errno)
            perror(message);
        else
            fprintf(stderr, "error: %s\n", message);
    }
}

static long read_long(char *path) {
    FILE *f = fopen_check(path, "r");
    long value;
    check(fscanf(f, "%ld", &value) == 1, "could not read number");
    fclose(f);
    return value;
}

static void write_long(char *path, long value) {
    do_setuid();
    FILE *f = fopen_check(path, "w");
    undo_setuid();
    check(fprintf(f, "%ld\n", value) > 0, "could not write number");
    fclose(f);
}

static inline long current_brightness() { return read_long(BRIGHTNESS); }
static inline long max_brightness()     { return read_long(MAX_BRIGHTNESS); }

__attribute__((noreturn))
static void usage(char *progname, int status) {
    printf("Usage: %s [+-]AMOUNT\n", progname);
    exit(status);
}

static void set_brightness(long level) {
    if (level < 1) {
        level = 1;
    } else {
        long max = max_brightness();
        if (level > max)
            level = max;
    }

    write_long(BRIGHTNESS, level);
}

static void inc_brightness(long amount) {
    set_brightness(current_brightness() + amount);
}

int main(int argc, char **argv) {
    if (argc < 2)
        usage(argv[0], 1);

    init_uids();

    if (argv[1][0] == '+') {
        inc_brightness(atol(&argv[1][1]));
    } else if (argv[1][0] == '-') {
        inc_brightness(-atol(&argv[1][1]));
    } else {
        set_brightness(atol(argv[1]));
    }

    return 0;
}
