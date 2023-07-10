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
#define BUFFSIZE 2048
#include "json.hpp"
using json = nlohmann::json;

using namespace std;
namespace add_msg
{
    typedef struct parameter
    {
        int a;
        int b;
    } PARAMS;

    void from_json(const json &j, PARAMS &p)
    {
        j.at("a").get_to(p.a);
        j.at("b").get_to(p.b);
    }
    void to_json(json &j, const PARAMS &p)
    {
        j = json{{"a", p.a}, {"b", p.b}};
    }

    typedef struct message
    {
        string funcname;
        int ret;
        PARAMS params;
    } MSG;

    void from_json(const json &j, MSG &p)
    {
        string s;
        j.at("funcname").get_to(p.funcname);
        // memcpy(p.funcname, s.c_str(), sizeof(p.funcname));
        j.at("ret").get_to(p.ret);
        from_json(j.at("params"), p.params);

    }
    void to_json(json &j, const MSG &s)
    {
        j = json{{"funcname", s.funcname}};
        j["ret"] = s.ret;
        // cout <<"ret:"<< s.ret << endl;
        json t;
        to_json(t, s.params);
        j["params"] = t;
    }
}
int add (int a, int b);
void add(char* buff)
{
    
    // cout <<sizeof(buff) << endl;
    string s = buff;
    auto j = json::parse(s);
    add_msg::MSG msg;
    add_msg::from_json(j, msg);
    msg.ret = add(msg.params.a, msg.params.b);
    cout << msg.ret << endl;
    json m;
    add_msg::to_json(m, msg);
    string l = m.dump();
    cout << l << endl;
    bzero(buff, BUFFSIZE);
    
    // cout << sizeof(buff) << endl;
    strncpy(buff, l.c_str(), BUFFSIZE);
    cout <<"buff:"<< buff << endl;
    // cout << msg->ret << endl;
    
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


