const BitsField = require('./BitsField.js');
class MemoryBuffer{
    #buffer;
    #address;
    #size;
    // 8字节对齐
    constructor(address, size) {
        this.#address = address;
        this.#size = size;
    }
    write(array, offset = 0n) {
        Memory.buffer(this.#address + offset, array);
        return this;
    }

    read(length = this.#size, offset = 0n){
        return Memory.buffer(this.#address, this.#size);
    }

    address(){
        return this.#buffer;
    }

    pointer(offset = 0n){
        return Memory.u64(this.#address + offset);
    }

    int(offset = 0n){
        return Memory.u32(this.#address + offset);
    }

    int64(offset = 0n){
        return Memory.u64(this.#address + offset);
    }

    float(offset = 0n){
        return Memory.f32(this.#address + offset);
    }

    double(offset = 0n){
        return Memory.f64(this.#address + offset);
    }

    // 输入输出默认utf8
    string(offset = 0n){
        return Memory.string(this.#address + offset);
    }
    // 输入输出 unicode
    unicode(offset = 0n){
        return Memory.unicode(this.#address + (offset*2));
    }
    // 根据数据长度读取值并生成位操作对象(最大8字节)
    bits(offset = 0n){
        const l = this.#size > 8 ? 8 : this.#size;
        return new BitsField(this.int64(offset));
    }
}

MemoryBuffer.from = function(address, size){
    return new MemoryBuffer(address, size);
}

MemoryBuffer.fromRva = function(name, offset, size){
    let address = findDriver(name) + offset;
    return new MemoryBuffer(address, size);
}

MemoryBuffer;
