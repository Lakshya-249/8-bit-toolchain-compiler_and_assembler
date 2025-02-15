.text
ldi A 90
push A
ldi A 2
sta %i0
start0:
mov B M %r0
ldi A 6
add
sta %r0
lda %i0
dec
sta %i0
jnz %start0
lda %r0
sta %i1
start1:
mov B M %r1
ldi A 4
add
sta %r1
lda %i1
dec
sta %i1
jnz %start1
lda %r1
pop B
add
out 0
hlt
.data
i1=0
r1=0
i0=0
r0=0