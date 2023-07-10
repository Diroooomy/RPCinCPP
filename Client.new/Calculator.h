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
    }MSG;
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
    json j = msg;
    string s = j.dump();
    memcpy(buff, s.c_str(), BUFFSIZE-1);
    send(sockfd, buff, sizeof(buff), 0);
    bzero(buff, sizeof(buff));
    recv(sockfd, buff, BUFFSIZE - 1, 0);
    auto ret = json::parse(buff);
    add_msg::MSG res = ret;
    close(sockfd);
    return res.ret;
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
    }MSG;
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
    json j = msg;
    string s = j.dump();
    memcpy(buff, s.c_str(), BUFFSIZE-1);
    send(sockfd, buff, sizeof(buff), 0);
    bzero(buff, sizeof(buff));
    recv(sockfd, buff, BUFFSIZE - 1, 0);
    auto ret = json::parse(buff);
    sub_msg::MSG res = ret;
    close(sockfd);
    return res.ret;
}


