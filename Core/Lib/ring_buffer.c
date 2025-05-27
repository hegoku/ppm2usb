#include <math.h>
#include <string.h>
#include "ring_buffer.h"

int ring_buffer_get_avail_size(struct ring_buffer *buffer)
{
    if (buffer->head==buffer->tail) { //empty
        return buffer->size - 1;
    } else if (buffer->head<buffer->tail) {
        return buffer->tail - buffer->head - 1;
    } else {
        if (buffer->head+1 == buffer->size && buffer->tail==0) {
            return 0;
        } else {
            return buffer->size - buffer->head + buffer->tail - 1;
        }
    }
}

int ring_buffer_get_data_size(struct ring_buffer *buffer)
{
    if (buffer->tail==buffer->head) { //empty
        return 0;
    } else if (buffer->head>buffer->tail) {
        return buffer->head - buffer->tail;
    } else {
        if (buffer->tail+1 == buffer->size && buffer->head==0) {
            return 1;
        } else {
            return buffer->size - buffer->tail + buffer->head;
        }
    }
} 

int ring_buffer_put(struct ring_buffer *buffer, unsigned char *data, int size)
{
    int i = 0;
    int s = ring_buffer_get_avail_size(buffer);
    size = s>size?size:s;
    for (i=0;i<size;i++) {
        buffer->data[buffer->head] = data[i];
        buffer->head++;
        if (buffer->head == buffer->size) {
            buffer->head = 0;
        }
    }
    
    return i;
}

int ring_buffer_pop(struct ring_buffer *buffer, unsigned char *data, int size)
{
    int i = 0;
    int s = ring_buffer_get_data_size(buffer);
    size = s>size?size:s;
    for (i=0;i<size;i++) {
        data[i] = buffer->data[buffer->tail];
        buffer->tail++;
        if (buffer->tail == buffer->size) {
            buffer->tail = 0;
        }
    }
    
    return i;
}

void ring_buffer_clear(struct ring_buffer *buffer)
{
	buffer->head = 0;
	buffer->tail = 0;
}