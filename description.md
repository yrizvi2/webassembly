<<<<<<< HEAD
=======
<<<<<<< HEAD
# webassembly
=======
>>>>>>> a3b6152 (initial commit)
# WebAssembly ↔ Host Function Interoperability (WAMR)

This repo demonstrates how to run native host ↔ Wasm function calls using the [Wasm Micro Runtime (WAMR)](https://github.com/bytecodealliance/wasm-micro-runtime), with examples compiled in **minimal wasm mode** (no WASI).

We split the experiment into two parts:

---

## 📌 Part 1: Calling Host Functions from Wasm

**Goal:**  
Have a Wasm module call a function (`print_custom`) that's implemented in the native host (C).

**Steps:**
- `custom_message.c`: Defines an exported Wasm function `trigger_custom()` which calls the host-imported function `print_custom(42)`.
- Compile with `clang` targeting `wasm32` without a standard library.
- Load and run with WAMR using `wamr_runner_invoke_host`.

🟢 **Expected Output**:  
`Host received code: 42`

---

## 📌 Part 2: Calling Wasm Functions from Host

**Goal:**  
Invoke a Wasm-exported function (`increment`) from the host, passing an argument and retrieving the result.

**Steps:**
- `host_calls_wasm.c`: Implements `int increment(int x) { return x + 1; }` and exports it.
- Host loads this Wasm file and uses WAMR APIs to call `increment(41)`.

🟢 **Expected Output**:  
`Host called Wasm 'increment(41)' and got: 42`

---

## 🛠 Tools Used

- [WAMR](https://github.com/bytecodealliance/wasm-micro-runtime)
- `clang` targeting `wasm32`
- `wasm-ld`
- Ubuntu VM

> See [`tools.md`](./tools.md) for setup and paths.

---

## 🧪 Why This Matters

These patterns form the basis of building more complex **syscall-style plugin systems**, such as those used in embedded WASI runtimes like WALI. Understanding both directions of host ↔ Wasm interaction helps us prototype future cross-runtime abstractions and memory-aware plugins.

<<<<<<< HEAD
=======
>>>>>>> 008cf45 (initial commit)
>>>>>>> a3b6152 (initial commit)
