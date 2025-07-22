/* === Native-to-Wasm Callback Logic === */
typedef void (*Tfunc)(uint32_t);

wasm_exec_env_t exec_env = wasm_runtime_create_exec_env(wasm_module_inst, stack_size);
if (!exec_env) {
    printf("Failed to create exec environment.\n");
    goto fail4;
}

wasm_function_inst_t func = wasm_runtime_lookup_function(wasm_module_inst, "addr", NULL);
if (!func) {
    printf("Function 'addr' not found in module.\n");
} else {
    if (!wasm_runtime_call_wasm(exec_env, func, 0, NULL)) {
        printf("Error invoking 'addr': %s\n", wasm_runtime_get_exception(wasm_module_inst));
    } else {
        uint32_t callback_addr = 0;
        wasm_runtime_get_return_value(exec_env, &callback_addr);
        printf("Got function pointer from Wasm: %u\n", callback_addr);

        Tfunc fn = (Tfunc)(uintptr_t)callback_addr;
        fn(42); // should print: you gave me = 42
    }
}
