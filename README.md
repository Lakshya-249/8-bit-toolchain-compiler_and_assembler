Simple 8-bit computer in Verilog
================================

This project contains:

* a 8-bit CPU with a basic instruction set
* 256 bytes of RAM



## How to use it

Build an exemple:

```
./asm/asm.py asm/multiplication.asm > memory.list
```

Run the computer:

```
make clean_computer && make run_computer
```

## Assembly

### Instructions set

#### Data transfert group

| Instruction   | Description                                                |
|---------------|------------------------------------------------------------|
| ``lda``       | Alias for ``mov A M D``                                    |
| ``sta``       | Alias for ``mov M A D``                                    |
| ``ldi r D``   | Load _D_ into _r_ register                                 |
| ``mov r M D`` | Copy the data at memory address D into register _r_        |
| ``mov r2 r1`` | Copy register _r1_ into _r2_                               |
| ``mov M r D`` | Copy the data from register _r_ into memory in address _D_ |


Legend:

* _D_ is a byte of data. It can be a memory address or directly the data depending on the instruction.
* _r_ is a register.
* _M_ means "memory", it's used to tell to the ``mov`` instruction the source/destination of the copy.


#### Arithmetic group

| Instruction   | Description                                                |
|---------------|------------------------------------------------------------|
| ``add``       | Perform A = A + B (A, B are registers)                     |
| ``sub``       | Perform A = A - B (A, B are registers)                     |
| ``inc``       | Perform A = A + 1 (A is a register)                        |
| ``dec``       | Perform A = A - 1 (A is a register)                        |


#### Branching group

| Instruction   | Description                                                |
|---------------|------------------------------------------------------------|
| ``jmp D``     | Jump to _D_                                                |
| ``jz D ``     | Jump to _D_ if register A is equal to zero                 |
| ``jnz D``     | Jump to _D_ if register A is not equal to zero             |
| ``je D ``     | Jump to _D_ if register A is equal to register B           |
| ``jne D``     | Jump to _D_ if register A is not equal to register B       |
| ``call D``    | Call sub-routine _D_                                       |
| ``ret``       | Return to the parent routine                               |


#### Machine control

| Instruction   | Description                                                |
|---------------|------------------------------------------------------------|
| ``nop``       | Do nothing                                                 |
| ``hlt``       | Halt the CPU                                               |


#### I/O group

| Instruction   | Description                                                |
|---------------|------------------------------------------------------------|
| ``out``       | Display the content of A                                   |



## Internal function

### Instruction decoding

T1 and T2 are always `FETCH_PC` and `FETCH_INST`.

List of instruction associated with states:

| Instruction | T3        | T4        | T5        | T6       | T7       |
|-------------|-----------|-----------|-----------|----------|----------|
| NOP         |           |           |           |          |          |
| ALU         | ALU_OP    |           |           |          |          |
| OUT         | OUT_A     |           |           |          |          |
| HLT         | HALT      |           |           |          |          |
| JMP         | FETCH_PC  | JUMP      |           |          |          |
| LDI         | FETCH_PC  | LDI       |           |          |          |
| MOV         | MOV_FETCH | MOV_LOAD  | MOV_STORE |          |          |
| CALL        | FETCH_PC  | TMP_STORE | FETCH_SP  | PC_STORE | TMP_JUMP |
| RET         | INC_SP    | FETCH_SP  | RET       |          |          |


States versus signals enabled:

| States        | II | CI | CO | RFI | RFO | EO | MI | RO | RI | HALT | J | OI | SO | SD | SI |
|---------------|----|----|----|-----|-----|----|----|----|----|------|---|----|----|----|----|
| `ALU_OP`      |    |    |    | X   |     | X  |    |    |    |      |   |    |    |    |    |
| `FETCH_INST`  | X  |    |    |     |     |    |    | X  |    |      |   |    |    |    |    |
| `FETCH_PC`    |    | X  | X  |     |     |    | X  |    |    |      |   |    |    |    |    |
| `FETCH_SP`    |    |    |    |     |     |    | X  |    |    |      |   |    | X  |    |    |
| `HALT`        |    |    |    |     |     |    |    |    |    | X    |   |    |    |    |    |
| `INC_SP`      |    |    |    |     |     |    |    |    |    |      |   |    |    |    | X  |
| `JUMP`        |    | *  |    |     |     |    |    | *  |    |      | * |    |    |    |    |
| `LDI`         |    |    |    | X   |     |    |    | X  |    |      |   |    |    |    |    |
| `MOV_FETCH`   |    | *  | *  |     |     |    | *  |    |    |      |   |    |    |    |    |
| `MOV_LOAD`    |    |    |    | *   | *   |    | *  | *  |    |      |   |    |    |    |    |
| `MOV_STORE`   |    |    |    | *   | *   |    |    | *  | *  |      |   |    |    |    |    |
| `OUT_A`       |    |    |    |     | X   |    |    |    |    |      |   | X  |    |    |    |
| `PC_STORE`    |    |    | X  |     |     |    |    |    | X  |      |   |    |    |    |    |
| `RET`         |    | X  |    |     |     |    |    | X  |    |      | X |    |    |    |    |
| `TMP_JUMP`    |    | X  |    |     | X   |    |    |    |    |      | X |    |    | X  | X  |
| `TMP_STORE`   |    |    |    | X   |     |    |    | X  |    |      |   |    |    |    |    |


### Clocks

```
CLK:
          +-+ +-+ +-+ +-+ +-+ +-+ +
          | | | | | | | | | | | | |
          | | | | | | | | | | | | |
          + +-+ +-+ +-+ +-+ +-+ +-+

CYCLE_CLK:
          +---+       +---+
          |   |       |   |
          |   |       |   |
          +   +---+---+   +---+---+

MEM_CLK:
              +---+       +---+
              |   |       |   |
              |   |       |   |
          +---+   +---+---+   +---+

INTERNAL_CLK:
                  +---+       +---+
                  |   |       |   |
                  |   |       |   |
          +---+---+   +---+---+   +
```



## Resources

* [ejrh's CPU in Verilog](https://github.com/ejrh/cpu)
* [Ben Eater's video series](https://eater.net/8bit/)
* [Steven Bell's microprocessor](https://stanford.edu/~sebell/oc_projects/ic_design_finalreport.pdf)
