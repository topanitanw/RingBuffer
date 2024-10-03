#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <queue>

#include "ring_buffer.h"

// Helper function to generate a random string
std::string
generate_random_string(int length) {
    std::string characters =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string random_string;
    for (int i = 0; i < length; i++) {
        random_string += characters[rand() % characters.length()];
    }
    return random_string;
}

// Differential test function
void
differential_test(RingBuffer *rb, int num_operations, int max_string_length) {
    std::queue<std::string> oracle;  // Queue as the oracle
    srand(time(0));                  // Seed the random number generator

    for (int i = 0; i < num_operations; i++) {
        // Randomly decide between push or pop
        bool is_push = rand() % 2;
		if(i == 0) {
			is_push = true;
		}

        if (is_push) {
            // Generate a random string
            std::string random_string =
                generate_random_string(rand() % max_string_length + 1);
            uint8_t *data = (uint8_t *) random_string.c_str();
            uint32_t size = random_string.length();

            // Push to the ring buffer and queue
            if (push(rb, data, size)) {
                oracle.push(random_string);
                std::cout << "Pushed: " << random_string << " size: " << size << "\n";
            } else {
                std::cout
                    << "Push failed (not enough space in the ring buffer).\n";
            }
        } else {
            // Pop data if available
            if (oracle.empty()) {
                std::cout << "Oracle queue is empty. ";
                if (!is_empty(rb)) {
                    std::cout
                        << "Ring buffer is not empty. This is unexpected.\n";
                    exit(1);
                }
				std::cout << "Ring buffer is also empty\n";
				continue;
            }

            std::string expected = oracle.front();
            oracle.pop();
            uint32_t size = expected.length();
            uint8_t *data_buffer =
                new uint8_t[size + 1];  // Allocate for the string to be
                                        // popped

            if (pop(rb, data_buffer, size)) {
                data_buffer[size] =
                    '\0';  // Null-terminate for string comparison
                std::string actual((char *) data_buffer);
                std::cout << "Popped: expect: " << expected << " size: " << size << " ";

                if (expected != actual) {
                    std::cout << "Mismatch detected! Expected: " << expected
                              << ", Actual: " << actual << "\n";
                    delete[] data_buffer;
                    exit(1);  // Exit on failure
                }

                std::cout << "Matched: " << actual << "\n";
            } else {
                std::cout
                    << "Pop failed (not enough data in the ring buffer).\n";
            }

            delete[] data_buffer;
        }
    }

    while (!oracle.empty()) {
        std::string expected = oracle.front();
        oracle.pop();
        uint32_t size = expected.length();
        // Allocate for the string to be popped
        uint8_t *data_buffer = new uint8_t[size + 1];

        if (pop(rb, data_buffer, size)) {
            data_buffer[size] = '\0';
            std::string actual((char *) data_buffer);
            std::cout << "Popped: expect: " << expected << " ";

            if (expected != actual) {
                std::cout << "Mismatch detected! Expected: " << expected
                          << ", Actual: " << actual << "\n";
                delete[] data_buffer;
                exit(1);  // Exit on failure
            }

            std::cout << "Matched: " << actual << "\n";
        } else {
            std::cout << "Pop failed (not enough data in the ring buffer).\n";
        }
        delete[] data_buffer;
    }

    std::cout << num_operations << " Differential test passed.\n";
}

int
main() {
    uint64_t ring_buffer_capacity = 1024;
    int num_operations = 1000;
    int max_string_length = 50;

    RingBuffer *rb = newRingBuffer(ring_buffer_capacity);
    if (!rb) {
        std::cerr << "Failed to create the ring buffer.\n";
        return -1;
    }

    differential_test(rb, num_operations, max_string_length);

    deleteRingBuffer(rb);
    return 0;
}
