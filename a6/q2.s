.pos 0x100
code:   ld $i, r0               # r0 = &i
        ld (r0), r0             # r0 = i = i'
        ld $0, r0               # i = 0

        ld $n, r1               # r1 = &n
        ld (r1), r1             # r1 = n = n'
        not r1                  # r1 = not n
        inc r1                  # r1 = -n
        ld $a, r2               # r2 = &a
        ld $b, r3               # r3 = &b
        ld $c, r4               # r4 = &c
        ld (r4), r5             # r4 = c = c'

L0:     mov r1, r6              # r6 = r1
        add r0, r6              # r6 = r6 + r0
        beq r6, end             # if (i < n = 0), jump to end
        ld (r2, r0, 4), r6      # r6 = a[i]
        ld (r3, r0, 4), r7      # r7 = b[i]
        not r7                  # r7 = not b[i]
        inc r7                  # r7 = -b[i]
        add r6, r7              # r7 = a[i] + -b[i]
        bgt r7, true            # if a[i] > b[i], jump to true
        j false                 # jump to false

true:   inc r5                  # r5 = c++
        st r5, (r4)             # c = r5

false:  inc r0                  # i++
        ld $i, r6               # r6 = &i
        st r0, (r6)             # i = r0
        j L0                    # jump back to LO

end:
        halt

.pos 0x1000
i:      .long -1            # i
n:      .long 5             # n
a:      .long 10            # a[0]
        .long 20            # a[1]
        .long 30            # a[2]
        .long 40            # a[3]
        .long 50            # a[4]
b:      .long 11            # b[0]
        .long 20            # b[1]
        .long 28            # b[2]
        .long 44            # b[3]
        .long 48            # b[4]
c:      .long 0             # c
