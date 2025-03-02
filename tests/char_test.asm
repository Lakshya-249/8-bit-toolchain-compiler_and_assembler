.text
ldi A 0
mov M A %i
while0:
mov A M %i
push A
ldi A 10
pop B
cmp
jc %bool0
ldi A 1
jmp %endbool0
bool0:
ldi A 0
endbool0:
dec
jc %whileEnd0
mov A M %fib
mov A M %i
mov M A %n_local1
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
out 0
ldi A 1
push A
mov A M %i
pop B
add
mov M A %i
jmp %while0
whileEnd0:
hlt
.data
i=0
fib=0
n_local1=0