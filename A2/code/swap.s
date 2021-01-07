.pos 0x100
                ld  $array, r0          # r0 = address of array[0]
                ld  (r0), r1            # r1 = array[0]
                ld  $t, r2              # r2 = address of t
                st  r1, (r2)            # t = array[0]


                ld  0x4(r0), r3         # r3 = array[1]
                st  r3, (r0)            # array[0] = array[1]

                ld  (r2), r1            # r1 = t
                st  r1, 0x4(r0)         # array[1] = t

                halt                    # halt

.pos 0x1000
t:               .long 0xffffffff         # t
.pos 0x2000
array:           .long 0x00000001         # array[0]
                 .long 0x00000002         # array[1]
