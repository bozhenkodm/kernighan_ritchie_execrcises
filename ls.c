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
#include <time.h>

#define dprint(expr) printf(#expr " = %g\n", expr)
#define sprint(expr) printf(#expr " = %s\n", expr)
#define MAXLENGTH 1000

int natural_comparison(const struct dirent **a, const struct dirent **b);
void print_permission_string(unsigned long st_mode);

struct ls_entry
{
    char file_type;
    char *permissions;
    nlink_t hardlinks_count;
    char *user;
    char *group;
    off_t size;
    struct timespec mod_time;
    char *file_name;
};
void print_ls_entry(struct ls_entry lse);

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

void print_ls_entry(struct ls_entry lse)
{
    time_t time = lse.mod_time.tv_sec;
    struct tm *broken_down_time = gmtime(&time);
    char human_readable_time[20]; // TODO figure out how to initialize variable
    strftime(human_readable_time, 20, "%F %R", broken_down_time);
    printf(
        "%c%s %lu %s %s %ld %s %s\n",
        lse.file_type,
        lse.permissions,
        lse.hardlinks_count,
        lse.user,
        lse.group,
        lse.size,
        human_readable_time,
        lse.file_name);
}

int main(int argc, char **argv)
{
    (void)argc;
    struct stat *os_entry_stat = malloc(sizeof *os_entry_stat);
    const char *path = argv[1];

    // drwxrwxr-x 8 bozhenkodm bozhenkodm  4096 фев 10 20:44 .git/

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
