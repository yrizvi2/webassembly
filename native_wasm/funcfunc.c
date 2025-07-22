wasm_exec_env_t exec_env = wasm_runtime_get_exec_env_singleton(wasm_module_inst);
if (!exec_env) {
    printf("Failed to get exec_env\n");
    goto fail4;
}

// Lookup exported 'addr' function from func.wasm
wasm_function_inst_t addr_func = wasm_runtime_lookup_function(wasm_module_inst, "addr", NULL);
if (!addr_func) {
    printf("Failed to find 'addr' function\n");
    goto fail4;
}

// Call addr() to get table index
if (!wasm_runtime_call_wasm(exec_env, addr_func, 0, NULL)) {
    printf("Failed to call 'addr': %s\n", wasm_runtime_get_exception(wasm_module_inst));
    goto fail4;
}

uint32_t func_index = *(uint32_t *)wasm_runtime_get_return_value(exec_env);
printf("Got function index from Wasm: %u\n", func_index);

// Prepare parameters for indirect call
uint32_t params[2] = { 42, 0 };
uint32_t param_count = sizeof(params) / sizeof(uint32_t);

bool ok = wasm_runtime_call_indirect(exec_env, func_index, param_count, params);
if (!ok) {
    printf("Indirect call failed: %s\n", wasm_runtime_get_exception(wasm_module_inst));
    goto fail4;
}
