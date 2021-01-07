.pos 0x100
                ld $data, r0            # r0 = address of data
                ld $i, r1               # r1 = address of r1
                ld (r1), r2             # r2 = value of r1
                ld (r0, r2, 4), r3      # r3 = data[i]
                inc r2                  # increment r2
                ld (r0, r2, 4), r4      # r4 = data[i+1]
                add r4, r3              # r3 =  r3 + r4
                ld $y, r0               # r0 = address of y
                st r3, (r0)             # y = r3

                ld $x, r1               # r1 = address of x
                ld $0xff, r3              # r3 = 0xff
                ld (r0), r4             # r4 = value of r0 (y)
                and r3, r4              # r4 = r4 & r3
                st r4, (r1)             # r1 = r0
                halt
                


.pos 0x1000
data:           .long 0xffffffff        # data[0]
                .long 0xffffffff        # data[1]
                .long 0xffffffff        # data[2]
                .long 0xffffffff        # data[3]

.pos 0x2000
i:              .long 0x0               # i

.pos 0x3000
x:              .long 0x0               # x

.pos 0x4000
y:              .long 0x0               # y
