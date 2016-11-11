#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "bright.h"

#define ROOT "/sys/class/backlight/intel_backlight"
#define BRIGHTNESS (ROOT "/brightness")
#define MAX_BRIGHTNESS (ROOT "/max_brightness")

static uid_t euid, ruid;

static void do_setuid(void) {
    int status;

#ifdef _POSIX_SAVED_IDS
    status = seteuid(euid);
#else
    status = setreuid(ruid, euid);
#endif
    if (status < 0) {
        perror("setuid");
        exit(status);
    }
}


static void undo_setuid(void) {
    int status;

#ifdef _POSIX_SAVED_IDS
    status = seteuid(ruid);
#else
    status = setreuid(euid, ruid);
#endif
    if (status < 0) {
        perror("setuid");
        exit(status);
    }
}

static void init_uids() {
    if (ruid == 0) {
        ruid = getuid();
        euid = geteuid();
        undo_setuid();
    }
}

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

static int read_int(char *path) {
    FILE *f = fopen_check(path, "r");
    int value;
    check(fscanf(f, "%d", &value) == 1, "could not read number");
    fclose(f);
    return value;
}

static void write_int(char *path, int value) {
    init_uids();
    do_setuid();
    FILE *f = fopen_check(path, "w");
    undo_setuid();
    check(fprintf(f, "%d\n", value) > 0, "could not write number");
    fclose(f);
}


int current_brightness() {
    return read_int(BRIGHTNESS);
}

int max_brightness()     {
    return read_int(MAX_BRIGHTNESS);
}

void set_brightness(int level) {
    if (level < 1) {
        level = 1;
    } else {
        int max = max_brightness();
        if (level > max)
            level = max;
    }

    write_int(BRIGHTNESS, level);
}

void inc_brightness(int amount) {
    set_brightness(current_brightness() + amount);
}
