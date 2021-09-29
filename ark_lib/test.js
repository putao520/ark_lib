const address = Memory.alloc(0x10);
print("alloc:" + address);
Memory.u32(address, 123);
const v = Memory.u32(address);
print("value:" + v);
"ok";
