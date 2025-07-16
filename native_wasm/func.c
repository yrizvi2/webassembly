#include <stdint.h>
#include <stdio.h>

#define WASM_EXPORT(name) __attribute__((used)) __attribute__((export_name(name)))

typedef void (*TFunc)(uint32_t);

void print_num(uint32_t x) {
    printf("you gave me = %d\n", x);
}

// Exported wrapper function
WASM_EXPORT("print_num")
void print_num_export(uint32_t x) {
    print_num(x);
}

// Function to populate the table at index 0
WASM_EXPORT("setup_table")
void setup_table() {
    if (__builtin_wasm_table_size(0) == 0) {
        __builtin_wasm_table_grow(0, (void *)print_num_export, 1);
    } else {
        __builtin_wasm_table_set(0, 0, (void *)print_num_export);
    }
}

// Return the index in the table where the function is stored
WASM_EXPORT("addr")
uint32_t addr() {
    return 0;
}
