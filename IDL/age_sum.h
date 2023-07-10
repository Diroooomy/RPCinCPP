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
#include "json.hpp"
#define BUFFSIZE 2048
using json = nlohmann::json;
using namespace std;
typedef struct age_sum_var1
{
    char name[20];
    int age;
}user;


namespace age_sum_msg
{
    typedef struct parameter{
        user user1;
        user user2;
    }PARAMS;
    typedef struct message{
        char funcname[20];
        int ret;
        PARAMS params;
    }MSG;
}
int age_sum (user user1, user user2) {
    age_sum_msg::PARAMS params;
    params.user1 = user1;
    params.user2 = user2;
    age_sum_msg::MSG msg;
    strcpy(msg.funcname, "age_sum");
    msg.params = params;
    pair<string, int> p = loadEnv("server.env");
    int sockfd = socketConnect(p.first.c_str(), p.second);
    char buff[BUFFSIZE];
    memcpy(buff, &msg, sizeof(age_sum_msg::message));
    send(sockfd, buff, sizeof(buff), 0);
    bzero(buff, sizeof(buff));
    recv(sockfd, buff, BUFFSIZE - 1, 0);
    age_sum_msg::MSG *res = (age_sum_msg::MSG *)buff;
    close(sockfd);
    return res->ret;
}


