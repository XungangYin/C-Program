#include <iostream>
#include <memory>
#include <assert.h>

template <typename T>
class SmartPointer{
private:
  T* _ptr;
  size_t *_count;
public:
  SmartPointer(T* ptr = nullptr):_ptr(ptr){
    if(_ptr){
      _count = new size_t(1);
    }else{
      _count = new size_t(0);
    }
  }

  //拷贝构造函数
  SmartPointer(const SmartPointer &ptr){
    if(this!=&ptr){
      this->_ptr = ptr._ptr;
      this->_count = ptr._count;
      (*this->_count)++;
    }
  }

  //重载=
  SmartPointer &operator=(const SmartPointer &ptr){
    if(this->_ptr == ptr._ptr)
      return *this;
    if(this->_ptr){
      (*this->_count)--;
      if(this->_count ==0){
	delete this->_ptr;
	delete this->_count;
      }
    }
    this->_ptr = ptr._ptr;
    this->_count = ptr._count;
    (*this->_count)++;
    return *this;
  }

  //重载*
  T &operator *(){
    //  assert(this->_ptr == nullptr);
    return *(this->_ptr);
  }

  //重载->
  T *operator->(){
    assert(this->_ptr == nullptr);
    return this->_ptr;
  }
  ~SmartPointer(){
    (*this->_count)--;
    if(*this->_ptr == 0){
      delete this->_count;
      delete this->_ptr;
	  std::cout<<"执行析构函数"<<std::endl;
    }
  }
  
  size_t use_count(){
    return *this->_count;
  }

};

int main(int argc,char **argv){
  
    SmartPointer<int> sp(new int(10));
    SmartPointer<int> sp2(sp);
    SmartPointer<int> sp3(new int(20));

    sp2 = sp3;

    std::cout<<*sp2<<std::endl;
    std::cout<<sp.use_count()<<std::endl;
    std::cout<<sp2.use_count()<<std::endl;
    std::cout<<sp3.use_count()<<std::endl;
  

  std::cout<<"adfasdf"<<std::endl;
  return 0;
}
