import serial
import time

class DUT:
    def __init__(self, port, baud=115200):
        self.s = serial.Serial(port, baud, timeout=1)
        self.set_reset(1)
        
    def set_reset(self, rst):
        if rst:
            self.s.write(b"R1")
        else:
            self.s.write(b"R0")
        
        out = self.s.read(1)
        assert len(out) == 1 and out[0] == ord("S")
        
    def enable_for_millis(self, ms):
        self.s.write(bytearray([
            ord("T"), ms & 0xFF, (ms >> 8) & 0xFF,
            (ms >> 16) & 0xFF, (ms >> 24) & 0xFF]))
        
        out = self.s.read(1)
        assert len(out) == 1 and out[0] == ord("A")
        
        start_time = time.time()
        while True:
            assert (time.time() - start_time) < (ms / 1000 + 2.0)
            out = self.s.read(1)
            if len(out) != 0:
                break
                
        assert len(out) == 1 and out[0] == ord("S")
        
    def set_clkdiv(self, div):
        self.s.write(bytearray([ord("C"), div & 0xFF, (div >> 8) & 0xFF]))
        
        out = self.s.read(1)
        assert len(out) == 1 and out[0] == ord("S")
        
    def set_caravel_voltage(self, v):
        self.s.write(bytearray([ord("V"), v & 0xFF, (v >> 8) & 0xFF]))
        
        out = self.s.read(1)
        assert len(out) == 1 and out[0] == ord("S")
        
    def set_project_voltage(self, v):
        self.s.write(bytearray([ord("P"), v & 0xFF, (v >> 8) & 0xFF]))
        
        out = self.s.read(1)
        assert len(out) == 1 and out[0] == ord("S")
        
    def reset_ctr(self):
        self.s.write(b"E")
        
        out = self.s.read(1)
        assert len(out) == 1 and out[0] == ord("S")
        
    def get_ctr(self):
        self.s.write(b"W")
        
        out = self.s.read(5)
        assert len(out) == 5 and out[0] == ord("S")
        
        return out[1] | (out[2] << 8) | (out[3] << 16) | (out[4] << 24)

    def reset_dc(self):
        self.s.write(b"I")
        
        out = self.s.read(1)
        assert len(out) == 1 and out[0] == ord("S")
    
    def get_dc_len(self):
        self.s.write(b"O")
        
        out = self.s.read(5)
        assert len(out) == 5 and out[0] == ord("S")
        
        return out[1] | (out[2] << 8) | (out[3] << 16) | (out[4] << 24)

    def get_dc_buf(self):
        self.s.write(b"U")
        
        out = self.s.read(5)
        assert len(out) == 5 and out[0] == ord("S")
        
        length = (out[1] | (out[2] << 8) | (out[3] << 16) | (out[4] << 24))
        
        out = self.s.read(length)
        assert len(out) == length
        
        return bytes(out)
    
    def read_flash(self, addr, length):
        assert length < 8192
        
        self.s.write(bytearray([
            ord("J"),
            addr & 0xFF, (addr >> 8) & 0xFF,
            (addr >> 16) & 0xFF, (addr >> 24) & 0xFF,
            length & 0xFF, (length >> 8) & 0xFF,
            (length >> 16) & 0xFF, (length >> 24) & 0xFF]))
        
        out = self.s.read(1+length)
        assert len(out) == 1+length and out[0] == ord("S")
        return bytearray(out[1:])
        
    def write_flash(self, addr, length, buf):
        assert length % 4096 == 0
        
        for i in range(0, length, 4096):
            a = addr + i
            b = buf[i:i+4096]
            assert len(b) == 4096
            
            self.s.write(bytearray([
                ord("K"),
                a & 0xFF, (a >> 8) & 0xFF,
                (a >> 16) & 0xFF, (a >> 24) & 0xFF]))
            
            self.s.write(bytearray(b))
            
            out = self.s.read(2)
            assert len(out) == 2 and out[0] == ord("A") and out[1] == ord("S")
        
        