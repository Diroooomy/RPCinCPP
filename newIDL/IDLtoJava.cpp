#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <regex>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;
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

string getS(string s)
{
    s[0] = toupper(s[0]);
    return "get" + s;
}

string setS(string s)
{
    s[0] = toupper(s[0]);
    return "set" + s;
}

int main(int argc, char *argv[])
{
    string input_file = "age_sum.idl";
    string type = "server";
    ifstream in(input_file);
    size_t pos = input_file.find('.');
    string temp = input_file.substr(0, pos);
    ofstream outfile;
    string query;

    int isCreate = mkdir(temp.c_str(), S_IRWXU);

    int writefunc = 0;
    int writestruct = 0;
    string structname;
    int structnum = 0;
    string servicename;
    vector<pair<string, string>> struct_params;
    vector<string> func_line;
    while (getline(in, query))
    {
        string line = query;
        if (query.empty())
            continue;
        vector<string> v;
        stringstream ss(trim(query));
        string token;
        while (getline(ss, token, ' '))
        {
            v.push_back(token);
        }
        if (v[0] == "service")
        {
            servicename = v[1];
            writefunc = 1;
            continue;
        }
        else if (v[0] == "struct")
        {
            structname = v[1];
            outfile.open(temp + "/" + v[1] + ".java");
            writestruct = 1;

            structnum++;
            // outfile << "typedef struct " << temp << "_var" << structnum << endl
            //         << "{" << endl;
            continue;
        }

        if (writefunc == 1)
        {
            if (v[0] != "}")
            {
                if (v[0] != "{") // 构造函数
                {
                    func_line.push_back(line);
                    string ret = v[0];      // 函数返回类型
                    string funcname = v[1]; // 函数名

                    //  定义参数结构体
                    outfile.open(temp + "/"+funcname+"_PARAMS.java");
                    outfile << "//package org.javaClient.Calculator;" << endl;
                    outfile << "public class "<< funcname<<"_PARAMS {" << endl;
                    vector<pair<string, string>> params;
                    for (int i = 2; i < v.size(); i += 2)
                    {
                        TrimString(v[i], '(');
                        TrimString(v[i], ',');
                        TrimString(v[i + 1], ')');
                        TrimString(v[i + 1], ',');
                        if (v[i + 1].find("[") != v[i + 1].npos)
                        {
                            v[i] += v[i + 1].substr(v[i + 1].find("["), v[i].size());
                            v[i + 1] = v[i + 1].substr(0, v[i + 1].find("["));
                        }
                        pair<string, string> p;
                        p = make_pair(v[i], v[i + 1]);
                        params.push_back(p);
                        // cout << v[i] << "    " << v[i + 1] << endl;
                        outfile << "    " << v[i] << " " << v[i + 1] << ";" << endl;
                        outfile << "    public void " << setS(v[i + 1]) << "(" << v[i] << " " << v[i + 1] << ") {" << endl
                                << "        this." << v[i + 1] << " = " << v[i + 1] << ";" << endl
                                << "    }" << endl;
                        outfile << "    public " << v[i] << " " << getS(v[i + 1]) << "() {" << endl
                                << "        return " << v[i + 1] << ";" << endl
                                << "    }" << endl;
                    }
                    outfile << "    public "<<funcname<<"_PARAMS(){}" << endl;
                    outfile << "    public "<<funcname<<"_PARAMS(";
                    for (int i=0;i < params.size();i++) {
                        if (i == 0) {
                            outfile << params[i].first << " " << params[i].second;
                        } else {
                            outfile << ", "<<params[i].first << " " << params[i].second;
                        }
                    }
                    outfile << ") {" << endl;
                    for (int i=0;i < params.size();i++) {
                        outfile << "        this." << params[i].second << " = " << params[i].second << ";" << endl;
                    }
                    outfile << "    }" << endl;
                    outfile << "}" << endl;
                    outfile.close();

                    //  定义消息结构体
                    outfile.open(temp + "/"+funcname+"_MSG.java");
                    // outfile << "//package org.javaClient.Calculator;" << endl;
                    // outfile << "public class MSG {" << endl;

                    // outfile << "}" << endl;
                    outfile << "//package org.javaClient.Calculator;" << endl
                            << "public class "<< funcname<<"_MSG {" << endl
                            << "    String funcname;" << endl
                            << "    " <<ret << " ret;" << endl
                            << "    "<<funcname <<"_PARAMS params;" << endl
                            << "    public "<<funcname<<"_MSG() {}"<<endl
                            << "    public "<<funcname<<"_MSG(String funcname, "<< funcname<< "_PARAMS params) {"<<endl
                            << "        this.funcname=funcname;" << endl
                            // << "        this.ret=ret;" << endl
                            << "        this.params=params;" << endl
                            << "    }"<<endl
                            << "    public void setFuncname(String funcname) {" << endl
                            << "        this.funcname = funcname;" << endl
                            << "    }" << endl

                            << "    public String getFuncname() {" << endl
                            << "        return funcname;" << endl
                            << "    }" << endl

                            << "    public void setRet(" << ret<<" ret) {" << endl
                            << "        this.ret = ret;" << endl
                            << "    }" << endl

                            << "    public "<<ret <<" getRet() {" << endl
                            << "        return ret;" << endl
                            << "    }" << endl

                            << "    public void setParams("<< funcname<<"_PARAMS params) {" << endl
                            << "        this.params = params;" << endl
                            << "    }" << endl

                            << "    public "<<funcname <<"_PARAMS getParams() {" << endl
                            << "        return params;" << endl
                            << "    }" << endl
                            << "}" << endl;
                    outfile.close();

                    //定义服务函数
                    


                }
            } else {
                writefunc = 0;
                outfile.open(temp + "/"+servicename+".java");
                outfile << "//package org.javaClient.Calculator;" << endl
                        << "import org.javaClient.socket;" << endl
                        << "import org.json.JSONObject;" << endl
                        << "import java.io.*;" << endl << endl;
                
                outfile << "public class " << servicename << "{" << endl;
                for (int i = 0; i < func_line.size(); i++) {
                    string l = func_line[i];
                    vector<string> word;
                    stringstream xx(trim(l));
                    string to;
                    while (getline(xx, to, ' '))
                    {
                        word.push_back(to);
                    }
                    string rettype = word[0];
                    string funcname = word[1];
                    outfile << "    public static " << word[0] << " " << word[1] << l.substr(l.find("("), l.size()) << " {" << endl;
                    vector<pair<string, string>> params;
                    for (int i = 2; i < word.size(); i += 2)
                    {
                        TrimString(word[i], '(');
                        TrimString(word[i], ',');
                        TrimString(word[i + 1], ')');
                        TrimString(word[i + 1], ',');
                        if (word[i + 1].find("[") != word[i + 1].npos)
                        {
                            word[i] += word[i + 1].substr(word[i + 1].find("["), word[i].size());
                            word[i + 1] = word[i + 1].substr(0, word[i + 1].find("["));
                        }
                        pair<string, string> p;
                        p = make_pair(word[i], word[i + 1]);
                        params.push_back(p);
                    }
                    outfile << "        " << funcname << "_PARAMS params = new " << funcname <<"_PARAMS(" ;
                    for (int i = 0; i < params.size(); i ++) {
                        if (i == 0) {
                            outfile << params[i].second;
                        } else {
                            outfile << ", " << params[i].second;
                        }
                        // outfile <<
                    }
                    outfile << ");" << endl;

                    outfile << "        " << funcname << "_MSG msg = new " << funcname << "_MSG(\"" <<funcname << "\", params);" << endl;
                    outfile << "        JSONObject m = new JSONObject(msg);" << endl
                            << "        JSONObject json = null;" << endl
                            << "        try {" << endl
                            << "            socket client = new socket();" << endl
                            << "            client.send(m.toString());" << endl
                            << "            json = client.recive();" << endl
                            << "            System.out.println(json);" << endl
                            << "            client.close();" << endl
                            << "        } catch (IOException e) {" << endl
                            << "            e.printStackTrace();" << endl
                            << "        }" << endl
                            << "        return (" << rettype << ") json.get(\"ret\");" << endl;
                    outfile << "    }" << endl;
                }
                outfile << "}" << endl;
                outfile.close();
            }
        }
        else if (writestruct == 1)
        {
            if (v[0] != "}")
            {
                if (v[0] != "{") // 构造结构体
                {
                    
                    // outfile << "    " << v[1] << " " << v[0] << ";" << endl;
                    pair<string, string> pa;
                    size_t pos = v[0].find("[");
                    if ((pos != v[0].npos))
                    {
                        v[1] += v[0].substr(pos, v[0].size());
                        v[0] = v[0].substr(0, pos);
                    }
                    if (v[1] == "string") v[1] = "String";
                    pa = make_pair(v[0], v[1]);
                    struct_params.push_back(pa);
                    
                    
                }
            }
            else
            {
                outfile << "public class " << structname << " {" << endl;
                for(int i=0; i < struct_params.size(); i++) {
                        string name = struct_params[i].first;
                        string type = struct_params[i].second;
                        outfile << "    " << type << " " << name << ";" << endl;
                        outfile << "    public void " << setS(name) << "(" << type << " " << name << ") {" << endl
                                << "        this." << name << " = " << name << ";" << endl
                                << "    }" << endl;
                        outfile << "    public " << type << " " << getS(name) << "() {" << endl
                                << "        return " << name << ";" << endl
                                << "    }" << endl;
                    }
                    outfile << "    public "<<structname<<"(){}" << endl;
                    outfile << "    public "<<structname<<"(";
                    for (int i=0;i < struct_params.size();i++) {
                        if (i == 0) {
                            outfile << struct_params[i].second << " " << struct_params[i].first;
                        } else {
                            outfile << ", "<<struct_params[i].second << " " << struct_params[i].first;
                        }
                    }
                    outfile << ") {" << endl;

                    for (int i=0;i < struct_params.size();i++) {
                        outfile << "        this." << struct_params[i].first << " = " << struct_params[i].first << ";" << endl;
                    }
                    outfile << "    }" << endl;
                    outfile << "}" << endl;
                struct_params.erase(struct_params.begin(), struct_params.end()); // 清空并释放内存
                writestruct = 0;
                outfile.close();
            }
        }
    }
}