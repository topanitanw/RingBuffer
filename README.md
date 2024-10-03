# Ring Buffer Differential Testing with Queue Oracle

This project implements a **differential test** for a ring buffer in C++ using a `std::queue` as the oracle to validate correctness. The test performs random `push` and `pop` operations on both the ring buffer and the queue, comparing the outputs. If the results differ, the test prints an error message and exits.

## Description

### RingBuffer
The `RingBuffer` is implemented in C with dynamically allocated memory, supporting `push` and `pop` operations. It manages a circular buffer and handles wrap-around logic.

- **push**: Adds data to the buffer, returning `false` if there is not enough space.
- **pop**: Removes data from the buffer, returning `false` if there is not enough data to satisfy the request.

### Differential Test
The differential test compares the behavior of the `RingBuffer` to a `std::queue`, which is used as the **oracle** (reference) to ensure correctness. The test performs randomized operations:

- **Push Operation**: Generates a random string and pushes it to both the `RingBuffer` and the `queue`.
- **Pop Operation**: Pops data from both the `RingBuffer` and the `queue` and compares them. If the results are different, the test outputs an error message and terminates.

The goal is to ensure that the `RingBuffer` behaves exactly like the queue in terms of data integrity for pushes and pops.

## Files
- **main.cpp**: Contains the differential test logic and `RingBuffer` implementation.
- **RingBuffer.h**: (Optional) Could contain the declaration for the `RingBuffer` structure and associated functions.
- **README.md**: This file, describing the project.

## How to Build and Run
```bash
# build the test binary
make test
# run the stress test
./test
```
