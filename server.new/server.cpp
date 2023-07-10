#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <iostream>
#include "util.h"
#include "Calculator.h"
#include "age_sum.h"
#include "json.hpp"
using json = nlohmann::json;
#define BUFFSIZE 2048
#define DEFAULT_PORT 16556 // 指定端口为16555
#define MAXLINK 2048

int sockfd, connfd; // 定义服务端套接字和客户端套接字

using namespace std;
void stopServerRunning(int p)
{
    close(sockfd);
    printf("Close Server\n");
    exit(0);
}

int main()
{
    sockfd = socketBind(DEFAULT_PORT);
    // struct sockaddr_in servaddr; // 用于存放ip和端口的结构
    char buff[BUFFSIZE]; // 用于收发数据
    // char func[20];//用于解析数据中前20个字节，即函数名

    while (true)
    {
        signal(SIGINT, stopServerRunning); // 这句用于在输入Ctrl+C的时候关闭服务器
        connfd = accept(sockfd, NULL, NULL);
        if (-1 == connfd)
        {
            printf("Accept error(%d): %s\n", errno, strerror(errno));
            return -1;
        }

        bzero(buff, BUFFSIZE);
        recv(connfd, buff, BUFFSIZE-1, 0);  //接收消息
        // std::string funcname = buff;
        json j = json::parse(buff);
        string funcname = j.at("funcname");
        std::cout << buff << std::endl; // 服务名识别
        // sleep(1);
        // bzero(buff, BUFFSIZE);
        // recv(connfd, buff, BUFFSIZE - 1, 0);
        // strncpy(func, buff, 20);
        // string funcname = func;
        // cout << "func:" << funcname << endl;
        

        // 服务注册块
        switch (hash_(funcname.c_str()))
        {
        case "add"_hash:
        {
            cout << "run add function" << endl;
            // char temp[BUFFSIZE];
            add(buff);
            // cout << msg->ret << endl;
            // cout << sizeof(buff) << endl;
            send(connfd, buff, sizeof(buff), 0);
            // string s = buff;
            // cout <<"funcname:" <<  s << endl;
            break;
        }
        case "sub"_hash:
        {
            cout << "run sub function" << endl;
            sub(buff);
            send(connfd, buff, sizeof(buff), 0);
            break;
        }
        case "age_sum"_hash:
        {
            // typedef struct var1
            // {
            //     char name[20];
            //     int age;
            // } user;
            // typedef struct parameter
            // {
            //     user user1;
            //     user user2;
            // } PARAMS;
            // typedef struct message
            // {
            //     char funcname[20];
            //     int ret;
            //     PARAMS params;
            // } MSG;
            
            // cout << buff << endl;
            // MSG *msg = (MSG *)buff;
            cout << "run age_sum function" << endl;
            // cout << msg->ret << endl;
            // cout << msg->params.user1.name << endl;
            // cout << msg->funcname << endl;
            // age_sum_msg::parameter::user user1 = msg->params.user1;
            age_sum(buff);
            // msg->ret = msg->params.user1.age+msg->params.user2.age;
            // cout << msg->ret << endl;
            send(connfd, buff, sizeof(buff), 0);
            break;
        }

        default:
            cout << "error" << endl;
            break;
        }
        // 与函数相关
        // message *msg = (message *)buff;
        // int ret = msg->params.a + msg->params.b;
        // std::cout << ret << std::endl;
        // // printf("%s \n", msg->client_name);
        // // printf("%s \n", msg->buffer);
        // // std::cout<< msg->t.c<<" "<<msg->t.d << std::endl;
        // memcpy(buff, &ret, sizeof(int));
        // send(connfd, buff, strlen(buff), 0);
        // end
        close(connfd);
    }
    return 0;
}
