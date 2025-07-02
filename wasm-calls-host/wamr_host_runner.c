#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wasm_export.h"

void print_custom(wasm_exec_env_t exec_env, int code) {
    printf("Host received code: %d\n", code);
}

int main() {
    RuntimeInitArgs init_args;
    static char global_heap_buf[512 * 1024];

    memset(&init_args, 0, sizeof(RuntimeInitArgs));

    NativeSymbol native_symbols[] = {
        { "print_custom", print_custom, "(i)", NULL }
    };
    init_args.mem_alloc_type = Alloc_With_Pool;
    init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
    init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);
    init_args.n_native_symbols = sizeof(native_symbols) / sizeof(NativeSymbol);
    init_args.native_module_name = "host";
    init_args.native_symbols = native_symbols;

    if (!wasm_runtime_full_init(&init_args)) {
        printf("Runtime init failed.\n");
        return -1;
    }

    // Load Wasm binary
    FILE *file = fopen("custom_message.wasm", "rb");
    if (!file) {
        printf("Failed to open wasm file.\n");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    uint8_t *wasm_buf = malloc(size);
    fread(wasm_buf, 1, size, file);
    fclose(file);

    char error_buf[128];
    wasm_module_t module = wasm_runtime_load(wasm_buf, size, error_buf, sizeof(error_buf));
    if (!module) {
        printf("Load failed: %s\n", error_buf);
        return -1;
    }

    wasm_module_inst_t module_inst = wasm_runtime_instantiate(module, 64 * 1024, 64 * 1024, error_buf, sizeof(error_buf));
    if (!module_inst) {
        printf("Instantiate failed: %s\n", error_buf);
        return -1;
    }

    wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, "trigger_custom");
    if (!func) {
        printf("trigger_custom not found.\n");
        return -1;
    }

    wasm_exec_env_t exec_env = wasm_runtime_create_exec_env(module_inst, 64 * 1024);
    if (!wasm_runtime_call_wasm(exec_env, func, 0, NULL)) {
        printf("Call failed.\n");
    }

    wasm_runtime_destroy_exec_env(exec_env);
    wasm_runtime_deinstantiate(module_inst);
    wasm_runtime_unload(module);
    wasm_runtime_destroy();
    free(wasm_buf);

    return 0;
}

