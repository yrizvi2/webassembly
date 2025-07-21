(module
  (type $t0 (func (param i32)))
  (func $print_num_export (type $t0)
    local.get 0
    drop
  )
  (table $func_table 11 funcref)
  (elem (i32.const 10) $print_num_export)
  (func $addr_export (result i32)
    i32.const 10
  )
  (func $force_table_use
    i32.const 42
    i32.const 10
    call_indirect (type $t0)
  )
  (export "print_num_export" (func $print_num_export))
  (export "addr_export" (func $addr_export))
  (export "force_table_use" (func $force_table_use))
  (export "table" (table $func_table))
)