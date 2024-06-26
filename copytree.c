// 329924567
#include "copytree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>
#include <dirent.h>

void copy_file(const char *src, const char *dest, int copy_symlinks, int copy_permissions)
{
    struct stat st;
    int res = lstat(src, &st);
    if (res < 0)
    {
        perror("lstat failed");
        return;
    }
    // copy symlinks
    if (S_ISLNK(st.st_mode) && copy_symlinks)
    {
        char buf[PATH_MAX];
        ssize_t len = readlink(src, buf, sizeof(buf) - 1);
        if (len < 0)
        {
            perror("readlink failed");
            return;
        }
        else
        {
            buf[len] = '\0';
            if (symlink(buf, dest) != 0)
            {
                perror("symlink failed");
            }
            return;
        }
    }
    else if(S_ISLNK(st.st_mode) && copy_symlinks == 0){
        return;
    }
    else
    {
        FILE *f;
        f = fopen(src, "r");
        if (f == NULL)
        {
            perror("fopen failed");
            return;
        }
        FILE *f2;
        f2 = fopen(dest, "w");
        if (f2 == NULL)
        {
            perror("fopen failed");
            return;
        }
        int ch;
        while ((ch = fgetc(f)) != EOF)
        {
            fputc(ch, f2);
        }
        fclose(f);
        fclose(f2);
        // copy permissions
        if (copy_permissions == 1)
        {
            if (chmod(dest, st.st_mode) != 0)
            {
                perror("chmod failed");
            }
        }
    }
}
void copy_directory(const char *src, const char *dest, int copy_symlinks, int copy_permissions)
{
    char src_path[PATH_MAX];
    char dest_path[PATH_MAX];
    DIR *dir = opendir(src);
    struct dirent *entry;
    if (dir == NULL)
    {
        perror("opendir failed");
        return;
    }
    create_dir(dest);

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, entry->d_name);
        struct stat st;
        if (lstat(src_path, &st) < 0)
        {
            perror("lstat failed");
            return;
        }
        if (S_ISDIR(st.st_mode))
        {
            copy_directory(src_path, dest_path, copy_symlinks, copy_permissions);
        }
        else
        {
            copy_file(src_path, dest_path, copy_symlinks, copy_permissions);
        }
    }
    closedir(dir);
}
void create_dir(const char *dest)
{
    if (mkdir(dest, 0777) != 0)
    {
        if (errno != EEXIST)
        {
            perror("mkdir failed");
        }
    }
}