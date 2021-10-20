const MemoryBuffer = require("./MemoryBuffer.js");
const ntoskrnl = require("./ntoskrnl.exe.js");

const eprocess_ptr = Memory._new(0x8);
print(`eprocess_ptr:${BigInt(eprocess_ptr).toString(16)}`);
ntoskrnl.functional.PsLookupProcessByProcessId(4672, eprocess_ptr);

const peprocess = Memory.u64(eprocess_ptr);
print(`peprocess:${BigInt(peprocess).toString(16)}`);

const eprocess = new ntoskrnl.structure._EPROCESS(peprocess);
print("struct loading");

const name_array_buffer = eprocess.ImageFileName(0, 14);
const image_name = String.fromCharCode.apply(null, new Uint16Array(name_array_buffer));
print(`image_name:${image_name}`);

Memory._delete(eprocess_ptr);
("ok");

