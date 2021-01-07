.pos 0x100
                    ld $b, r0               # r0 = address of b
                    ld (r0), r1             # r1 = value of b
                    inc r1                  # increment r1
                    ld $4, r2               # r2 = 4
                    add r2, r1              # r1 = r1 + r2
                    shr $1, r1              # r1 shift right by 1 bit (divide by 2)
                    ld (r0), r2             # r2 = value of b
                    and r2, r1              # r1 = r1 & r3
                    shl $2, r1              # r1 shift left by 2 bit (r1 * 4)
                    ld $a, r0               # r0 = address of a
                    st r1, (r0)             # a = r1
                    
                    halt                    # stop


.pos 0x1000
a:                  .long 0x00000000         # a
.pos 0x2000
b:                  .long 0x00000001         # b
