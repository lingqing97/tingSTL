#ifndef __WJCONSTRUCT___
#define __WJCONSTRUCT___

#include<new>       //for placement new
#include"stl_wj_iterator.h"
#include"stl_wj_type_traits.h"

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

    //以下是destory()第二版本，接受两个迭代器。此函数设法找出元素的数值型别
    //判断元素的数值型别(value type)是否有trivial destructor
    template<class ForwardIterator,class T>
    inline void __destory(ForwardIterator first,ForwardIterator last,T*){
        typedef typename wj::__type_traits<T>::has_trivial_destructor trivial_destructor;
        __destory_aux(first,last,trivial_destructor());
    }

    //如果元素的数值型别(value type)有non-trivial destructor
    template<class ForwardIterator>
    inline void
    __destory_aux(ForwardIterator first,ForwardIterator last,wj::__false_type){
        for(;first<last;++first)
            destory(&*first);
    }

    //如果元素的数值型别(value type)有trivial destructor
    template<class ForwardIterator>
    inline void __destory_aux(ForwardIterator first,ForwardIterator last,wj::__true_type) {}

    //进而利用__type_traits<>求取最适当措施
    template<class ForwardIterator>
    inline void destory(ForwardIterator first,ForwardIterator last){
        __destory(first, last,wj::value_type(first));
    }

    //以下是destory()针对迭代器为char*和wchar_t*的特化版
    inline void destory(char*,char*) {}
    inline void destory(wchar_t*,wchar_t*) {}
}

#endif