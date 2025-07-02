/*
wasm module source file written in C. It calls function imported from host environment - it does not define full logic of printing or handling output. It assumes a function like log_message(int) is available from the host. It simulates what a plugin might do- rely on the host for functionality like logging, file access or syscalls
plays the role of guest module - it compiles to .wasm using wasi-sdk or clang --target=wasm32
declares the host-provided function using an __attribute__
The runtime/host must provide actual C implementation during runtime
*/
#define WASM_EXPORT(A) __attribute__((export_name(A)))
#define WASM_IMPORT(A, B) __attribute__((__import_module__(A), __import_name__(B)))
//tell compiler this function will be provided by host
WASM_IMPORT("host", "print_custom") void print_custom(int code);

WASM_EXPORT("trigger_custom") void trigger_custom() {
    print_custom(42); //call into host - host prints/logs 42
} //exported wasm function that uses imported host function

