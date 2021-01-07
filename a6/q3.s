.pos 0x1000
        ld $s, r0               # r0 = &s
        ld (r0), r0             # r0 = &base
        ld $n, r5               # r5 = &n
        ld (r5), r5             # r5 = n = n'
        gpc $6, r6              
        j avg                   # jump to average
        dec r5                  # r5--

avg_l:  beq r5, start           # jump to start
        ld $24, r7              # r7 = 24
        add r7, r0              # r0 = r0 + 24 = address of next student
        gpc $6, r6              # r6 = pc+6
        j avg                   # jump to avg
        dec r5                  # r5--
        br avg_l                # go back to avg loop

avg:    ld 4(r0), r1            # r1 = grade 0
        ld 8(r0), r2            # r2 = grade 1
        ld 12(r0), r3           # r3 = grade 2
        ld 16(r0), r4           # r4 = grade 3
        add r1, r2              # r2 = grade 0 + grade 1
        add r2, r3              # r3 = grade 0 + grade 1 + grade 2
        add r3, r4              # r4 = grade 0 + grade 1 + grade 2 + grade 3
        shr $2, r4              # r4 = computed average
        st r4, 20(r0)           # base[5] = r4 = computed average
        j (r6)

start:  ld $n, r5		# r5 = &n
        ld (r5), r5             # r5 = n
        dec r5		        # r5--

sort:   bgt r5, sort_init	# if(r5 > 0), sort_init			
        j median		# jump to median

sort_init:      ld $s, r0	        # r0 = &s 
                ld (r0), r0	        # r0 = s[0]
                ld $24, r7	        # r7 = 24
                add r7, r0	        # r0 = r0 + 24
                ld $1, r3	        # r3 = 1

sort_once:      mov r5, r4		# r4 = r5 = n - 1
                not r4		        # r4 = ~r4
                inc r4			# r4 = -r5 (obtain negative value)
                add r3, r4		# r4 = r4 + r3
                bgt r4, no_swap	        # if (r4 > 0), inner sort loop
           
compare:        ld 20(r0), r1		# r1 = student i avg
                ld $-4, r7		# r7 = -4
                mov r0, r6		# r6 = r0
                add r7, r6		# r6 = r6 + r7
                ld (r6), r2		# r2 = (r6) = student i-1 avg
                not r1			# r1 = ~r1
                inc r1			# r1 = r1 + 1 = -(student i  avg)
                add r1, r2		# r2 = r2 + r1
                bgt r2, swap		# if(r2 > 0), swap
           
next_student:   ld $24, r7		# go to next element in list of students
                add r7, r0
                inc r3
                j sort_once
           
no_swap:        dec r5		# r5--
                j sort		# jump to sort

swap:   ld $-24, r7             # r7 = -24
        mov r0, r6              # r6 = r0
        add r7, r6              # r6 = &student i-1

        ld (r0), r1             # swap student ID
        ld (r6), r2             
        st r2, (r0)                     
        st r1, (r6)

        ld 4(r0), r1            # swap grade 0
        ld 4(r6), r2                     
        st r2, 4(r0)                     
        st r1, 4(r6)

        ld 8(r0), r1            # swap grade 1
        ld 8(r6), r2     
        st r2, 8(r0)                     
        st r1, 8(r6)

        ld 12(r0), r1           # swap grade 2
        ld 12(r6), r2     
        st r2, 12(r0)                     
        st r1, 12(r6)

        ld 16(r0), r1           # swap grade 3
        ld 16(r6), r2     
        st r2, 16(r0)                     
        st r1, 16(r6)

        ld 20(r0), r1           # swap grade avg
        ld 20(r6), r2     
        st r2, 20(r0)                     
        st r1, 20(r6)

        j next_student          # jump to next_student 

median: ld $1, r1		# r1 = 1
        ld $n, r5		# r5 = &n
        ld (r5), r5             # r5 = n
        mov r5, r2		# r2 = r5
        add r1, r2		# r2 = r2 + 1
        shr $1, r2		# r2 = r2 / 2
        ld $24, r3		# r3 = 24
        ld $0, r4		# r4 = 0
        dec r2		

find:   beq r2, get_m		# if(r2 = 0), get
        add r3, r4		# r4 = r4 + r3
        dec r2
        br find			# branch to cond

get_m:  ld $s, r0		# r0 = &s
        ld (r0), r0		# r0 = s[0]
        add r0, r4		# r4 = r4 + r0
        ld (r4), r4             # r4 = student ID with median grade
        ld $m, r5		# r5 = median
        st r4, (r5)		# r4 = (r5)
        
end:    halt

.pos 0x3000
n:      .long 7                 # just one student
m:      .long 0                 # put the answer here
s:      .long base              # address of the array

base:
	.long 1111		# studend id
	.long 1			# grade 0
	.long 1			# grade 1
	.long 1			# grade 2
	.long 1			# grade 3
	.long 0			# computed average

	.long 7777		# studend id
	.long 7			# grade 0
	.long 7			# grade 1
	.long 7			# grade 2
	.long 7			# grade 3
	.long 0			# computed average

	.long 4444		# studend id
	.long 4			# grade 0
	.long 4			# grade 1
	.long 4			# grade 2
	.long 4			# grade 3
	.long 0			# computed average

	.long 6666		# studend id
	.long 6			# grade 0
	.long 6			# grade 1
	.long 6			# grade 2
	.long 6			# grade 3
	.long 0			# computed average

	.long 2222		# studend id
	.long 2			# grade 0
	.long 2			# grade 1
	.long 2			# grade 2
	.long 2			# grade 3
	.long 0			# computed average

	.long 3333		# studend id
	.long 3			# grade 0
	.long 3			# grade 1
	.long 3			# grade 2
	.long 3			# grade 3
	.long 0			# computed average

	.long 5555		# studend id
	.long 5			# grade 0
	.long 5			# grade 1
	.long 5			# grade 2
	.long 5			# grade 3
	.long 0			# computed average