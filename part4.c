#include "copytree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s [-l] [-p] <source_directory> <destination_directory>", prog_name);
    fprintf(stderr, "  -l: Copy symbolic links as links");
    fprintf(stderr, "  -p: Copy file permissions");
}

int main(int argc, char *argv[]) {
    int opt = 1;
    int copy_symlinks = 0;
    int copy_permissions = 0;
    if(argc < 3 ) {
        print_usage(argv[0]);
        return 1;
    }
    if(argc == 3 && (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "-p") == 0)) {
        print_usage(argv[0]);
        return 1;
    }
    if(strcmp(argv[1], "-l") == 0 || strcmp(argv[2], "-l") == 0){
        copy_symlinks = 1;
        opt++;
    }
    if(strcmp(argv[1], "-p") == 0 || strcmp(argv[2], "-p") == 0){
        copy_permissions = 1;
        opt++;
    }
    const char *src_dir = argv[opt];
    const char *dest_dir = argv[opt + 1];
    copy_directory(src_dir, dest_dir, copy_symlinks, copy_permissions);
    return 0;
}