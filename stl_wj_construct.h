#ifndef __WJCONSTRUCT___
#define __WJCONSTRUCT___

#include<new>       //for placement new

namespace wj{

    //定义构造对象的construct
    template<class T1,class T2>
    inline void construct(T1* p,const T2& value){
        new(p) T1(value);       //placement new;调用T1::T1(value);
    }

    //定义析构对象的destory
    template<class T>
    inline void destory(T* p){
        p->~T();                //调用 dtor ~T()
    }

}

#endif