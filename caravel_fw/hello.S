.section .text.start
.global _start

_start:
    # Calibration byte
    li a0, 0xAA
    call sendbyte

    li a0, 'H'
    call sendbyte

    li a0, 'e'
    call sendbyte

    li a0, 'l'
    call sendbyte

    li a0, 'l'
    call sendbyte

    li a0, 'o'
    call sendbyte


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
