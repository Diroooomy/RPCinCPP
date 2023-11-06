use C/C++ correctly is a really difficult thing

带new的文件夹是使用了json序列化数据传输网络消息的代码
不带new的是普通的将数据转为char*正常发送

newIDL加入了使用json后的新存根文件的生成方式，该改变是不可逆的
可使用旧的IDL生成旧的存根文件使用

客户端和服务端的存根文件都是通过IDLtoCPP自动生成，只需要填写服务端存根文件的TO_DO项，即函数实体，再到server.cpp的服务注册模块中注册函数，函数标识为函数名，应该与函数实体的名字一致
