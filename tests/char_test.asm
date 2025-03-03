.text
mov A M %fib
ldi A 4
mov M A %n_local1
call %fib
out 0
hlt
fib:
ldi A 5
push A
mov A M %n_local1
pop B
cmp
jc %bool0
ldi A 1
jmp %endbool0
bool0:
ldi A 0
endbool0:
dec
jc %else0
mov A M %n_local1
push A
mov A M %n_local1
mov C A
pop B
ldi A 0
push A
startM0:
pop A
add
push A
mov A C
dec
mov C A
jnz %startM0
pop A
ret
jmp %endif0
else0:
mov A M %n_local1
push A
ldi A 3
pop B
cmp
jc %bool1
ldi A 1
jmp %endbool1
bool1:
ldi A 0
endbool1:
dec
jc %else1
ldi A 10
push A
mov A M %n_local1
mov C A
pop B
ldi A 0
push A
startM1:
pop A
add
push A
mov A C
dec
mov C A
jnz %startM1
pop A
ret
jmp %endif1
else1:
ldi A 1
push A
ldi A 20
push A
mov A M %n_local1
mov C A
pop B
ldi A 0
push A
startM2:
pop A
add
push A
mov A C
dec
mov C A
jnz %startM2
pop A
pop B
add
ret
endif1:
endif0:
ldi A 0
ret
.data
fib=0
n_local1=0
