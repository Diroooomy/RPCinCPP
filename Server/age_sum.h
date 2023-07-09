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
typedef struct var1
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
int age_sum (user user1, user user2);
void age_sum(char* buff)
{
    age_sum_msg::MSG *msg = (age_sum_msg::MSG *)buff;
    msg->ret = age_sum(msg->params.user1, msg->params.user2);
}


int age_sum (user user1, user user2)
{
    //TO_DO::
    return user1.age+user2.age;
}


