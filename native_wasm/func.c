#include <stdint.h>
#include <stdio.h>
#include "wasm_export.h"

typedef void (*Tfunc)(uint32_t);

void print_num(uint32_t x) {
    printf("you gave me = %d\n", x);
}

Tfunc function = print_num;

// ✅ Macro goes after return type, before function name
uint32_t WASM_EXPORT("addr") addr() {
    return (uint32_t)(uintptr_t)&function;
}
