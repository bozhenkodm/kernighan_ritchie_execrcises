#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
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
    if (S_ISDIR(mystat->st_mode))
    {
        DIR *directory = opendir(path);
        char *filename, *dir_copy;

        struct dirent *dir_entry;
        while ((dir_entry = readdir(directory)) != NULL)
        {
            filename = dir_entry->d_name;
            filename = strcat(path, filename);
            stat(filename, mystat);
            printf("%-60s%ld\n", filename, mystat->st_size);
        }
    }
    else
    {
        printf("%-20s%ld\n", argv[1], mystat->st_size);
    }
}
