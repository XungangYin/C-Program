#include <iostream>
#include <string.h>

using namespace std;
int main(int argc,char **argv){

  char a[]="12345";

  cout<<strlen(a)<<endl; //结果为5
  cout<<sizeof(a)<<endl; //结果为6

  char *b = "123456789";
  cout<<strlen(b)<<endl;
  cout<<sizeof(b)<<endl; //结果永远为8，输出的是指针的所占字节大小

}
