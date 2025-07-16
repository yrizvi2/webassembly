#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "wasm_export.h"

#define STACK_SIZE (64 * 1024)
#define HEAP_SIZE (64 * 1024)

int main() {
    const char *wasm_file = "func1.wasm";

    RuntimeInitArgs init_args;
    memset(&init_args, 0, sizeof(RuntimeInitArgs));

    static char global_heap_buf[512 * 1024];
    init_args.mem_alloc_type = Alloc_With_Pool;
    init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
    init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

    wasm_runtime_full_init(&init_args);

    FILE *file = fopen(wasm_file, "rb");
    if (!file) {
        printf("Failed to open wasm file\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    uint8_t *buf = malloc(size);
    fread(buf, 1, size, file);
    fclose(file);

    wasm_module_t module = wasm_runtime_load(buf, size, NULL, 0);
    free(buf);
    if (!module) {
        printf("Failed to load module\n");
        return 1;
    }

    wasm_module_inst_t module_inst = wasm_runtime_instantiate(module, STACK_SIZE, HEAP_SIZE, NULL, 0);
    if (!module_inst) {
        printf("Failed to instantiate module\n");
        return 1;
    }

    wasm_exec_env_t exec_env = wasm_runtime_create_exec_env(module_inst, STACK_SIZE);
    if (!exec_env) {
        printf("Failed to create exec_env\n");
        return 1;
    }

    wasm_function_inst_t setup_func = wasm_runtime_lookup_function(module_inst, "setup_table");
    if (!setup_func) {
        printf("Function setup_table not found!\n");
        return 1;
    }

    bool ok = wasm_runtime_call_wasm(exec_env, setup_func, 0, NULL);
    if (!ok) {
        printf("Failed to call setup_table: %s\n", wasm_runtime_get_exception(module_inst));
        return 1;
    }

    wasm_function_inst_t addr_func = wasm_runtime_lookup_function(module_inst, "addr");
    if (!addr_func) {
        printf("Function addr not found!\n");
        return 1;
    }

    uint32_t result[1] = {0};
    ok = wasm_runtime_call_wasm(exec_env, addr_func, 0, result);
    if (!ok) {
        printf("Failed to call addr: %s\n", wasm_runtime_get_exception(module_inst));
        return 1;
    }

    uint32_t index = result[0];
    printf("Got function index: %u\n", index);

    uint32_t params[1] = {42};
    uint32_t paramCount = 1;

    ok = wasm_runtime_call_indirect(exec_env, index, paramCount, params);
    if (!ok) {
        printf("Indirect call failed: %s\n", wasm_runtime_get_exception(module_inst));
        return 1;
    }

    wasm_runtime_destroy_exec_env(exec_env);
    wasm_runtime_deinstantiate(module_inst);
    wasm_runtime_unload(module);
    wasm_runtime_destroy();

    return 0;
}
