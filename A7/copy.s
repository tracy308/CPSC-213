.pos 0x1000
main:   ld $stack, r5           # r5 = &stack
        inca r5                 # r5 = &stack + 1
        gpc $6, r6              # r6 = return address
        j copy                  # call copy()
        halt                    # end

copy:   ld $-12, r0             # r0 = -12 = size of callee
        add r0, r5              # r5 = stack top
        st r6, 8(r5)            # store return address

        ld $0, r0               # r0 = 0 = i
        ld $src, r1             # r1 = &src

while:  ld (r1, r0, 4), r2      # r2 = src[i]
        beq r2, end_w           # if (src[i] = 0), end_while
        st r2, (r5, r0, 4)      # dst[i] = src[i]
        inc r0                  # r0 = i++
        br while                # goto while

end_w:  ld $0, r1               # r1 = 0;
        st r1, (r5, r0, 4)      # dst[i] = 0;
        ld 8(r5), r6            # r6 = return address
        ld $12, r1              # r1 = 12 = size of callee
        add r1, r5              # deallocate callee
        j (r6)                  # return

.pos 0x2000
src:    .long 0x300c
        .long 0x300c
        .long 0x300c
        .long 0X0000ffff
        .long 0Xffff6001
        .long 0x60126023
        .long 0x60346045
        .long 0x60566067
        .long 0xf000f000
        .long 0

.pos 0x3000
            .long 0
            .long 0
stack:      .long 0