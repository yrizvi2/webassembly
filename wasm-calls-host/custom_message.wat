(module $custom_message.wasm
  (type (;0;) (func (param i32)))
  (type (;1;) (func))
  (import "host" "print_custom" (func $print_custom (type 0)))
  (func $trigger_custom (type 1)
    i32.const 42
    call $print_custom)
  (table (;0;) 1 1 funcref)
  (memory (;0;) 2)
  (global $__stack_pointer (mut i32) (i32.const 66560))
  (export "memory" (memory 0))
  (export "trigger_custom" (func $trigger_custom)))
