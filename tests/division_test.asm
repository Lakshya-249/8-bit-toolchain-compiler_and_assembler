.text
ldi A 5
mov M A %x
ldi A 19
mov M A %y
ldi A 10
mov M A %x_local1
mov A M %y
push A
mov A M %x_local1
pop B
add
out 0
ldi A 60
mov M A %y
ldi A 37
mov M A %x_local1
mov A M %y
push A
mov A M %x_local1
pop B
add
mov M A %z_local1
ldi A 10
mov M A %x_local2
ldi A 5
mov M A %x_local2
mov A M %x_local2
push A
mov A M %z_local1
pop B
add
out 0
ldi A 19
mov M A %y
mov A M %y
push A
mov A M %x
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
hlt
.data
y=0
x=0
x_local1=0
z_local1=0
x_local2=0