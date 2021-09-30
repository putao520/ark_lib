const address = Memory._new(0x1000);
print("alloc:" + Number(address).toString(16));

print("test string")
Memory.string(address, "putao520和葡萄520");
const str = Memory.string(address);
print("string:" + str);

print("test unicode")
Memory.unicode(address, "putao520和葡萄520");
const uni = Memory.unicode(address);
print("unicode:" + uni);

Memory._delete(address);
"ok"