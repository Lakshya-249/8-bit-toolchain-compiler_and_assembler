.text

start:
    lda %a       ; Load the value of %a (first number) into the accumulator
    ldi B 7      ; Load literal 7 into register B (second number)
    add          ; Add B to the accumulator
    sta %r       ; Store the result into register %r

    lda %r       ; Load the result from %r into the accumulator
    out 0        ; Output the result to I/O device 0
    hlt          ; Halt the CPU

.data
a = 105            ; First number (initially 5)
r = 0            ; Result register (initially 0)
