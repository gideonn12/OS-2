#include "buffered_open.h"

buffered_file_t *buffered_open(const char *pathname, int flags, ...)
{
    buffered_file_t *bf = (buffered_file_t *)malloc(sizeof(buffered_file_t));
    if (bf == NULL)
    {
        perror("malloc failed");
        return NULL;
    }
    // flags &= ~O_PREAPPEND;
    bf->fd = open(pathname, flags);
    if (bf->fd < 0)
    {
        perror("open failed");
        free(bf);
        return NULL;
    }
    bf->read_buffer = (char *)malloc(BUFFER_SIZE);
    if (bf->read_buffer == NULL)
    {
        perror("malloc failed");
        free(bf);
        return NULL;
    }
    bf->write_buffer = (char *)malloc(BUFFER_SIZE);
    if (bf->write_buffer == NULL)
    {
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
int buffered_flush(buffered_file_t *bf)
{
    ssize_t total_bytes_written = 0;
    ssize_t bytes_written = 0;
    while (total_bytes_written < bf->write_buffer_pos)
    {
        bytes_written = write(bf->fd, bf->write_buffer + total_bytes_written, bf->write_buffer_pos - total_bytes_written);
        if (bytes_written < 0)
        {
            perror("write failed");
            return -1;
        }
        total_bytes_written += bytes_written;
    }
    bf->write_buffer_pos = 0;
    return 0;
}
ssize_t buffered_write(buffered_file_t *bf, const void *buf, size_t count)
{
    if (count + bf->write_buffer_pos > bf->write_buffer_size)
    {
        if (buffered_flush(bf) < 0)
        {
            perror("flush failed");
            return -1;
        }
    }
    while (count > 0)
    {
        if (bf->write_buffer_pos == bf->write_buffer_size)
        {
            if (buffered_flush(bf) < 0)
            {
                perror("flush failed");
                return -1;
            }
        }
        size_t bytes_to_copy = count;
        if (bytes_to_copy + bf->write_buffer_pos > bf->write_buffer_size)
        {
            bytes_to_copy = bf->write_buffer_size - bf->write_buffer_pos;
        }
        memcpy(bf->write_buffer + bf->write_buffer_pos, buf, bytes_to_copy);
        bf->write_buffer_pos += bytes_to_copy;
        count -= bytes_to_copy;
        buf += bytes_to_copy;
    }
    return count;
}
ssize_t buffered_read(buffered_file_t *bf, void *buf, size_t count)
{
    if (buffered_flush(bf) < 0)
    {
        perror("flush failed");
        return -1;
    }
    while (count > 0)
    {
        if( bf->read_buffer_pos >= bf->read_buffer_size){
            ssize_t res = read(bf->fd, bf->read_buffer,bf->read_buffer_size);
            if(res < 0){
                perror("read failed");
                return -1;
            }
        }
        bf->read_buffer_pos = 0;
        size_t bytes_to_copy = count;
        if (bytes_to_copy > bf-> read_buffer_size - bf->read_buffer_pos)
        {
            bytes_to_copy = bf->read_buffer_size - bf->read_buffer_pos;
        }
        memcpy(buf, bf->read_buffer + bf->read_buffer_pos, bytes_to_copy);
        bf->read_buffer_pos += bytes_to_copy;   
        count -= bytes_to_copy;
        buf += bytes_to_copy;
    }
    return count;
}

int buffered_close(buffered_file_t *bf)
{
    if (buffered_flush(bf) < 0)
    {
        perror("flush failed");
        return -1;
    }
    int res = close(bf->fd);
    if (res < 0)
    {
        perror("close failed");
        return -1;
    }
    free(bf->read_buffer);
    free(bf->write_buffer);
    free(bf);
    return 0;
}