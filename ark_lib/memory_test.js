print("chinese中文cn");

const address = Memory.alloc(0x1000);
print("alloc:" + Number(address).toString(16));

print("test u32")
Memory.u32(address, 123);
const v1 = Memory.u32(address);
print(`return:${v1}/123`);

print("test u64")
Memory.u64(address, 0xffffffff11111111n);
const v2 = Memory.u64(address);
print(`return:${BigInt(v2).toString(16)}/0xffffffff11111111`);

print("test f32")
Memory.f32(address, 0.123);
const v3 = Memory.f32(address);
print(`return:${v3}/0.123`);

print("test f64")
Memory.f64(address, 0.123456);
const v4 = Memory.f64(address);
print(`return:${v4}/0.123456`);

print("test u8array")
const s = new Set([1, 2, 3]);
const u8array = Uint8Array.from(s);
Memory.buffer(address, u8array);
const arr = Memory.u8Array(address, 3);
var eArray = arr.entries();
for( let n of eArray ){
    print(n);
}

print("test string")
Memory.string(address, "putao520和string葡萄520");
const str = Memory.string(address);
print("string:" + str);

print("test unicode")
Memory.unicode(address, "putao520和unicode葡萄520");
const uni = Memory.unicode(address);
print("unicode:" + uni);

print("free:" + Number(address).toString(16));
Memory.free(address);

print("hal.dll:" + Number(findDriver("hal.dll")).toString(16));
 
// procedure(address, arg_0, arg_1, arg_2,)

print("test crash")
Memory.f32('to crash');
"ok";