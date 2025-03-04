.text
ldi A 10
mov M A %a
ldi A 15
mov M A %b
mov A M %a
out 0
mov A M %b
out 0
mov A M %reverse
mov A M %a
mov M A %a_local1
mov A M %b
mov M A %b_local1
call %reverse
mov A M %add
mov A M %a
mov M A %a_local1
mov A M %b
mov M A %b_local1
call %add
mov M A %x_local2
mov A M %x_local2
out 0
ldi A 2
push A
ldi A 3
push A
ldi A 5
pop B
and
pop B
or
mov M A %ans
mov A M %ans
out 0
ldi A 0
ldi B 1
cmp
jc %orand1
jmp %orandEnd1
orand1:
ldi A 65
orandEnd1:
ldi B 1
cmp
jc %orand0
jmp %orandEnd0
orand0:
ldi A 3
orandEnd0:
out 0
hlt
reverse:
mov A M %a_local1
mov M A %temp_local1
mov A M %b_local1
mov M A %a_local1
mov A M %temp_local1
mov M A %b_local1
mov A M %a_local1
out 0
mov A M %b_local1
out 0
ldi A 0
ret
add:
mov A M %b_local1
push A
mov A M %b_local1
push A
mov A M %a_local1
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
pop B
add
mov M A %temp_local1
mov A M %temp_local1
ret
ldi A 0
ret
.data
ans=0
b=0
a=0
add=0
reverse=0
x_local2=0
temp_local1=0
b_local1=0
a_local1=0