const lib = require("./libm.js")

lib.func();

print("libm->var:" + lib.vars );
lib.vars = 456;

const lib_ = require("./libm.js")
print("cache library")
lib_.func();
print("lib_->var:" + lib_.vars );

"ok"