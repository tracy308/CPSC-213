.pos 0x100
    # i = a[3]
    ld $i, r0           # r0 = address of i
    ld $a, r1           # r1 = address of a
    ld 12(r1), r2       # r2 = a[3]
    st r2, (r0)         # i = r2 = a[3]
    
    # i = a[i]
    ld (r0), r2         # r2 = i
    ld (r1, r2, 4), r3  # r3 = a[i]
    st r3, (r0)         # i = a[i]

    # p  = &j;
    ld $p, r0           # r0 = address of p
    ld $j, r1           # r1 = address of j
    st r1, (r0)         # p = &j
    
    # *p = 4;
    ld $4, r1           # r1 = 4
    ld (r0), r0         # r0 = j (which itself is an address)
    st r1, (r0)         # *p = j = 4
    
    # p = &a[a[2]];
    ld $a, r0           # r0 = address of a
    ld 8(r0), r1        # r1 = a[2]
    shl $2, r1          # r1 = 4 * a[2]
    add r0, r1          # r1 = r0 + r1
    ld $p, r2           # r2 = address of p
    st r1, (r2)         # p = a[a[2]]
    
    # *p = *p + a[4];
    ld $p, r0           # r0 = address of p
    ld (r0), r1         # r1 = p = address of a[a[2]]
    ld (r1), r2         # r2 = *p
    ld $a, r3           # r3 = address of a
    ld 16(r3), r3       # r3 = a[4]
    add r3, r2          # r2 = *p + a[4]
    st r2, (r1)         # *p = *p + a[4]
    
    halt

.pos 0x200
# Data area

i:  .long 0             # i
j:  .long 0             # j
p:  .long 0             # p
a:  .long 0             # a[0]
    .long 0             # a[1]
    .long 0             # a[2]
    .long 0             # a[3]
    .long 0             # a[4]
    .long 0             # a[5]
    .long 0             # a[6]
    .long 0             # a[7]
    .long 0             # a[8]
    .long 0             # a[9]
