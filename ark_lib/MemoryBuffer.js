const BitsField = require('./BitsField');
class MemoryBuffer{
    #buffer;
    #address;
    #size;
    #loaded;
    // 8字节对齐
    constructor(address, size) {
        this.#address = address;
        this.#size = size;
        this.#loaded = false;
    }
    load(){
        if( !this.#loaded ){
            this.#buffer = Memory.alloc( (size % 8) != 0 ? Math.ceil(size/8) * 8 : size );
            Memory.read(this.#buffer, address, size);
            this.#loaded = true;
        }
        return this;
    }

    write(array, length = this.#size, offset = 0) {
        this.load();
        Memory.buffer(this.#address, array);
        return this;
    }

    read(length = this.#size, offset = 0){
        this.load();
        return Memory.buffer(this.#address, this.#size);
    }

    // 析构函数(需要手动释放)
    destructor(){
        Memory.free(this.#address);
    }

    address(){
        return this.#buffer;
    }

    pointer(offset = 0){
        this.load();
        return Memory.u64(this.#address + offset);
    }

    int(offset = 0){
        this.load();
        return Memory.u32(this.#address + offset);
    }

    int64(offset = 0){
        this.load();
        return Memory.u64(this.#address + offset);
    }

    float(offset = 0){
        this.load();
        return Memory.f32(this.#address + offset);
    }

    double(offset = 0){
        this.load();
        return Memory.f64(this.#address + offset);
    }

    // 输入输出默认utf8
    string(offset = 0){
        this.load();
        return Memory.string(this.#address + offset);
    }

    // 根据数据长度读取值并生成位操作对象(最大8字节)
    bits(offset = 0){
        this.load();
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
