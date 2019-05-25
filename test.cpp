#include <iostream>

using namespace std;

int main(int argc,char **argv)
{
  int a=0,b=0;

  for(a;a<10;a++){
    cout<<a<<endl;
  }
    cout<<'\n'<<endl;
  for(b;b<10;++b){
    cout<<b<<endl;
  }

  cout<<a<<endl;
  cout<<b<<endl;

	cout<<"..........."<<endl;

	int a1 = 10;
	int b1 = 4;
	int c;
	cout<<(c=a1++)<<endl;

  return 0;
}
