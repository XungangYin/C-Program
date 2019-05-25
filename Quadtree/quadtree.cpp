#include <iostream>
#include <deque>
#include <string.h>

using namespace std;
struct node{
  node *child[4];
  bool zero;
  node():zero(true){
    memset(child,0,sizeof(child));//将child中当前位置后的sizeof个字节用0代替
  }
  void print(){
    if(child[0]){
      cout<<1;
      return ;
    }
    // cout<<0<<(zero ? 0:1);
  }
};
node gnode[257];
char gmap[16][16];
node *alloc(){
  static int i  =0;
  return &gnode[i++];
}

void print(node *root){
  deque<node *>dq;
  dq.push_back(root);
  // cout<<1;
  //  cout<<dq.size()<<endl;
   static int abc = 0;
  while(!dq.empty()){
    cout<<"asdf:"<<++abc<<endl;
    node *front = dq.front();
    dq.pop_front();
    for(int i = 0;i <4;i++){
      node *child = front->child[i];
      child->print();
      if(child->child[0])
	dq.push_back(child);
    }
  }
  cout<<endl;
}

bool check_blocks(int x,int y,int n){
  char c = gmap[y][x];
  if(n ==1) return true;
  for(int i = y; i< y+n;++i){
    for(int j = x; j<x+n;++ j){
      if(gmap[i][j] != c) return false;
    }
  }
  return true;
}

void get_start(int i,int n,int sx,int sy,int &x,int &y){
  switch(i){
  case 0:
    x = sx;
    y = sy;
    break;
  case 1:
    x = sx+n/2; y = sy;
    break;
  case 2:
    x = sx; y = sy+n/2;
    break;
  case 3:
    x = sx + n/2;
    y = sy + n/2;
    break;
  }
}

void build(node *root,int n,int sx,int sy){
  int x, y;
  for(int i  =0; i<4;i++){
    get_start(i,n,sx,sy,x,y);
    node *child = alloc();
    root->child[i] = child;
    if(check_blocks(x,y,n/2)){
      child->zero = gmap[y][x] =='0';
    }
    else{
      build(child,n/2,x,y);
    }
  }
}
void solve(int n){
  if(n ==1){
    cout<<1<<gmap[0][0]<<endl;
  }
  else {
    node root;
    root.zero = true;
    build(&root,n,0,0);
    print(&root);
  }
}

int main(){
  int n;
  cin>>n;
  for(int i = 0;i <n;++i){
    for(int j = 0; j<n;j++)
      cin>>gmap[i][j];
  }
  solve(n);
  return 0;
}

