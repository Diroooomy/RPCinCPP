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
        // cout << "s:" <<s <<endl;
        j.at("ret").get_to(p.ret);
        from_json(j.at("params"), p.params);
    }
    void to_json(json &j, const MSG &s)
    {
        j = json{{"funcname", s.funcname}};
        j["ret"] = s.ret;
        // json t;
        to_json(j["params"], s.params);
        // j["params"] = t;
        
    }

}


int add(int a, int b)
{
    add_msg::PARAMS params;
    params.a = a;
    params.b = b;
    add_msg::MSG msg;
    
    // strcpy(msg.funcname, "add");
    msg.funcname = "add";
    msg.params = params;

    
    json j;
    to_json(j, msg);
    
    string s = j.dump();
    cout << s <<endl;


    pair<string, int> p = loadEnv("server.env");
    int sockfd = socketConnect(p.first.c_str(), p.second);
    send(sockfd, "add", sizeof("add"), 0);
    sleep(1);
    char buff[BUFFSIZE];
    
    // cout << s.c_str() << endl;
    memcpy(buff, s.c_str(), BUFFSIZE - 1);
    // cout << "buff:" << buff << endl;
    int size = send(sockfd, buff, sizeof(buff), 0);

    bzero(buff, sizeof(buff));
    recv(sockfd, buff, BUFFSIZE - 1, 0);
    cout <<"buff:"<< buff << endl;
    // string r = buff;
    auto ret = json::parse(buff);
    add_msg::MSG res;
    add_msg::from_json(ret, res);
    close(sockfd);
    return res.ret;
}

namespace sub_msg
{
    typedef struct parameter
    {
        int a;
        int b;
    } PARAMS;
    typedef struct message
    {
        char funcname[20];
        int ret;
        PARAMS params;
    } MSG;
}
int sub(int a, int b)
{
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
