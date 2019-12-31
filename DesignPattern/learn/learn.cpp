// File Name: learn.cpp
// Author: YXG
// Created Time: 2019年11月07日 星期四 15时43分02秒
//学习一下boost
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <sstream>
using namespace std;

class String{
public:
    String(const string &s):s(s){}        

    String to_lower_copy() const {
        return {boost::to_lower_copy(s)};
    }

    vector<String> split(const string & delimiter = " ") const {
        vector<string> parts;
        //boost分割
        boost::split(parts,s,boost::is_any_of(delimiter), 
              boost::token_compress_on );
        return vector<String>(parts.begin(),parts.end());
    };

    size_t get_length() const{
        return s.length();    
    };

    friend ostream &operator<<(ostream &os, const  String &s){
        os<<s.s;
        return os;    
    }

private:
    string s;
};




using namespace std;
int main(int argc,char **argv){
   
    
    String s("hello YXG  lcx");
    cout<<"original is :"<<s<<endl;
    for(auto & w : s.to_lower_copy().split()){
        cout<<w.get_length()<<" ";    
        cout<<w<<endl;
    }

    
    return 0;
}
