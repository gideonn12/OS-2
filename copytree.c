// 329924567
#include "copytree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <linux/limits.h>
#include <errno.h>

void copy_file(const char *src, const char *dest, int copy_symlinks, int copy_permissions)
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
    struct stat st;
    if (copy_permissions == 1)
    {
        if (stat(src, &st) == 0)
        {
            if (chmod(dest, st.st_mode) != 0)
            {
                perror("chmod failed");
            }
        }
    }
    // copy symlinks
    if (copy_symlinks == 1)
    {
        char buf[PATH_MAX];
        ssize_t len = readlink(src, buf, sizeof(buf));
        if (len != -1)
        {
            buf[len] = '\0';
            if (symlink(buf, dest) != 0)
            {
                perror("symlink failed");
            }
        }
        else{
        perror("readlink failed");
        }
    }
}
void copy_directory(const char *src, const char *dest, int copy_symlinks, int copy_permissions)
{
    char src_path[PATH_MAX];
    char dest_path[PATH_MAX];
    DIR *dir = opendir(src);
    struct dirent *entry;
    struct stat st;
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
         // Construct full source path
        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        // Construct full destination path
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, entry->d_name);
        if (stat(src_path, &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                copy_directory(src_path, dest_path, copy_symlinks, copy_permissions);
            }
            else if (S_ISREG(st.st_mode))
            {
                copy_file(src_path, dest_path, copy_symlinks, copy_permissions);
            }
        }
    }
    closedir(dir);
}
void create_dir(const char *dest)
{
    if (mkdir(dest, 0777) != 0)
    {
        if(errno != EEXIST){
            return;
        }
        else{
        perror("mkdir failed");
        }
    }
}