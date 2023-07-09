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

// #define BUFFSIZE 2048
// #define SERVER_IP "127.0.0.1" // 指定服务端的IP，记得修改为你的服务端所在的ip
// #define SERVER_PORT 16555     // 指定服务端的port
using namespace std;
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

int add (int a, int b);

void add(char *buff)
{

    // bzero(buff, BUFFSIZE);
    // cout << buff << endl;
    add_msg::MSG *msg = (add_msg::MSG *)buff;
    // bzero(buff, buffsize);
    // char *newStr = new char[strlen(buff)];
    // strcpy(newStr, buff);
    // cout << sizeof(buff) << endl;
    // cout << sizeof(newStr) << endl;  
    // cout << newStr << endl;
    // /*
    // {
    //     //TO_DO::
    // }
    // */
    // add_msg::MSG *msg = (add_msg::MSG *)newStr;
    // cout << msg->params.a << endl;
    // msg->ret = msg->params.a + msg->params.b;
    // //    cout << msg->ret <<endl;
    // memcpy(buff, &msg, sizeof(add_msg::MSG));
    // return buff;
    msg->ret = add(msg->params.a, msg->params.b);
    cout << "ret:" << msg->ret << endl;
    // memcpy(temp, msg, sizeof(msg));
    // cout << sizeof(temp) <<endl;
    // MSG *msg1 = (MSG *)temp;
    // cout<< "ret:"  << msg1->ret<<endl;
    // return temp;
}


int add (int a, int b) 
{
    //TO_DO::
    return a+b;
}