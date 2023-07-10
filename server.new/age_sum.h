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
#include "json.hpp"
#define BUFFSIZE 2048
using json = nlohmann::json;
using namespace std;
typedef struct age_sum_var1
{
    char name[20];
    int age;
}user;


void from_json(const json &j, user &p)
{    
    string s;
    j.at("name").get_to(s);
    memcpy(p.name, s.c_str(), sizeof(p.name));
    j.at("age").get_to(p.age);
}
void to_json(json &j, const user &p)
{
    j = json{{"name", p.name}, {"age", p.age}};
}
namespace age_sum_msg
{
    typedef struct parameter{
        user user1;
        user user2;
    }PARAMS;
    void from_json(const json &j, PARAMS &p)
    {
        j.at("user1").get_to(p.user1);
        j.at("user2").get_to(p.user2);
    }

    void to_json(json &j, const PARAMS &p)
    {
        j = json{{"user1", p.user1}, {"user2", p.user2}};
    }

    typedef struct message{
        char funcname[20];
        int ret;
        PARAMS params;
    } MSG;
    void from_json(const json &j, MSG &p)
    {
        string s;
        j.at("funcname").get_to(s);
        memcpy(p.funcname, s.c_str(), sizeof(p.funcname));
        j.at("ret").get_to(p.ret);
        j.at("params").get_to(p.params);
    }

    void to_json(json &j, const MSG &p)
    {
        j = json{{"funcname", p.funcname}, {"ret", p.ret}, {"params", p.params}};
    }

}

int age_sum (user user1, user user2);
void age_sum(char* buff)
{
    string s = buff;
    auto j = json::parse(s);
    age_sum_msg::MSG msg = j;
    msg.ret = age_sum(msg.params.user1, msg.params.user2);
    json m = msg;
    string l = m.dump();
    bzero(buff, BUFFSIZE);
    strncpy(buff, l.c_str(), BUFFSIZE);
}


int age_sum (user user1, user user2)
{
    //TO_DO::
    return user1.age+user2.age;
}


