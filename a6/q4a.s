.pos 0x0
                 ld   $sb, r5               # r5 = &stack
                 inca r5                    # r5 = &stack + 1 sb[-1]
                 gpc  $6, r6                
                 j    0x300             
                 halt                     
.pos 0x100
                 .long 0x00001000         
.pos 0x200
                 ld   (r5), r0                  # r0 = 3,   1
                 ld   4(r5), r1                 # r1 = 4,   2
                 ld   $0x100, r2                # r2 = 0x100
                 ld   (r2), r2                  # r2 = 0x1000
                 ld   (r2, r1, 4), r3           # r3 = m[r2[4]] = 0 / m[r2[2]] = 0
                 add  r3, r0                    # r0 = 0 + 3 / 0 + 1
                 st   r0, (r2, r1, 4)           # m[r2[4]] = 3 / m[r2[2]] = 1
                 j    (r6)                      # jump to (r6) line 34 / line 44
.pos 0x300
                 ld   $-12, r0                  # r0 = -12
                 add  r0, r5                    # r5 = r5 -12 = sb[2]
                 st   r6, 8(r5)                 # sb[0] = halt
                 ld   $1, r0                    # r0 = 1
                 st   r0, (r5)                  # sb[2] = 1
                 ld   $2, r0                    # r0 = 2
                 st   r0, 4(r5)                 # sb[1] = 2
                 ld   $-8, r0                   # r0 = -8
                 add  r0, r5                    # r5 = r5 - 8 = sb[4]
                 ld   $3, r0                    # r0 = 3
                 st   r0, (r5)                  # sb[4] = 3
                 ld   $4, r0                    # r0 = 4
                 st   r0, 4(r5)                 # sb[3] = 4
                 gpc  $6, r6                    
                 j    0x200
                 ld   $8, r0                    # r0 = 8
                 add  r0, r5                    # r5 = sb[2]
                 ld   (r5), r1                  # r1 = sb[2] = 1
                 ld   4(r5), r2                 # r2 = 2
                 ld   $-8, r0                   # r0 = -8
                 add  r0, r5                    # r5 = sb[4]
                 st   r1, (r5)                  # sb[4] = 1
                 st   r2, 4(r5)                 # sb[3] = 2
                 gpc  $6, r6                    # r6 = get pc + 6
                 j    0x200                     
                 ld   $8, r0                    # r0 = 8
                 add  r0, r5                    # r5 = sb[2]
                 ld   8(r5), r6                 # r6 = sb[0]
                 ld   $12, r0                   # r0 = 12
                 add  r0, r5                    # r5 += 12 = sb[0]
                 j    (r6)                      # go to (r6)
.pos 0x1000
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
.pos 0x8000
# These are here so you can see (some of) the stack contents.
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
sb: .long 0
