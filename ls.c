#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define dprint(expr) printf(#expr " = %g\n", expr)
#define sprint(expr) printf(#expr " = %s\n", expr)
#define MAXLENGTH 1000

int natural_comparison(const struct dirent **a, const struct dirent **b);

int natural_comparison(const struct dirent **a, const struct dirent **b)
{
    const char *a_name, *b_name;
    a_name = (*a)->d_name;
    b_name = (*b)->d_name;
    if (*a_name == '.')
    {
        a_name++;
    }
    if (*b_name == '.')
    {
        b_name++;
    }

    return strcasecmp(b_name, a_name);
}

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
        // DIR *directory = opendir(path);
        char dir_path_copy[MAXLENGTH];
        int directory_entries_number;

        struct dirent **sorted_dir_entries;

        directory_entries_number = scandir(path, &sorted_dir_entries, NULL, natural_comparison);

        while (directory_entries_number--)
        {
            struct dirent *dir_entry;
            strcpy(dir_path_copy, path);
            strcat(dir_path_copy, "/");
            dir_entry = sorted_dir_entries[directory_entries_number];
            strcat(dir_path_copy, dir_entry->d_name);
            stat(dir_path_copy, mystat);
            printf("%-60s%ld\n", dir_path_copy, mystat->st_size);
        }
    }
    else
    {
        printf("%-20s%ld\n", argv[1], mystat->st_size);
    }
}
