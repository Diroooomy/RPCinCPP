#pragma once
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
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
using namespace std;

pair<string, int> loadEnv(const char *filepath)
{
    ifstream infile; // 定义文件变量
    pair<string, int> p;
    infile.open(filepath, ios::in);
    if (!infile)
    {
        cout << "filepath not exist!" << endl;
        // exit(0);
    }
    string line;
    while (getline(infile, line))
    {
        size_t size = line.size();
        size_t pos = line.find(":");
        string a = line.substr(0, pos);
        string b = line.substr(pos + 1, size);
        if (a == "SERVER_IP")
        {
            p.first = b;
            // cout << a<< endl;
        }
        else if (a == "SERVER_PORT")
        {
            int num;
            sscanf(b.c_str(), "%d", &num);
            p.second = num;
        }
    }
    infile.close();
    return p;
}

int socketConnect(const char *SERVER_IP, int SERVER_PORT, int __domain = AF_INET, int __type = SOCK_STREAM, int __protocol = 0)
{
    struct sockaddr_in servaddr;

    int sockfd;
    sockfd = socket(__domain, __type, __protocol);
    if (-1 == sockfd)
    {
        printf("Create socket error(%d): %s\n", errno, strerror(errno));
        return -1;
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr);
    servaddr.sin_port = htons(SERVER_PORT);
    if (-1 == connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        printf("Connect error(%d): %s\n", errno, strerror(errno));
        return -1;
    }
    return sockfd;
}

// int main()
// {

//     pair<string, int> p = loadEnv("server.env");
//     cout << p.first << endl << p.second << endl;
//     int sockfd = socketConnect(p.first.c_str(), p.second);
//     return 0;
// }