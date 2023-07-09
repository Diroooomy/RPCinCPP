#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
using namespace std;
constexpr std::uint32_t hash_str_to_uint32(const char *data)
{
    std::uint32_t h(0);
    for (int i = 0; data && ('\0' != data[i]); i++)
        h = (h << 6) ^ (h >> 26) ^ data[i];
    return h;
}

string &trim(string &s)
{
    if (!s.empty())
    {
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    }
    return s;
}

void TrimString(string &src, char c = ' ') // 默认删除前后空格
{
    if (src.empty())
        return;

    if (src[0] == c) // 判断第一个字符
    {
        src = src.substr(1);
    }

    if (!src.empty() && src[src.length() - 1] == c) // 判断最后一个字符
    {
        src = src.substr(0, src.length() - 1);
    }

    if (!src.empty() && (src[0] == c || src[src.length() - 1] == c)) // 如果字符串前后还有指定字符，刚递归删除即可
        TrimString(src, c);
}

string split(const string &str, const string &pattern) // 适用于linux下去除文件路径中文件夹名字的函数，只保留文件全名
{
    string res;
    if (str == "")
        return res;
    string strs = str;
    size_t pos = strs.find(pattern);

    while (pos != strs.npos)
    {

        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(pattern);
    }
    return strs;
}

