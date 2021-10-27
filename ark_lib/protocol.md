### 基本通讯协议
```
# 基本包结构
{
	id;string
	header:{
		json
	},
	path:string,
	payload{		// 请求时为参数,返回时为结果
		any
	}
}

# 返回包结构
{
	code:int,
	message:string,
	data:any
}
```