.pos 0x100
    # a = 3
    ld $a, r0           # r0 = address of a
    ld $3, r1           # r1 = 3
    st r1, (r0)         # a = 3
        
    # p = &a;
    ld $p, r1           # r1 = address of p
    st r0, (r1)         # p = &a

    # *p = *p - 1;
    ld (r1), r2         # r2 = &a
    ld (r2), r3         # r3 = *p = a
    dec r3              # *p - 1 = a--
    st r3, (r2)         # *p = *p - 1

    # p = &b[0];
    ld $b, r2           # r2 = address of b
    st r2, (r1)         # p = address of b

    # p++;
    inca r2             # r2 = address of b + 4
    st r2, (r1)         # p = p++ = address of b + 4

    # p[a] = b[a];
    ld (r0), r0         # r0 = a
    ld $b, r3           # r3 = address of b
    ld (r3, r0, 4), r2  # r2 = b[a]
    ld (r1), r1         # r1 = value of p
    st r2, (r1, r0, 4)  # p[a] = b[a]

    # *(p+3) = b[0];
    ld (r3), r3         # r3 = b[0]
    st r3, 12(r1)       # *(p+3) = b[0]

    halt

.pos 0x200
a:  .long 0             # a
p:  .long 0             # p
b:  .long 0             # b[0]
    .long 1             # b[1]
    .long 2             # b[2]
    .long 3             # b[3]
    .long 4             # b[4]
