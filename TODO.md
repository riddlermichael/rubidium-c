## `C`

### `containers`

- [ ] list
- [ ] map
- [ ] set
- [ ] span
- [ ] string_view
- [ ] vector
- [ ] print container

### `core`

- [ ] `i128`/`u128`
- [ ] SAL annotations
- [ ] meta-types
- [ ] safe opCmp
- [ ] stable values for `rbc_error`
- [ ] `cold` attribute

### `fmt`

- [ ] pretty print

### `log`

- [ ] revise current implementation
- [ ] C <-> C++ bridge

### `io`

- [ ] serial port
- [ ] serial port info
- [ ] serial port listener

### `sync`

- [ ] `[[nodiscard]]`
- [ ] static mutexes
- [ ] do we need to get actual stack size?
- [ ] windows
  - [ ] `cond_var`
  - [ ] `mutex`
  - [ ] `spinlock`
  - [ ] `tls` destructor

### `time`

- [ ] `TIME_V2`: use `i128`
- [ ] duration
  - [ ] reduce size to 12 bytes
  - [ ] division
  - [ ] conversions to s/ns/...
  - [ ] to/from string
- [ ] time to/from string
- [ ] `rbc_timespec_getres` has resolution 100ns

### Unsorted

- [ ] `RBC_CONST` and `RBC_PURE` where possible
  - [ ] `rbc_duration`
- [ ] build with TinyCC
- [ ] `-fbounds-safety`
