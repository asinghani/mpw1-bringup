.section .text.start
.global _start

_start:
    # Calibration byte
    li a0, 0xAA
    call sendbyte

	li t0, 0x30010000

    # sha256 init
    li t1, 1
    sw t1, 0(t0)

    li t1, 0x33613537
    sw t1, 4(t0)
    li t1, 0x63643330
    sw t1, 4(t0)
    li t1, 0x35363066
    sw t1, 4(t0)
    li t1, 0x62326637
    sw t1, 4(t0)
    li t1, 0x32383361
    sw t1, 4(t0)
    li t1, 0x37343333
    sw t1, 4(t0)
    li t1, 0x65636539
    sw t1, 4(t0)
    li t1, 0x31393436
    sw t1, 4(t0)
    li t1, 0x61633664
    sw t1, 4(t0)
    li t1, 0x61346234
    sw t1, 4(t0)
    li t1, 0x30343063
    sw t1, 4(t0)
    li t1, 0x38633337
    sw t1, 4(t0)
    li t1, 0x61666135
    sw t1, 4(t0)
    li t1, 0x36343031
    sw t1, 4(t0)
    li t1, 0x39396465
    sw t1, 4(t0)
    li t1, 0x64353964
    sw t1, 4(t0)
    li t1, 0x80000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000000
    sw t1, 4(t0)
    li t1, 0x00000200
    sw t1, 4(t0)

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
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    li t0, 0x30010010
	lw t1, 0(t0)

    srli a0, t1, 24
	jal ra, sendbyte

    srli a0, t1, 16
	jal ra, sendbyte

    srli a0, t1, 8
	jal ra, sendbyte

    srli a0, t1, 0
	jal ra, sendbyte


    li t0, 0x30010014
	lw t1, 0(t0)

    srli a0, t1, 24
	jal ra, sendbyte

    srli a0, t1, 16
	jal ra, sendbyte

    srli a0, t1, 8
	jal ra, sendbyte

    srli a0, t1, 0
	jal ra, sendbyte


    li t0, 0x30010018
	lw t1, 0(t0)

    srli a0, t1, 24
	jal ra, sendbyte

    srli a0, t1, 16
	jal ra, sendbyte

    srli a0, t1, 8
	jal ra, sendbyte

    srli a0, t1, 0
	jal ra, sendbyte


    li t0, 0x3001001C
	lw t1, 0(t0)

    srli a0, t1, 24
	jal ra, sendbyte

    srli a0, t1, 16
	jal ra, sendbyte

    srli a0, t1, 8
	jal ra, sendbyte

    srli a0, t1, 0
	jal ra, sendbyte


    li t0, 0x30010020
	lw t1, 0(t0)

    srli a0, t1, 24
	jal ra, sendbyte

    srli a0, t1, 16
	jal ra, sendbyte

    srli a0, t1, 8
	jal ra, sendbyte

    srli a0, t1, 0
	jal ra, sendbyte


    li t0, 0x30010024
	lw t1, 0(t0)

    srli a0, t1, 24
	jal ra, sendbyte

    srli a0, t1, 16
	jal ra, sendbyte

    srli a0, t1, 8
	jal ra, sendbyte

    srli a0, t1, 0
	jal ra, sendbyte


    li t0, 0x30010028
	lw t1, 0(t0)

    srli a0, t1, 24
	jal ra, sendbyte

    srli a0, t1, 16
	jal ra, sendbyte

    srli a0, t1, 8
	jal ra, sendbyte

    srli a0, t1, 0
	jal ra, sendbyte


    li t0, 0x3001002C
	lw t1, 0(t0)

    srli a0, t1, 24
	jal ra, sendbyte

    srli a0, t1, 16
	jal ra, sendbyte

    srli a0, t1, 8
	jal ra, sendbyte

    srli a0, t1, 0
	jal ra, sendbyte


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
