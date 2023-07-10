#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <regex>
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

void TrimString(string &src, char c = ' ') // 默认删除前后空格,递归删除
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
    string input_file = "Calculator.idl";
    string type = "server";
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
        
    }
    outfile << "#include \"json.hpp\"" << endl;
    outfile << "#define BUFFSIZE 2048" << endl;
    outfile << "using json = nlohmann::json;" << endl;
    outfile << "using namespace std;" << endl;
    int flag = 0;
    int writefunc = 0;
    int writestruct = 0;
    string structname;
    int writeend = 0;
    int structnum = 0;
    vector<pair<string, string>> struct_params;
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


                        outfile << "    void from_json(const json &j, PARAMS &p)" <<endl
                                << "    {" <<endl;
                        for (int i = 0; i < params.size(); ++i) {
                            if (params[i].first == "char" && params[i].second.find("[")!=params[i].second.npos) //如果是char类型数组，无法直接赋值
                            {
                                string type = params[i].first;
                                string param_name = params[i].second;
                                type += param_name.substr(param_name.find("["), param_name.size());
                                param_name = param_name.substr(0, param_name.find("["));
                                outfile << "        string s;" <<endl
                                        << "        j.at(\""<< param_name<<"\").get_to(s);" <<endl
                                        << "        memcpy(p."<<param_name << ",s.c_str(), sizeof(p."<<param_name << "));" <<endl
                                        << endl;
                            } else if (params[i].second.find("[")!=params[i].second.npos){
                                string type = params[i].first;
                                string param_name = params[i].second;
                                type += param_name.substr(param_name.find("["), param_name.size());
                                param_name = param_name.substr(0, param_name.find("["));
                                outfile << "        j.at(\"" << param_name << "\").get_to(p." << param_name << ");" <<endl;
                            } else {
                                outfile << "        j.at(\"" << params[i].second << "\").get_to(p." << params[i].second << ");" <<endl;
                            }
                        }

                        outfile << "    }" << endl << endl;

                        outfile << "    void to_json(json &j, const PARAMS &p)" <<endl
                                << "    {"<<endl
                                << "        j = json{";
                        for (int i = 0; i < params.size(); ++i) {
                            if (params[i].second.find("[")!=params[i].second.npos){
                                string type = params[i].first;
                                string param_name = params[i].second;
                                type += param_name.substr(param_name.find("["), param_name.size());
                                param_name = param_name.substr(0, param_name.find("["));
                                if (i == 0) {
                                    outfile << "{\""<<param_name  <<"\", p."<< param_name<<"}";
                                } else {
                                    outfile << ", {\""<<param_name  <<"\", p."<< param_name<<"}";
                                }
                            } else {
                                if (i == 0) {
                                    outfile << "{\""<<params[i].second  <<"\", p."<< params[i].second<<"}";
                                } else {
                                    outfile << ", {\""<<params[i].second  <<"\", p."<< params[i].second<<"}";
                                }
                            }
                        }

                        outfile << "};" <<endl;
                        outfile << "    }" <<endl<<endl;

                        // 定义消息结构体
                        outfile << "    typedef struct message{" << endl;
                        outfile << "        char funcname[20];" << endl;
                        outfile << "        " << ret << " ret;" << endl;
                        outfile << "        "
                                << "PARAMS params;" << endl;
                        outfile << "    }MSG;" << endl;
                                // << "}" << endl;

                        
                        


                        // msg序列化与反序列化
                        outfile << "    void from_json(const json &j, MSG &p)" <<endl
                                << "    {" << endl
                                << "        string s;" <<endl
                                << "        j.at(\"funcname\").get_to(s);" <<endl
                                << "        memcpy(p.funcname, s.c_str(), sizeof(p.funcname));" <<endl
                                << "        j.at(\"ret\").get_to(p.ret);" << endl
                                << "        j.at(\"params\").get_to(p.params);" << endl
                                << "    }" << endl << endl;
                        outfile << "    void to_json(json &j, const MSG &p)" <<endl
                                << "    {" << endl
                                << "        j = json{{\"funcname\", p.funcname}, {\"ret\", p.ret}, {\"params\", p.params}};" << endl
                                << "    }" << endl << endl;

                        // end

                        // 命名空间结束
                        outfile << "}" << endl <<endl <<endl;

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

                        outfile << "    json j = msg;" <<endl
                                << "    string s = j.dump();" <<endl;

                        // outfile << "    send(sockfd, msg.funcname, sizeof(msg.funcname), 0);" << endl;
                        outfile << "    memcpy(buff, s.c_str(), BUFFSIZE-1);" << endl;
                        // outfile << "    sleep(0.7);" << endl;
                        outfile << "    send(sockfd, buff, sizeof(buff), 0);" << endl;
                        // end

                        // 接收消息 解码
                        outfile << "    bzero(buff, sizeof(buff));" << endl;
                        outfile << "    recv(sockfd, buff, BUFFSIZE - 1, 0);" << endl;

                        outfile << "    auto ret = json::parse(buff);" <<endl;
                        outfile << "    " << funcname << "_msg::MSG res = ret;" << endl;
                        // outfile << "    " << funcname << "_msg::MSG *res = (" << funcname << "_msg::MSG *)buff;" << endl;

                        // 关闭连接
                        outfile << "    close(sockfd);" << endl;

                        // 返回值
                        if (ret != "void")
                        {
                            outfile << "    return res.ret;" << endl;
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

                        outfile << "    void from_json(const json &j, PARAMS &p)" <<endl
                                << "    {" <<endl;
                        for (int i = 0; i < params.size(); ++i) {
                            if (params[i].first == "char" && params[i].second.find("[")!=params[i].second.npos) //如果是char类型数组，无法直接赋值
                            {
                                string type = params[i].first;
                                string param_name = params[i].second;
                                type += param_name.substr(param_name.find("["), param_name.size());
                                param_name = param_name.substr(0, param_name.find("["));
                                outfile << "        string s;" <<endl
                                        << "        j.at(\""<< param_name<<"\").get_to(s);" <<endl
                                        << "        memcpy(p."<<param_name << ",s.c_str(), sizeof(p."<<param_name << "));" <<endl
                                        << endl;
                            } else if (params[i].second.find("[")!=params[i].second.npos){
                                string type = params[i].first;
                                string param_name = params[i].second;
                                type += param_name.substr(param_name.find("["), param_name.size());
                                param_name = param_name.substr(0, param_name.find("["));
                                outfile << "        j.at(\"" << param_name << "\").get_to(p." << param_name << ");" <<endl;
                            } else {
                                outfile << "        j.at(\"" << params[i].second << "\").get_to(p." << params[i].second << ");" <<endl;
                            }
                        }

                        outfile << "    }" << endl << endl;

                        outfile << "    void to_json(json &j, const PARAMS &p)" <<endl
                                << "    {"<<endl
                                << "        j = json{";
                        for (int i = 0; i < params.size(); ++i) {
                            if (params[i].second.find("[")!=params[i].second.npos){
                                string type = params[i].first;
                                string param_name = params[i].second;
                                type += param_name.substr(param_name.find("["), param_name.size());
                                param_name = param_name.substr(0, param_name.find("["));
                                if (i == 0) {
                                    outfile << "{\""<<param_name  <<"\", p."<< param_name<<"}";
                                } else {
                                    outfile << ", {\""<<param_name  <<"\", p."<< param_name<<"}";
                                }
                            } else {
                                if (i == 0) {
                                    outfile << "{\""<<params[i].second  <<"\", p."<< params[i].second<<"}";
                                } else {
                                    outfile << ", {\""<<params[i].second  <<"\", p."<< params[i].second<<"}";
                                }
                            }
                        }

                        outfile << "};" <<endl;
                        outfile << "    }" <<endl<<endl;


                        // 定义消息结构体
                        outfile << "    typedef struct message{" << endl;
                        outfile << "        char funcname[20];" << endl;
                        outfile << "        " << ret << " ret;" << endl;
                        outfile << "        "
                                << "PARAMS params;" << endl;
                        outfile << "    } MSG;" << endl;
                                // << "}" << endl <<endl;
                        outfile << "    void from_json(const json &j, MSG &p)" <<endl
                                << "    {" << endl
                                << "        string s;" <<endl
                                << "        j.at(\"funcname\").get_to(s);" <<endl
                                << "        memcpy(p.funcname, s.c_str(), sizeof(p.funcname));" <<endl
                                << "        j.at(\"ret\").get_to(p.ret);" << endl
                                << "        j.at(\"params\").get_to(p.params);" << endl
                                << "    }" << endl << endl;
                        outfile << "    void to_json(json &j, const MSG &p)" <<endl
                                << "    {" << endl
                                << "        j = json{{\"funcname\", p.funcname}, {\"ret\", p.ret}, {\"params\", p.params}};" << endl
                                << "    }" << endl << endl;
                        // end


                        //命名空间结束
                        outfile << "}" <<endl<<endl;
                        // 定义函数

                        outfile << trim(query) << ";" << endl;
                        outfile << "void " << funcname << "(char* buff)" << endl
                                << "{" << endl;

                        // 消息解码
                        outfile << "    string s = buff;" << endl
                                << "    auto j = json::parse(s);" <<endl
                                << "    " <<funcname << "_msg::MSG msg = j;" <<endl;
                        // outfile << "    " << funcname << "_msg::MSG *msg = (" << funcname << "_msg::MSG *)buff;" << endl;
                        // 将参数交由服务端函数处理  函数格式与客户端调用的形式一致
                        outfile << "    msg.ret = " << funcname << "(";
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
                            cout << "msg.params." << v[i + 1] << endl;
                            if (m == 0)
                            {
                                outfile << "msg.params." << v[i + 1];
                            }
                            else
                            {
                                outfile << ", msg.params." << v[i + 1];
                            }
                        }
                        outfile << ");" << endl;

                        // 构造回复的消息
                        outfile << "    json m = msg;" <<endl
                                << "    string l = m.dump();" << endl
                                << "    bzero(buff, BUFFSIZE);" << endl
                                << "    strncpy(buff, l.c_str(), BUFFSIZE);" <<endl;

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
                    pair<string, string> pa;
                    size_t pos = v[0].find("[");
                    if ((pos != v[0].npos)) {
                        v[1] += v[0].substr(pos,v[0].size());
                        v[0] = v[0].substr(0, pos);
                    }
                    pa = make_pair(v[0], v[1]);
                    struct_params.push_back(pa);
                }
            }
            else
            {
                outfile << "}" << structname << ";" << endl
                        << endl
                        << endl;

                outfile << "void from_json(const json &j, "<<structname<<" &p)" <<endl
                        << "{    "<<endl;
                for (int i = 0; i < struct_params.size(); ++i) {
                    // regex reg("char\\[(\\d+)\\]");
                    cout << struct_params[i].second << endl;
                    if (struct_params[i].second.substr(0,struct_params[i].second.find('[')) == "char") //如果是char类型数组，无法直接赋值
                    {
                        outfile << "    string s;" <<endl
                                << "    j.at(\"" << struct_params[i].first << "\").get_to(s);" << endl
                                << "    memcpy(p." << struct_params[i].first << ", s.c_str(), sizeof(p."<< struct_params[i].first << "));" <<endl;

                    } else { // 其他类型
                        outfile << "    j.at(\"" << struct_params[i].first << "\").get_to(p." << struct_params[i].first << ");" <<endl;
                    }
                    // cout <<  << endl;
                }
                outfile << "}" << endl;

                outfile << "void to_json(json &j, const "<<structname<<" &p)" <<endl
                        << "{"<<endl
                        << "    j = json{";
                for (int i = 0; i < struct_params.size(); ++i) {
                    if (i == 0) {
                        outfile << "{\"" <<struct_params[i].first << "\", p." << struct_params[i].first << "}";
                    } else {
                        outfile << ", {\"" <<struct_params[i].first << "\", p." << struct_params[i].first << "}";
                    }
                    
                }
                outfile << "};" << endl;
                outfile << "}" << endl;
                struct_params.erase(struct_params.begin(), struct_params.end()); //清空并释放内存
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
