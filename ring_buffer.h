#pragma once

#define NULLPTR 0

typedef struct {
    uint8_t *buffer;        // Dynamically allocated buffer
    uint64_t capacity;      // Total capacity of the buffer
    uint64_t head;          // Points to the position of the next write
    uint64_t tail;          // Points to the position of the next read
    uint64_t current_size;  // Current size of data in the buffer
} RingBuffer;

RingBuffer *
newRingBuffer(uint64_t capacity);

uint64_t
increase_index_by_size(uint64_t index, uint64_t size, uint64_t capacity);

bool
push(RingBuffer *rb, uint8_t *data, uint32_t size);

bool
is_empty(RingBuffer *rb);

bool
pop(RingBuffer *rb, uint8_t *data_buffer, uint32_t size);

void
deleteRingBuffer(RingBuffer *rb);
