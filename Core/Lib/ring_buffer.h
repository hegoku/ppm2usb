#ifndef RING_BUFFER_H
#define RING_BUFFER_H

struct ring_buffer {
    unsigned char* data;
    volatile unsigned int head;
    volatile unsigned int tail;
    unsigned int size;
};

#define RING_BUFFER_DEFINE(name, len) \
	unsigned char name##_ring_buffer_data[len];	\
	struct ring_buffer name = {	\
		.data=name##_ring_buffer_data,	\
		.head=0,	\
		.tail=0,	\
		.size=len	\
	}

#define RING_BUFFER_IS_EMPTY(b) (b.head==b.tail)

int ring_buffer_get_avail_size(struct ring_buffer *buffer);
int ring_buffer_get_data_size(struct ring_buffer *buffer);
int ring_buffer_put(struct ring_buffer *buffer, unsigned char *data, int size);
int ring_buffer_pop(struct ring_buffer *buffer, unsigned char *data, int size);
void ring_buffer_clear(struct ring_buffer *buffer);

#endif