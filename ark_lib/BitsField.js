class BitsField{
    #value;
    constructor( init ){
        this.#value = init;
    }

    #getBitsBlock(offset, size){
        const tail = BigInt(size%4);
        const cnt = BigInt(Math.floor(size/4));
        let v = 0n;
        for(let i =0n; i <cnt; i++){
            v += (0xfn<<(4n * i));
        }
        let t = 0n;
        for(let i =0n; i<tail; i++){
            t += (1n<<i);
        }
        return ((v<<tail) + t)<<BigInt(offset);
    }

    // 按偏移和长度,读位值
    rd(offset, length){
        return this.#getBitsBlock(offset, length) & this.#value;
    }

    // 按偏移和长度,设置位值
    st(offset, length){
        this.#value = this.#getBitsBlock(offset, length) | this.#value;
    }

    // 按偏移和长度,清除位值
    cl(offset, length){
        this.#value = (~this.#getBitsBlock(offset, length)) & this.#value;
    }
    val(){
        return this.#value;
    }
}

BitsField;
