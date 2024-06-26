#include "buffered_open.h"

buffered_file_t *buffered_open(const char *pathname, int flags, ...){
    buffered_file_t *bf = (buffered_file_t *)malloc(sizeof(buffered_file_t));
    if(bf == NULL){
        perror("malloc failed");
    }
    // flags &= ~O_PREAPPEND;
    bf->fd = open(pathname, flags);
    if(bf->fd < 0){
        perror("open failed");
        free(bf);
        return NULL;
    }
    bf->read_buffer = (char *)malloc(BUFFER_SIZE);
    if(bf->read_buffer == NULL){
        perror("malloc failed");
        free(bf);
        return NULL;
    }
    bf->write_buffer = (char *)malloc(BUFFER_SIZE);
    if(bf->write_buffer == NULL){
        perror("malloc failed");
        free(bf->read_buffer);
        free(bf);
        return NULL;
    }
    bf->read_buffer_size = BUFFER_SIZE;
    bf->write_buffer_size = BUFFER_SIZE;
    bf->read_buffer_pos = 0;
    bf->write_buffer_pos = 0;
    bf->flags = flags;
    bf->preappend = (flags & O_PREAPPEND) == O_PREAPPEND;
}
