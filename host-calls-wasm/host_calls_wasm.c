#define WASM_EXPORT(A) __attribute__((export_name(A)))

WASM_EXPORT("increment") int increment(int x) {
	return x + 1; 
}
