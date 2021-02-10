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
void print_permission_string(unsigned long st_mode);

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

void print_permission_string(unsigned long st_mode)
{
    switch (st_mode & S_IFMT)
    {
    case S_IFSOCK:
        printf("s");
        break;
    case S_IFLNK:
        printf("l");
        break;
    case S_IFREG:
        printf("-");
        break;
    case S_IFBLK:
        printf("b");
        break;
    case S_IFDIR:
        printf("d");
        break;
    case S_IFCHR:
        printf("c");
        break;
    case S_IFIFO:
        printf("p");
        break;

    default:
        printf("hz");
        break;
    }
    printf((st_mode & S_IRUSR) ? "r" : "-");
    printf((st_mode & S_IWUSR) ? "w" : "-");
    printf((st_mode & S_IXUSR) ? "x" : "-");
    printf((st_mode & S_IRGRP) ? "r" : "-");
    printf((st_mode & S_IWGRP) ? "w" : "-");
    printf((st_mode & S_IXGRP) ? "x" : "-");
    printf((st_mode & S_IROTH) ? "r" : "-");
    printf((st_mode & S_IWOTH) ? "w" : "-");
    printf((st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");
}

int main(int argc, char **argv)
{
    (void)argc;
    struct stat *os_entry_stat = malloc(sizeof *os_entry_stat);
    const char *path = argv[1];

    stat(path, os_entry_stat);
    if (errno)
    {
        printf("errno = %s\n", strerror(errno));
        return 1;
    }
    if (S_ISDIR(os_entry_stat->st_mode))
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
            stat(dir_path_copy, os_entry_stat);
            print_permission_string(os_entry_stat->st_mode);
            printf("%-60s%ld\n", dir_path_copy, os_entry_stat->st_size);
        }
    }
    else
    {
        print_permission_string(os_entry_stat->st_mode);
        printf("%-60s%ld\n", argv[1], os_entry_stat->st_size);
    }
}
