.text
ldi A 5
mov M A %x
ldi A 10
mov M A %y
mov A M %y
out 0
mov A M %x
out 0
mov A M %x
push A
mov A M %y
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
push A
mov A M %x
pop B
add
mov M A %z
ldi A 29
mov M A %x
ldi A 2
push A
mov A M %z
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
mov A M %x
out 0
hlt
.data
z=0
y=0
x=0