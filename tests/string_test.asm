.data
msg = "HELLO"
a = 0
.text
loop:
    ldi M 8
    mov A M
    out 0
    hlt             ; Stop 