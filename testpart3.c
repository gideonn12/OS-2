#include "buffered_open.h"
#include <stdio.h>
#include <string.h>

int main() {
    buffered_file_t *bf = buffered_open("example.txt", O_RDWR | O_CREAT , 0777);
    if (!bf) {
        perror("buffered_open");
        return 1;
    }

    const char *text = "Hello, World!";
    if (buffered_write(bf, text, strlen(text)) == -1) {
        perror("buffered_write");
        buffered_close(bf);
        return 1;
    }
    const char *text2 = "Test for opreappend";
    if (buffered_write(bf, text2, strlen(text2)) == -1) {
        perror("buffered_write");
        buffered_close(bf);
        return 1;
    }
    const char *text3 = "BYYEE ";
    if (buffered_write(bf, text3, strlen(text3)) == -1) {
        perror("buffered_write");
        buffered_close(bf);
        return 1;
    }

    if(buffered_close(bf) == -1){
        perror("buffered_close");
        return 1;
    }	

    return 0;
}