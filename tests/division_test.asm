.text
ldi A 2
push A
ldi A 4
sta %d0
pop B
startD0:
lda %d0
cmp
jc %remainder0
lda %q0
inc
sta %q0
lda %d0
sub
sta %d0
jmp %startD0
remainder0:
lda %q0
push A
ldi A 6
push A
ldi A 5
sta %i0
pop B
start0:
lda %r0
add
sta %r0
lda %i0
dec
sta %i0
jnz %start0
lda %r0
sta %i1
pop B
start1:
lda %r1
add
sta %r1
lda %i1
dec
sta %i1
jnz %start1
lda %r1
out 0
hlt
.data
i1=0
r1=0
i0=0
r0=0
d0=0
q0=0