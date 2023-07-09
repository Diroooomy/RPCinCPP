#pragma once
#include <string>
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
using namespace std;
namespace add_msg
{
    typedef struct parameter{
        int a;
        int b;
    }PARAMS;
    typedef struct message{
        // char funcname[20];
        int ret;
        PARAMS params;
    }MSG;
}
int add (int a, int b);
void add(char* buff)
{
    // cout << sizeof(add_msg::MSG) << endl;
    cout << buff << endl;
    add_msg::MSG *msg = (add_msg::MSG *)buff;
    msg->ret = add(msg->params.a, msg->params.b);
    cout << msg->ret << endl;
}


int add (int a, int b)
{
    //TO_DO::
    return a+b;
}


namespace sub_msg
{
    typedef struct parameter{
        int a;
        int b;
    }PARAMS;
    typedef struct message{
        char funcname[20];
        int ret;
        PARAMS params;
    }MSG;
}
int sub (int a, int b);
void sub(char* buff)
{
    sub_msg::MSG *msg = (sub_msg::MSG *)buff;
    msg->ret = sub(msg->params.a, msg->params.b);
}


int sub (int a, int b)
{
    //TO_DO::
    return a-b;
}


