.text
ldi A 3
push A
ldi A 5
push A
ldi A 4
mov C A
pop B
ldi A 0
push A
start0:
pop A
add
push A
mov A C
dec
mov C A
jnz %start0
pop A
push A
ldi A 2
push A
ldi A 6
mov C A
pop B
ldi A 0
push A
start1:
pop A
add
push A
mov A C
dec
mov C A
jnz %start1
pop A
mov C A
pop B
ldi A 0
push A
start2:
pop A
add
push A
mov A C
dec
mov C A
jnz %start2
pop A
pop B
push A
ldi C 0
startD0:
pop A
cmp
push A
jc %remainder0
mov A C
inc
mov C A
pop A
sub
push A
jmp %startD0
remainder0:
pop A
mov A C
out 0
hlt
.data