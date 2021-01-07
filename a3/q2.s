.pos 0x100
    # tmp = 0;
    ld $0, r0               # r0 = 0
    ld $tmp, r1             # r1 = address of tmp
    st r0, (r1)             # tmp = 0
    
    # tos = 0;
    ld $tos, r2             # r2 = address of tos
    st r0, (r2)             # tos = 0

    # s[tos] = a[0];
    ld (r2), r0             # r0 = tos = 0
    ld $a, r3               # r3 = address of a
    ld $s, r4               # r4 = address of s
    ld (r3), r5             # r5 = a[0]
    st r5, (r4, r0, 4)      # s[tos] = a[0];

    # tos++;
    inc r0                  # r0 = r0 + 1
    st r0, (r2)             # tos++

    # s[tos] = a[1];
    ld (r2), r0             # r0 = tos = 0
    ld 4(r3), r5            # r5 = a[1]
    st r5, (r4, r0, 4)      # s[tos] = a[0];
    
    # tos++;
    inc r0                  # r0 = r0 + 1
    st r0, (r2)             # tos++

    # s[tos] = a[2];
    ld (r2), r0             # r0 = tos = 0
    ld 8(r3), r5            # r5 = a[1]
    st r5, (r4, r0, 4)      # s[tos] = a[0];

    # tos++;
    inc r0                  # r0 = r0 + 1
    st r0, (r2)             # tos++

    # tos--;
    dec r0                  # r0 = r0 - 1
    st r0, (r2)             # tos--

    # tmp = s[tos];
    ld (r4, r0, 4), r3      # r3 = s[tos]
    st r3, (r1)             # tmp = r3 = s[tos]

    # tos--;
    dec r0                  # r0 = r0 - 1
    st r0, (r2)             # tos--

    # tmp = tmp + s[tos];
    ld (r4, r0, 4), r3      # r3 = s[tos]
    ld (r1), r5             # r2 = tmp
    add r3, r5              # r5 = tmp + s[tos]
    st r5, (r1)             # tmp = tmp + s[tos]

    # tos--;
    dec r0                  # r0 = r0 - 1
    st r0, (r2)             # tos--

    # tmp = tmp + s[tos];
    ld (r4, r0, 4), r3      # r3 = s[tos]
    ld (r1), r5             # r2 = tmp
    add r3, r5              # r5 = tmp + s[tos]
    st r5, (r1)             # tmp = tmp + s[tos]

    halt

.pos 0x200
a:      .long 0             # a[0]
        .long 0             # a[1]
        .long 0             # a[2]
s:      .long 0             # s[0]
        .long 0             # s[1]
        .long 0             # s[2]
        .long 0             # s[3]
        .long 0             # s[4]
tos:    .long 0             # tos
tmp:    .long 0             # tmp
