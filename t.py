import ctypes
import os

class FST:
    def __init__(self):
        self.dll = ctypes.CDLL(os.getcwd() + os.sep + "fst.dll")

    def open(self,fn):
        self.dll.init(ctypes.c_char_p(bytes(fn,'utf-8')))

    def add_signal(self,name,bits):
        self.dll.add_signal(ctypes.c_char_p(bytes(name,'utf-8')),bits)

    def change_signal(self,name,val):
        self.dll.change_signal(ctypes.c_char_p(bytes(name,'utf-8')), val)

    def dump(self):
        self.dll.dump()

    def flush(self):
        self.dll.flush()
    
    def close(self):
        self.dll.close_wave()


if __name__ == '__main__':
    fst = FST()
    fst.open("t.fst")
    fst.add_signal("a",32)
    fst.add_signal("b",1)
    fst.add_signal("c",33)
    fst.add_signal("d",64)
    fst.dump()

    fst.change_signal("a",0x12345678)
    fst.change_signal("b",0)
    fst.change_signal("c",ctypes.c_uint64(0x1ffffffff))
    fst.change_signal("d",ctypes.c_uint64(0x8000000080000000))
    fst.dump()

    fst.change_signal("a",0x87654321)
    fst.change_signal("b",1)
    fst.change_signal("c",ctypes.c_uint64(0x0))
    fst.change_signal("d",ctypes.c_uint64(0xf0000000f0000000))
    fst.dump()

    fst.flush()
    fst.close()
