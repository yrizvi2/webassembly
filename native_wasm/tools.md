# Tooling Setup (Paths & Versions)

### WAMR Path:
Installed at:

~/wasm/tools/wasm-micro-runtime/
Used native C build system to compile:

cd wasm-micro-runtime/product-mini/platforms/linux
mkdir build && cd build
cmake ..
make

Output library used in:
build/libiwasm.a


### Clang Setup
We use the system-wide installed clang for basic wasm compilation:
sudo apt update && sudo apt install clang

Check version:
which clang
clang --version
Example call:
clang --target=wasm32 -O3 -nostdlib \
  -Wl,--no-entry \
  -Wl,--export=trigger_custom \
  -o custom_message.wasm custom_message.c

 Optional: WAT inspection
We used wabt for .wat conversion:
wasm2wat custom_message.wasm -o custom_message.wat

### Directory Layout
wasm-calls-host/: Demonstrates Wasm calling a native host import


host-calls-wasm/: Demonstrates native host invoking an exported Wasm function


Verified On
Ubuntu 22.04 (VM)


System clang


WAMR v2.3.1

