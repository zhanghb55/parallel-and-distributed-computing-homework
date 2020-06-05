//
//  Created by zhb on 2020/5/1.
//  Copyright © 2020 zhb. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string.h>
#include <fstream>
#include <algorithm>

using namespace std;

vector<string> all_function;
vector<string> Non_reentrant_fun;




inline void deduplication(vector<string>& c) 
{
	sort(c.begin(), c.end());
    auto new_end = unique(c.begin(), c.end());//"删除"相邻的重复元素
	c.erase(new_end, c.end());//删除(真正的删除)重复的元素
}
 

string read_file(const char*file_name){
    fstream file(file_name);
    stringstream ss; 
    ss << file.rdbuf();
    string str = ss.str();
    return str;
}

void divide(string s){
    vector<int> pos;
    int index = 0;
    string sub = "define";
	while ((index = s.find(sub, index)) < s.length())
	{
        pos.push_back(index);
        string temp;
        int flag = 0;
        int target = 0;
        for(int i = index;i < s.size();i++){
            if(s[i] == '}'){
                target = i;
                flag = 1;
                break;
            }
            if(flag)break;
        }
        temp = s.substr(index,target - index + 1);
        all_function.push_back(temp);
		index++;

	}
}


class module{
    string fun_name;//函数名
    string argument;//参数列表
    string ret_type;//返回值类型
    string content;//函数体
    vector<int> symbol;//标志@在函数体的位置
    vector<string> _global;//函数中的全局变量
    vector<string> _static;//函数中的静态变量
    vector<string> _non_reentrant_fun;//函数中调用的不可重入函数

    //获取所有的@标志
    void get_all_symbol();
    //检测symbol向量中是否有全局变量，有则放入_global向量
    void detect_global();
    //检测symbol向量中是否有静态变量，有则放入_static向量
    void detect_static();
    //检测symbol向量中是否有函数中调用的不可重入函数，有则放入_non_reentrant_fun向量
    void detect_non_reentrant_fun();
    //对上述四种操作进行封装，若为不可重用函数则返回true
    bool detect();
public:
    //将IR code中一个函数的字符串表示作为一个整体初始化模块类
    module(string fun);
    //对detect()进行封装并打印提示信息。
    bool detect_and_print();
};
module::module(string fun){
    stringstream s;
    s << fun;
    string define;
    s >> define;
    s >> ret_type;
    s >> fun_name;
    int content_begin;
    for(int i = 0;i < fun.size();i++){
        if(fun[i] == '{'){
            content_begin = i;
            break;
        }
    }
    content = fun.substr(content_begin);
    int argument_pos = 0;
    for(int i = 0;i < fun_name.size();i++){
        if(fun_name[i] == '('){
            argument_pos = i;
            break;
        }
    }
    argument = fun_name.substr(argument_pos + 1);
    argument.pop_back();
    fun_name.erase(fun_name.begin() + argument_pos,fun_name.end());
    fun_name.erase(fun_name.begin());
}
void module::get_all_symbol(){
    for(int i = 0;i < content.size();i++){
        if(content[i] == '@'){
            symbol.push_back(i);
        }
    }

}
void module::detect_global(){
    for(int i = 0;i < symbol.size();i++){
        stringstream ss;
        string sub = content.substr(symbol[i]);
        ss << sub;
        string value;
        ss >> value;
        int flag = 0;
        for(int j = 0;j < value.size();j++){
            if(value[j] == '(' || value[j] == '.'){
                flag = 1;
                break;
            }
        }
        if(flag)continue;
        while(value[value.size() - 1] == ',')value.pop_back();
        value.erase(value.begin());
        _global.push_back(value);
    }
}
void module::detect_static(){
    for(int i = 0;i < symbol.size();i++){
        stringstream ss;
        string sub = content.substr(symbol[i]);
        ss << sub;
        string value;
        ss >> value;
        int flag = 0;
        int point = 1;
        for(int j = 0;j < value.size();j++){
            if(value[j] == '('){
                flag = 1;
                break;
            }
            if(value[j] == '.'){
                point = 0;
            }
        }
        if(flag || point)continue;
        while(value[value.size() - 1] == ',')value.pop_back();
        value.erase(value.begin());
        if(value[0] == '.')continue;
        _static.push_back(value);
    }
}
void module::detect_non_reentrant_fun(){
    for(int i = 0;i < symbol.size();i++){
        stringstream ss;
        string sub = content.substr(symbol[i]);
        ss << sub;
        string value;
        ss >> value;
        int flag = 0;
        int pos = -1;
        for(int j = 0;j < value.size();j++){
            if(value[j] == '('){
                flag = 1;
                pos = j;
                break;
            }
        }
        if(!flag)continue;
        
        value.erase(value.begin() + pos,value.end());
        value.erase(value.begin());
        for(int k = 0;k < Non_reentrant_fun.size();k++){
            if(value == Non_reentrant_fun[k]){
                _non_reentrant_fun.push_back(value);
            }
        }
    }
}
bool module::detect(){
    get_all_symbol();
    detect_non_reentrant_fun();
    detect_global();
    detect_static();
    return _global.size() || _static.size() || _non_reentrant_fun.size();
}
bool module::detect_and_print(){
    if(fun_name == "main")return false;
    bool res = detect();
    if(res == false){
        return false;
    }
    else{
        Non_reentrant_fun.push_back(fun_name);
        cout << "Detect non reentrant function " << fun_name << ", details are as follows: " << endl;
        deduplication(_non_reentrant_fun);
        deduplication(_global);
        deduplication(_static);
        if(_non_reentrant_fun.size()){
            cout << "Call non reentrant function:";
            for(int i = 0;i < _non_reentrant_fun.size();i++){
                cout << _non_reentrant_fun[i] << "  ";
            }
            cout << endl;
        }
        if(_global.size()){
            cout << "Use global valuable:";
            for(int i = 0;i < _global.size();i++){
                cout << _global[i] << " ";
            }
            cout << endl;
        }
        if(_static.size()){
            cout << "Use static valuable:";
            for(int i = 0;i < _static.size();i++){
                cout << _static[i] << " ";
            }
            cout << endl;
        }
        return true;
    }
    return true;
}
