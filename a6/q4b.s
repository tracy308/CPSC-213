.pos 0x100
start:
    ld $sb, r5          # r5 = &sb
    inca    r5          # r5 = &sb[-1]
    gpc $6, r6          # r6 = pc + 6 = line 7
    j main              # call main
    halt

f:
    deca r5                     # r5 -= 4 (new top)
    ld $0, r0                   # r0 = 0
    ld 4(r5), r1                # r1 = x[i]
    ld $0x80000000, r2          # r2 = 0x80000000
f_loop:
    beq r1, f_end               # if (r1 = 0), f_end
    mov r1, r3                  # r3 = r1 = x[i]
    and r2, r3                  # r3 = r2 & r3
    beq r3, f_if1               # if (r3 = 0), f_if1
    inc r0                      # r0++
f_if1:
    shl $1, r1                  # r1 = r1 * 2
    br f_loop                   # brancj to f_loop
f_end:
    inca r5                     # r5 = &sb[2]
    j(r6)                       # go to (r6)

main:
    deca r5                     # r5 = &sb[0]
    deca r5                     # r5 = &sb[1]
    st r6, 4(r5)                # &sb[0] = r6 = line 7
    ld $8, r4                   # r4 = 8 = i
main_loop:
    beq r4, main_end            # if (r4 = 0), main_end
    dec r4                      # r4--
    ld $x, r0                   # r0 = &x
    ld (r0,r4,4), r0            # r0 = x[i]
    deca r5                     # r5 = &sb[1+9-i]
    st r0, (r5)                 # sb[2] = x[i]
    gpc $6, r6                  # r6 = pc + 6 = line 41
    j f                         # call f
    inca r5                     # r5 += 4
    ld $y, r1                   # r1 = &y
    st r0, (r1,r4,4)            # y[i] = r0
    br main_loop
main_end:
    ld 4(r5), r6                # r6 = &sb[0]
    inca r5                     # r5 += 4
    inca r5                     # r5 += 4
    j (r6)                      # go to (r6)

.pos 0x2000
x:
    .long 1
    .long 2
    .long 3
    .long -1
    .long -2
    .long 0
    .long 184
    .long 340057058

y:
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

