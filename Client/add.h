#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "socket.h"
#define BUFFSIZE 2048
#define SERVER_IP "127.0.0.1" // 指定服务端的IP，记得修改为你的服务端所在的ip
#define SERVER_PORT 16555     // 指定服务端的port
namespace add_msg
{
    typedef struct parameter
    {
        int a;
        int b;
    } PARAMS;
    typedef struct message
    {
        string funcname;
        int ret;
        PARAMS params;
    } MSG;
}
int add(int a, int b)
{
    
    
    add_msg::PARAMS params;
    params.a = a;
    params.b = b;
    add_msg::MSG msg;
    msg.funcname = "add";
    msg.params = params;

    pair<string, int> p = loadEnv("server.env");
    int sockfd = socketConnect(p.first.c_str(), p.second);
    char buff[BUFFSIZE];
    
    send(sockfd, msg.funcname.c_str(), msg.funcname.length(), 0);
    memcpy(buff, &msg, sizeof(add_msg::message));
    sleep(0.5);
    send(sockfd, buff, sizeof(buff), 0);

    // send(sockfd, buff, strlen(buff), 0);
    bzero(buff, sizeof(buff));
    recv(sockfd, buff, BUFFSIZE - 1, 0);
    // int ret;
    // memcpy(&ret, buff, sizeof(buff));
    // cout << sizeof(buff) << endl;
    add_msg::MSG *res = (add_msg::MSG *)buff;

    cout << res->params.a << endl;
    // printf("Recv: %d\n", ret);
    close(sockfd);
    return res->ret;
}
