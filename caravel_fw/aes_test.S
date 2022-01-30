.section .text.start
.global _start

_start:
    # Calibration byte
    li a0, 0xAA
    call sendbyte

	li t0, 0x30010000

    li t1, 1
    sw t1, 0(t0)
