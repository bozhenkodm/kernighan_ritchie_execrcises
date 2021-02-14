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
#include <pwd.h>
#include <grp.h>

#define dprint(expr) printf(#expr " = %g\n", expr)
#define sprint(expr) printf(#expr " = %s\n", expr)
#define MAXLENGTH 1000

int natural_comparison(const struct dirent **a, const struct dirent **b);
// void print_permission_string(unsigned long st_mode);
char get_file_mode(mode_t st_mode);
static char *ls_permissions_string(mode_t mode);

struct ls_entry
{
    char file_type;
    char *permissions;
    nlink_t hardlinks_count;
    uid_t uid;
    gid_t gid;
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

char get_file_mode(mode_t st_mode)
{
    switch (st_mode & S_IFMT)
    {
    case S_IFSOCK:
        return 's';
        break;
    case S_IFLNK:
        return 'l';
        break;
    case S_IFREG:
        return '-';
        break;
    case S_IFBLK:
        return 'b';
        break;
    case S_IFDIR:
        return 'd';
        break;
    case S_IFCHR:
        return 'c';
        break;
    case S_IFIFO:
        return 'p';
        break;
    default:
        return '?';
        break;
    }
    // printf((st_mode & S_IRUSR) ? "r" : "-");
    // printf((st_mode & S_IWUSR) ? "w" : "-");
    // printf((st_mode & S_IXUSR) ? "x" : "-");
    // printf((st_mode & S_IRGRP) ? "r" : "-");
    // printf((st_mode & S_IWGRP) ? "w" : "-");
    // printf((st_mode & S_IXGRP) ? "x" : "-");
    // printf((st_mode & S_IROTH) ? "r" : "-");
    // printf((st_mode & S_IWOTH) ? "w" : "-");
    // printf((st_mode & S_IXOTH) ? "x" : "-");
    // printf(" ");
}

// https://stackoverflow.com/a/10323131
static char *ls_permissions_string(mode_t st_mode)
{
    static const char *rwx[] = {"---", "--x", "-w-", "-wx",
                                "r--", "r-x", "rw-", "rwx"};
    static char human_readable_permissions[10];

    strcpy(&human_readable_permissions[0], rwx[(st_mode >> 6) & 7]);
    strcpy(&human_readable_permissions[3], rwx[(st_mode >> 3) & 7]);
    strcpy(&human_readable_permissions[6], rwx[(st_mode & 7)]);
    if (st_mode & S_ISUID)
        human_readable_permissions[2] = (st_mode & S_IXUSR) ? 's' : 'S';
    if (st_mode & S_ISGID)
        human_readable_permissions[5] = (st_mode & S_IXGRP) ? 's' : 'S';
    if (st_mode & S_ISVTX)
        human_readable_permissions[8] = (st_mode & S_IXOTH) ? 't' : 'T';
    human_readable_permissions[9] = '\0';
    return (human_readable_permissions);
}

void print_ls_entry(struct ls_entry lse)
{
    time_t time = lse.mod_time.tv_sec;
    struct tm *broken_down_time = gmtime(&time);
    char human_readable_time[20]; // TODO figure out how to initialize variable
    char *user = getpwuid(lse.uid)->pw_name;
    char *group = getgrgid(lse.gid)->gr_name;
    strftime(human_readable_time, 20, "%F %R", broken_down_time);
    printf(
        "%c%s %lu %s %s %ld %s %s\n",
        lse.file_type,
        lse.permissions,
        lse.hardlinks_count,
        user,
        group,
        lse.size,
        human_readable_time,
        lse.file_name);
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

            struct ls_entry lse = {
                .file_type = get_file_mode(os_entry_stat->st_mode),
                .permissions = ls_permissions_string(os_entry_stat->st_mode),
                .hardlinks_count = os_entry_stat->st_nlink,
                .uid = os_entry_stat->st_uid,
                .gid = os_entry_stat->st_gid,
                .size = os_entry_stat->st_size,
                .mod_time = os_entry_stat->st_mtim,
                .file_name = dir_entry->d_name};
            print_ls_entry(lse);
        }
    }
    else
    {
        struct ls_entry lse = {
            .file_type = get_file_mode(os_entry_stat->st_mode),
            .permissions = ls_permissions_string(os_entry_stat->st_mode),
            .hardlinks_count = os_entry_stat->st_nlink,
            .uid = os_entry_stat->st_uid,
            .gid = os_entry_stat->st_gid,
            .size = os_entry_stat->st_size,
            .mod_time = os_entry_stat->st_mtim,
            .file_name = (char *)path};
        print_ls_entry(lse);
    }
}
