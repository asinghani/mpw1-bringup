import os

NOP = [0x13, 0x00, 0x00, 0x00]
INF_LOOP = [0x6F, 0x00, 0x00, 0x00]

def prep_flash(dat):
    dat = bytearray(dat)
    while len(dat) % 4096 != 0:
        dat = dat + bytearray([0xFF])
        
    return dat

ASM_CMD = "riscv-none-elf-gcc -march=rv32i -mabi=ilp32 -ffreestanding -nostartfiles -Wl,-Bstatic,--strip-debug"

def assemble_file(filename):
    cmd_build = f"{ASM_CMD} -o /tmp/rvbuild.elf {filename}"
    cmd_copy = f"riscv-none-elf-objcopy --only-section=.text /tmp/rvbuild.elf -O binary /tmp/rvbuild.bin"
    
    os.system(cmd_build)
    os.system(cmd_copy)
    
    with open("/tmp/rvbuild.bin", "rb") as f:
        dat = f.read()
        
    return dat

