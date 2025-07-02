#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wasm_export.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <wasm file>\n", argv[0]);
        return 1;
    }

    RuntimeInitArgs init_args;
    memset(&init_args, 0, sizeof(RuntimeInitArgs));
    wasm_runtime_init();

    const char *wasm_file = argv[1];
    FILE *fp = fopen(wasm_file, "rb");
    if (!fp) {
        printf("Failed to open WASM file: %s\n", wasm_file);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    uint8_t *wasm_buf = malloc(size);
    fread(wasm_buf, 1, size, fp);
    fclose(fp);

    char error_buf[128];
    wasm_module_t module = wasm_runtime_load(wasm_buf, size, error_buf, sizeof(error_buf));
    if (!module) {
        printf("Failed to load module: %s\n", error_buf);
        return 1;
    }

    wasm_module_inst_t module_inst =
        wasm_runtime_instantiate(module, 64 * 1024, 64 * 1024, error_buf, sizeof(error_buf));
    if (!module_inst) {
        printf("Failed to instantiate module: %s\n", error_buf);
        return 1;
    }

    wasm_exec_env_t exec_env = wasm_runtime_create_exec_env(module_inst, 64 * 1024);
    if (!exec_env) {
        printf("Failed to create exec env.\n");
        return 1;
    }

    wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, "increment");
    if (!func) {
        printf("Function 'increment' not found\n");
        return 1;
    }

    uint32_t argv_vals[1] = { 41 };
    uint32_t result = 0;

    if (!wasm_runtime_call_wasm(exec_env, func, 1, argv_vals)) {
        printf("Failed to call Wasm function.\n");
        return 1;
    }

    result = argv_vals[0]; // Wasm returns in-place by modifying argv
    printf("Host called Wasm 'increment(41)' and got: %d\n", result);

    wasm_runtime_destroy_exec_env(exec_env);
    wasm_runtime_deinstantiate(module_inst);
    wasm_runtime_unload(module);
    wasm_runtime_destroy();

    free(wasm_buf);
    return 0;
}