int main(int argc, char *argv[])
{
    string input_file = "age_sum.idl";
    string type = "client";
    size_t pos = input_file.find('.');
    string temp = input_file.substr(0, pos);
    std::cout << temp << endl;
    string query;
    std::ifstream in(input_file);
    ofstream outfile;
    // outfile.open("Calculator.h");
    outfile.open(temp + ".h");
    outfile << "#pragma once" << endl
            << "#include <string>" << endl
            << "#include <cstdio>" << endl
            << "#include <cstring>" << endl
            << "#include <cstdlib>" << endl
            << "#include <sys/socket.h>" << endl
            << "#include <sys/unistd.h>" << endl
            << "#include <sys/types.h>" << endl
            << "#include <sys/errno.h>" << endl
            << "#include <netinet/in.h>" << endl
            << "#include <arpa/inet.h>" << endl
            << "#include <iostream>" << endl;
    if (type == "client" || type == "Client")
    {
        outfile << "#include \"socket.h\"" << endl;
        outfile << "#define BUFFSIZE 2048" << endl;
    }
    outfile << "using namespace std;" << endl;
    int flag = 0;
    int writefunc = 0;
    int writestruct = 0;
    string structname;
    int writeend = 0;
    int structnum = 0;
    while (getline(in, query))
    {
        if (query.empty())
            continue;
        flag++;
        vector<string> v;
        string t;
        stringstream ss(trim(query));
        string token;
        while (getline(ss, token, ' '))
        {
            v.push_back(token);
        }

        // cout << v[0] << endl;
        if (v[0] == "service")
        {

            writefunc = 1;
            continue;
        }
        else if (v[0] == "struct")
        {
            writestruct = 1;
            structname = v[1];
            structnum++;
            outfile << "typedef struct " << temp << "_var" << structnum << endl
                    << "{" << endl;
            continue;
        }
        // else {
        //     cout << "IDLfile syntax error in line "<< flag <<endl;
        // }
        if (writefunc == 1)
        {

            if (v[0] != "}")
            {

                if (v[0] != "{") // 构造函数
                {
                    string ret = v[0];                        // 函数返回类型
                    string funcname = v[1];                   // 函数名
                    if (type == "client" || type == "Client") // 构造客户端存根
                    {
                        // 定义参数结构体
                        outfile << "namespace " << funcname << "_msg" << endl
                                << "{" << endl;
                        outfile << "    typedef struct parameter{" << endl;
                        vector<pair<string, string>> params;
                        for (int i = 2; i < v.size(); i += 2)
                        {
                            TrimString(v[i], '(');
                            TrimString(v[i], ',');
                            TrimString(v[i + 1], ')');
                            TrimString(v[i + 1], ',');
                            pair<string, string> p;
                            p = make_pair(v[i], v[i + 1]);
                            params.push_back(p);
                            cout << v[i] << "    " << v[i + 1] << endl;
                            outfile << "        " << v[i] << " " << v[i + 1] << ";" << endl;
                        }
                        outfile << "    }PARAMS;" << endl;
                        // end

                        // 定义消息结构体
                        outfile << "    typedef struct message{" << endl;
                        outfile << "        char funcname[20];" << endl;
                        outfile << "        " << ret << " ret;" << endl;
                        outfile << "        "
                                << "PARAMS params;" << endl;
                        outfile << "    }MSG;" << endl
                                << "}" << endl;
                        // end

                        // 定义函数
                        outfile << trim(query) << " {" << endl;

                        // 构造消息
                        outfile << "    " << funcname << "_msg::PARAMS params;" << endl;
                        if (params.size())
                        {

                            for (int i = 0; i < params.size(); i++)
                            {
                                outfile << "    params." << params[i].second << " = " << params[i].second << ";" << endl;
                            }
                        }

                        outfile << "    " << funcname << "_msg::MSG msg;" << endl;
                        outfile << "    strcpy(msg.funcname, "
                                << "\"" << funcname << "\");" << endl;
                        outfile << "    msg.params = params;" << endl;
                        // end

                        // 连接服务器 发送消息
                        outfile << "    pair<string, int> p = loadEnv(\"server.env\");" << endl;
                        outfile << "    int sockfd = socketConnect(p.first.c_str(), p.second);" << endl;
                        outfile << "    char buff[BUFFSIZE];" << endl;

                        // outfile << "    send(sockfd, msg.funcname, sizeof(msg.funcname), 0);" << endl;
                        outfile << "    memcpy(buff, &msg, sizeof(" << funcname << "_msg::message));" << endl;
                        // outfile << "    sleep(0.7);" << endl;
                        outfile << "    send(sockfd, buff, sizeof(buff), 0);" << endl;
                        // end

                        // 接收消息 解码
                        outfile << "    bzero(buff, sizeof(buff));" << endl;
                        outfile << "    recv(sockfd, buff, BUFFSIZE - 1, 0);" << endl;
                        outfile << "    " << funcname << "_msg::MSG *res = (" << funcname << "_msg::MSG *)buff;" << endl;

                        // 关闭连接
                        outfile << "    close(sockfd);" << endl;

                        // 返回值
                        if (ret != "void")
                        {
                            outfile << "    return res->ret;" << endl;
                        }
                        outfile << "}" << endl
                                << endl
                                << endl;
                    }
                    else if (type == "server" || type == "Server") // 服务端存根
                    {
                        // 定义参数结构体
                        outfile << "namespace " << funcname << "_msg" << endl
                                << "{" << endl;
                        outfile << "    typedef struct parameter{" << endl;
                        vector<pair<string, string>> params; // 分析函数中参数的数量类型及名字
                        for (int i = 2; i < v.size(); i += 2)
                        {
                            TrimString(v[i], '(');
                            TrimString(v[i], ',');
                            TrimString(v[i + 1], ')');
                            TrimString(v[i + 1], ',');
                            pair<string, string> p;
                            p = make_pair(v[i], v[i + 1]);
                            params.push_back(p);
                            // cout << v[i] << "    " << v[i + 1] << endl;
                            outfile << "        " << v[i] << " " << v[i + 1] << ";" << endl;
                        }
                        outfile << "    }PARAMS;" << endl;
                        // end

                        // 定义消息结构体
                        outfile << "    typedef struct message{" << endl;
                        outfile << "        char funcname[20];" << endl;
                        outfile << "        " << ret << " ret;" << endl;
                        outfile << "        "
                                << "PARAMS params;" << endl;
                        outfile << "    }MSG;" << endl
                                << "}" << endl;
                        // 定义函数

                        outfile << trim(query) << ";" << endl;
                        outfile << "void " << funcname << "(char* buff)" << endl
                                << "{" << endl;

                        // 消息解码
                        outfile << "    " << funcname << "_msg::MSG *msg = (" << funcname << "_msg::MSG *)buff;" << endl;
                        // 将参数交由服务端函数处理  函数格式与客户端调用的形式一致
                        outfile << "    msg->ret = " << funcname << "(";
                        int m = 0;
                        for (int i = 2; i < v.size(); i += 2, m++)
                        {
                            TrimString(v[i], '(');
                            TrimString(v[i], ',');
                            TrimString(v[i + 1], ')');
                            TrimString(v[i + 1], ',');
                            pair<string, string> p;
                            p = make_pair(v[i], v[i + 1]);
                            params.push_back(p);
                            cout << "msg->params." << v[i + 1] << endl;
                            if (m == 0)
                            {
                                outfile << "msg->params." << v[i + 1];
                            }
                            else
                            {
                                outfile << ", msg->params." << v[i + 1];
                            }
                        }
                        outfile << ");" << endl;

                        outfile << "}" << endl
                                << endl
                                << endl;
                        // end

                        // 交由服务端实现的函数
                        outfile << trim(query) << endl;
                        outfile << "{" << endl;
                        outfile << "    //TO_DO::" << endl;
                        outfile << "}" << endl
                                << endl
                                << endl;
                        // end
                    }
                    else
                    {
                        cout << "type error" << endl;
                        return 0;
                    }
                }
            }
            else
            {
                writefunc = 0;

                // writeend = 1;
                // break;
            }
        }
        else if (writestruct == 1)
        {
            if (v[0] != "}")
            {
                if (v[0] != "{") // 构造结构体
                {
                    outfile << "    " << v[1] << " " << v[0] << ";" << endl;
                }
            }
            else
            {
                outfile << "}" << structname << ";" << endl
                        << endl
                        << endl;
                writestruct = 0;
            }
        }
        // for (int i = 0; i < v.size(); i++)
        // {
        //     // cout << v[i] << endl;

        //     if (v[i] == "service")
        //     {
        //         writefunc = 1;
        //         break;
        //     }
        //     if (v[i] != "}")
        //     {
        //         if (v[i] != "{")    //构造函数
        //         {
        //             outfile << trim(query) << "{" << endl;

        //             outfile << "}" << endl;
        //             break;
        //         }
        //     }
        //     else
        //     {
        //         writefunc = 0;
        //         writeend = 1;
        //         break;
        //     }
        // }
        // cout << 1<<endl;
        // if (writefunc == 1)
        // {
        //     continue;
        // }
    }
    outfile.close();

    return 0;
}
