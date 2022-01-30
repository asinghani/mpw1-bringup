.section .text.start
.global _start

_start:
    # Calibration byte
    li a0, 0xAA
    call sendbyte

	li t0, 0x30010050
	addi t1, t0, 64 # end ptr

loop:
	lb a0, 0(t0)
	jal ra, sendbyte
    addi t0, t0, 1
	bltu t0, t1, loop

spin:
	jal x0, spin

sendbit:
	li a1, 0x21000000
	sw x0, 0(a1)
	sw x0, 4(a1)
	sw x0, 8(a1)
	sw x0, 12(a1)

	# set output high
    li a2, 1
    sw a2, 0(a1)
    
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    
    # set output high if input is 1, else low
    andi a0, a0, 1
    sw a0, 0(a1)
    
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    
	# set output low
    li a2, 0
    sw a2, 0(a1)

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    
    ret

sendbyte:
	addi a3, a0, 0
    addi a4, ra, 0
    
    andi a0, a3, 1
	jal ra, sendbit
    srli a3, a3, 1
    
    andi a0, a3, 1
	jal ra, sendbit
    srli a3, a3, 1
    
    andi a0, a3, 1
	jal ra, sendbit
    srli a3, a3, 1
    
    andi a0, a3, 1
	jal ra, sendbit
    srli a3, a3, 1
    
    andi a0, a3, 1
	jal ra, sendbit
    srli a3, a3, 1
    
    andi a0, a3, 1
	jal ra, sendbit
    srli a3, a3, 1
    
    andi a0, a3, 1
	jal ra, sendbit
    srli a3, a3, 1
    
    andi a0, a3, 1
	jal ra, sendbit
    srli a3, a3, 1
    
    addi ra, a4, 0
    
    ret
