.text
ldi A 80
sta %d0
ldi A 4
startD0:
lda %d0
ldi B 4
cmp
jc %remainder0
lda %q0
inc
sta %q0
lda %d0
ldi B 4
sub
sta %d0
jmp %startD0
remainder0:
lda %q0
out 0
hlt
.data
d0=0
q0=0