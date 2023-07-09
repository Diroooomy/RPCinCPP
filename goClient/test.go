package main

import (
	// "bufio"
	"fmt"
	"net"
	"bytes"
    // "encoding/binary"
	
	"encoding/gob"
)


type PARAMS struct {
    a int 
    b int 
};
type MSG struct{
    // funcname [20] byte
    ret int 
    params PARAMS 
}

func main() {

    // 创建一个新的结构体
//     fmt.Println(Books{"Go 语言", "www.runoob.com", "Go 语言教程", 6495407})

//     // 也可以使用 key => value 格式
//     fmt.Println(Books{title: "Go 语言", author: "www.runoob.com", subject: "Go 语言教程", book_id: 6495407})

//     // 忽略的字段为 0 或 空
//    fmt.Println(Books{title: "Go 语言", author: "www.runoob.com"})
   // 建立服务
	
    conn, err := net.Dial("tcp", "127.0.0.1:16555")
	if err != nil {
		fmt.Printf("listen udp server error:%v\n", err)
	}
	defer conn.Close()

	
	sendData := []byte("add")
	_, err = conn.Write(sendData) // 发送数据
	if err != nil {
		fmt.Println("发送数据失败，err:", err)
		return
	}
// 发送数据
    var params PARAMS
    params.a = 1
    params.b = 2
    var msg MSG
	// s1 := "add"
    // msg.funcname = []byte(s1)
    msg.ret = 0
    msg.params = params
	buf := new(bytes.Buffer)
	
	enc := gob.NewEncoder(buf)
	if err := enc.Encode(msg); err != nil {
		fmt.Println(err)
	}

	_, err = conn.Write(buf.Bytes()) // 发送数据
	// // 接收数据
	// data := make([]byte, 4096)
	// n, remoteAddr, err := listen.ReadFromUDP(data) // 接收数据
	// if err != nil {
	// 	fmt.Println("接收数据失败，err:", err)
	// 	return
	// }
	// fmt.Printf("recv:%v addr:%v count:%v\n", string(data[:n]), remoteAddr, n)
// read or write on conn
}