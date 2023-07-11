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
namespace add_msg
{
    typedef struct parameter{
        int a;
        int b;
    }PARAMS;
    void from_json(const json &j, PARAMS &p)
    {
        j.at("a").get_to(p.a);
        j.at("b").get_to(p.b);
    }

    void to_json(json &j, const PARAMS &p)
    {
        j = json{{"a", p.a}, {"b", p.b}};
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

int add (int a, int b);
void add(char* buff)
{
    string s = buff;
    auto j = json::parse(s);
    cout << "json:"<<j.dump() << endl;
    add_msg::MSG msg = j;
    msg.ret = add(msg.params.a, msg.params.b);
    cout << "msg.ret:" << msg.ret << endl;
    json m = msg;
    string l = m.dump();
    bzero(buff, BUFFSIZE);
    strncpy(buff, l.c_str(), BUFFSIZE);
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
    void from_json(const json &j, PARAMS &p)
    {
        j.at("a").get_to(p.a);
        j.at("b").get_to(p.b);
    }

    void to_json(json &j, const PARAMS &p)
    {
        j = json{{"a", p.a}, {"b", p.b}};
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

int sub (int a, int b);
void sub(char* buff)
{
    string s = buff;
    auto j = json::parse(s);
    sub_msg::MSG msg = j;
    msg.ret = sub(msg.params.a, msg.params.b);
    json m = msg;
    string l = m.dump();
    bzero(buff, BUFFSIZE);
    strncpy(buff, l.c_str(), BUFFSIZE);
}


int sub (int a, int b)
{
    //TO_DO::
    return a-b;
}


