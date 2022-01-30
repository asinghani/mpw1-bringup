import signal
import time

# https://stackoverflow.com/questions/842557/how-to-prevent-a-block-of-code-from-being-interrupted-by-keyboardinterrupt-in-py
# allows marking critical section to avoid breaking communication protocol
# when using it inside a Jupyter notebook
class CriticalSection:
    def __enter__(self):
        self.signal_received = False
        self.old_handler = signal.signal(signal.SIGINT, self.handler)
                
    def handler(self, sig, frame):
        self.signal_received = (sig, frame)
        print("delayed SIGINT")
    
    def __exit__(self, type, value, traceback):
        signal.signal(signal.SIGINT, self.old_handler)
        if self.signal_received:
            self.old_handler(*self.signal_received)
            
            
def millis():
    return int(time.time() * 1000)

def sleep(ms):
    time.sleep(ms / 1000.0)