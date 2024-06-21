// 329924567
#include <copytree.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
int PATH_MAX = 4096;
void copy_file(const char *src, const char *dest, int copy_symlinks, int copy_permissions){
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
    while(ch = fgetc(f) != EOF){
        fputc(ch, f2);
    }
    fclose(f);
    fclose(f2);
    // copy permissions
    struct stat st;
    if(copy_permissions == 1){
        if(stat(src, &st) == 0){
            if(chmod(dest, st.st_mode) != 0){
                perror("chmod failed");
            }
        }
    }
    // copy symlinks
    if(copy_symlinks == 1){
        char buf[PATH_MAX];
        ssize_t len = readlink(src, buf, sizeof(buf));
        if(len != -1){
            buf[len] = '\0';
            if(symlink(buf, dest) != 0){
                perror("symlink failed");
            }
        }
        perror("readlink failed");
    }
}
void copy_directory(const char *src, const char *dest, int copy_symlinks, int copy_permissions){

}
void create_dir(){}