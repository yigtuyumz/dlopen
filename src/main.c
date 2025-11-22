#include "dlopen.h"

int
main(void)
{
    // RTLD_NOW  -> resolve all symbols immediately
    // RTDL_LAZY -> resolve symbol when needed
    // loads libc
    void *libc_handler = dlopen(NULL, RTLD_LAZY);

    // why use stack!?
    void *libc_symbols[3];

    if (!libc_handler)
        return (EXIT_FAILURE);
    // here we can be more paranoid, but this thing aims to demontstrate the idea for now...
    libc_symbols[0] = dlsym(libc_handler, "malloc");
    libc_symbols[1] = dlsym(libc_handler, "printf");
    libc_symbols[2] = dlsym(libc_handler, "free");

    void *(*malloc)(size_t) = libc_symbols[0];
    int (*printf)(const char *restrict, ...) = libc_symbols[1];
    void (*free)(void *) = libc_symbols[2];

    printf("Hello, World!\n%s\n", dlerror());
    void *a = malloc(3);

    free(a);
    return (EXIT_SUCCESS);
}
