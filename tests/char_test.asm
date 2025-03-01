.text
ldi A 0
mov M A %a
ldi A 1
mov M A %b_local1
while0:
mov A M %a
push A
ldi A 100
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
mov A M %a
out 0
mov A M %a
mov M A %temp
mov A M %b_local1
mov M A %a
mov A M %b_local1
push A
mov A M %temp
pop B
add
mov M A %b_local1
jmp %while0
whileEnd0:
hlt
.data
temp=0
a=0
b_local1=0