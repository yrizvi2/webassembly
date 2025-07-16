#include <stdint.h>
#include <stdio.h>

#define WASM_EXPORT(name) __attribute__((used)) __attribute__((export_name(name)))

// Declare the external funcref table
extern void *__funcref_table __attribute__((import_module("env")))
                                __attribute__((import_name("table")));

typedef void (*TFunc)(uint32_t);

void print_num(uint32_t x) {
    printf("you gave me = %d\n", x);
}

// Exported function so host or other Wasm modules can call
WASM_EXPORT("print_num")
void print_num_export(uint32_t x) {
    print_num(x);
}

// Set up the table with our function pointer at index 0
WASM_EXPORT("setup_table")
void setup_table() {
    if (__builtin_wasm_table_size(__funcref_table) == 0) {
        __builtin_wasm_table_grow(__funcref_table, (void *)print_num_export, 1);
    } else {
        __builtin_wasm_table_set(__funcref_table, 0, (void *)print_num_export);
    }
}

// Export a way to tell the host what table index to use
WASM_EXPORT("addr")
uint32_t addr() {
    return 0;
}
