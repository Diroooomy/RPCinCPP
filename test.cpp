//

#include <iostream>
// #include <vector>
#include <fstream>
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
// #include <iostream>
// #include "json.hpp"
// using json_t = nlohmann::json;

// using namespace std;

// int main()
// {
//     json_t j; // json对象
//     // 添加json对象
//     j["age"] = 21;               // "age":23
//     j["name"] = "diyuyi";        // "name":diyuyi
//     j["gear"]["suits"] = "2099"; // "gear":{"suits":"2099"}
//     j["jobs"] = {"student"};     // "jobs":["student"]
//     std::vector<int> v = {1, 2, 3};
//     j["numbers"] = v; // "numbers":[1, 2, 3]
//     std::map<std::string, int> m = {{"one", 1}, {"two", 2}};
//     j["map"] = m; // "map":{"one", 1},{"two", 2}

//     // 序列化得到json文本形式
//     // std::cout << j.dump() << std::endl;  // 无缩进
//     // std::cout << j.dump(2) << std::endl; // 缩进两格
//     ofstream outfile;
//     outfile.open("cpp.json");
//     outfile << j.dump(2) << std::endl; // 缩进两格
// }
// #include <iostream>
// #include <fstream>


#
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
string split(const string &str, const string &pattern)
{
    string res;
    if(str == "")
        return res;
    //在字符串末尾也加入分隔符，方便截取最后一段
    string strs = str;
    size_t pos = strs.find(pattern);

    while(pos != strs.npos)
    {
        
        strs = strs.substr(pos+1, strs.size());
        pos = strs.find(pattern);
    }

    return strs;
}
int main() {
    // ofstream outfile;
    // outfile.open("example.txt");
    // outfile << "Writing this to a file.\n";
    // outfile << "Writing";
    // outfile.close();
    char buff1[512];
    string funcname ="/home/helen/桌面/RPC/test/add.idl";
    // memcpy(buff1, &funcname, sizeof(string));
    trim(funcname);
    cout <<sizeof(funcname)<<endl;

    // char buff2[2048];
    // cout <<(string)(buff1) <<endl;
    // memcpy(buff2, &funcname, sizeof(string));
    // char *buff = strcat(buff1, buff2);
    
    // // cout <<strlen(buff1) <<endl;
    // char arr2[512];
    // strncpy(arr2, buff, 512);
    // cout << arr2 << endl;
    return 0;
}