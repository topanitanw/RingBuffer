#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULLPTR 0

typedef struct {
    uint8_t *buffer;        // Dynamically allocated buffer
    uint64_t capacity;      // Total capacity of the buffer
    uint64_t head;          // Points to the position of the next write
    uint64_t tail;          // Points to the position of the next read
    uint64_t current_size;  // Current size of data in the buffer
} RingBuffer;

// Initialize and allocate the ring buffer
RingBuffer *
newRingBuffer(uint64_t capacity) {
    RingBuffer *rb = (RingBuffer *) malloc(sizeof(RingBuffer));
    if (!rb) {
        // Memory allocation failed, return false.
        return rb;
    }

    rb->buffer = (uint8_t *) malloc(capacity * sizeof(uint8_t));
    if (!rb->buffer) {
        // Memory allocation failed, return false.
        return NULL;
    }

    rb->capacity = capacity;
    rb->head = 0;
    rb->tail = 0;
    rb->current_size = 0;
    return rb;
}

// handle wrap-around using modulo operation
uint64_t
increase_index_by_size(uint64_t index, uint64_t size, uint64_t capacity) {
    return (index + size) % capacity;
}

// Push data to the ring buffer
bool
push(RingBuffer *rb, uint8_t *data, uint32_t size) {
    if (size > (rb->capacity - rb->current_size)) {
        // Not enough space in the buffer
        return false;
    }

    // Copy data to the buffer, handle wrap-around using modulo operation
    for (uint32_t i = 0; i < size; i++) {
        rb->buffer[rb->head] = data[i];
        rb->head = increase_index_by_size(rb->head, 1, rb->capacity);
    }

    rb->current_size += size;
    return true;
}

bool
is_empty(RingBuffer *rb) {
	printf("current_size: %llu\n", rb->current_size);
    return rb->current_size == 0;
}

// Pop data from the ring buffer
bool
pop(RingBuffer *rb, uint8_t *data_buffer, uint32_t size) {
    if (size > rb->current_size) {
        // Not enough data in the buffer
        return false;
    }

    // Copy data from the buffer, handle wrap-around using modulo operation
    for (uint32_t i = 0; i < size; i++) {
        data_buffer[i] = rb->buffer[rb->tail];
        rb->tail = increase_index_by_size(rb->tail, 1, rb->capacity);
    }

    rb->current_size -= size;
    return true;
}

// Free the dynamically allocated buffer
void
deleteRingBuffer(RingBuffer *rb) {
    if (!rb) {
        return;
    }

    free(rb->buffer);
    free(rb);
}

// int main() {
//     printf("Hello, World!\n");
//     RingBuffer* rb = newRingBuffer(8 * (1 << 10));
//     if (!rb) {
//         printf("Failed to initialize ring buffer\n");
//         return -1;
//     }
//     printf("Ring buffer initialized successfully\n");
//
//     uint8_t data[] = {1, 2, 3, 4, 5};
//     if (push(rb, data, 5)) {
//         printf("Data pushed successfully\n");
//     } else {
//         printf("Failed to push data\n");
//     }
//
//     uint8_t data_out[5];
//     if (pop(rb, data_out, 5)) {
//         printf("Data popped: ");
//         for (int i = 0; i < 5; i++) {
//             printf("%d ", data_out[i]);
//         }
//         printf("\n");
//     } else {
//         printf("Failed to pop data\n");
//     }
//
//     deleteRingBuffer(rb);
//     return 0;
// }
