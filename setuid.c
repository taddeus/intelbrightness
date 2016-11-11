#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

static uid_t euid, ruid;

void do_setuid(void) {
    int status;

#ifdef _POSIX_SAVED_IDS
    status = seteuid(euid);
#else
    status = setreuid(ruid, euid);
#endif
    if (status < 0) {
        fprintf(stderr, "Couldn't set uid.\n");
        exit(status);
    }
}


/* Set the effective UID to the real UID. */
void undo_setuid(void) {
  int status;

#ifdef _POSIX_SAVED_IDS
    status = seteuid(ruid);
#else
    status = setreuid(euid, ruid);
#endif
    if (status < 0) {
        fprintf(stderr, "Couldn't set uid.\n");
        exit(status);
    }
}

/* Remember the real and effective user IDs.  */
void init_uids() {
    ruid = getuid();
    euid = geteuid();
    undo_setuid();
}
