const MemoryBuffer = require("./MemoryBuffer.js");
const ntoskrnl = require("./ntoskrnl.exe.js");

const eprocess_ptr = memory.alloc(0x1000)
ntoskrnl.funcs.PsLookupProcessByProcessId(123, eprocess_ptr);

const eprocess = ntoskrnl.structs._EPROCESS(eprocess_ptr);
const name_buffer = eprocess.ImageFileName(0,14);
const image_name = name_buffer.string();
print(image_name);
memory.free(eprocess_ptr);
"ok"