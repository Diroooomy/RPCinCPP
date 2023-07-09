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
#include "socket.h"
#define BUFFSIZE 2048
using namespace std;
namespace add_msg
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
int add (int a, int b) {
    add_msg::PARAMS params;
    params.a = a;
    params.b = b;
    add_msg::MSG msg;
    strcpy(msg.funcname, "add");
    msg.params = params;
    pair<string, int> p = loadEnv("server.env");
    int sockfd = socketConnect(p.first.c_str(), p.second);
    char buff[BUFFSIZE];
    memcpy(buff, &msg, sizeof(add_msg::message));
    send(sockfd, buff, sizeof(buff), 0);
    bzero(buff, sizeof(buff));
    recv(sockfd, buff, BUFFSIZE - 1, 0);
    add_msg::MSG *res = (add_msg::MSG *)buff;
    close(sockfd);
    return res->ret;
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
int sub (int a, int b) {
    sub_msg::PARAMS params;
    params.a = a;
    params.b = b;
    sub_msg::MSG msg;
    strcpy(msg.funcname, "sub");
    msg.params = params;
    pair<string, int> p = loadEnv("server.env");
    int sockfd = socketConnect(p.first.c_str(), p.second);
    char buff[BUFFSIZE];
    memcpy(buff, &msg, sizeof(sub_msg::message));
    send(sockfd, buff, sizeof(buff), 0);
    bzero(buff, sizeof(buff));
    recv(sockfd, buff, BUFFSIZE - 1, 0);
    sub_msg::MSG *res = (sub_msg::MSG *)buff;
    close(sockfd);
    return res->ret;
}



