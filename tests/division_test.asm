.text
ldi A 4
push A
ldi A 5
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
mov M A %x
ldi A 10
mov M A %y
mov A M %y
out 0
mov A M %x
out 0
mov A M %y
push A
mov A M %x
pop B
add
mov M A %z
mov A M %z
out 0
hlt
.data
z=0
y=0
x=0