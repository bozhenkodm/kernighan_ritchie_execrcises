#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define dprint(expr) printf(#expr " = %g\n", expr)
#define sprint(expr) printf(#expr " = %s\n", expr)

int main(int argc, char **argv)
{
    (void)argc;
    struct stat *mystat = malloc(sizeof *mystat);
    const char *path = argv[1];

    stat(path, mystat);
    if (errno)
    {
        printf("errno = %s\n", strerror(errno));
        return 1;
    }
    S_ISDIR(mystat->st_mode);
    printf("%-20s%ld\n", argv[1], mystat->st_size);
}
