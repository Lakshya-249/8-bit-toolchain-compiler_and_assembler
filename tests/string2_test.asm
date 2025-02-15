.data
msg = "HELLO"
i = 1

.text
start:
    lda %msg
loop:
    pop B
    cmp
    jnz %loop
    lda %i
    jmp %index
index:
    dec 
    pop B
    jnz %index
    mov A B
    jmp %output
output:
    out 0
    hlt