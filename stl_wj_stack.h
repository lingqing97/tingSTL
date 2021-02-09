#ifndef __WJSTACK__
#define __WJSTACK__

/*
    本文件实现stack
*/

#include"stl_wj_deque.h"    //for class deque

namespace wj{
    template<class,class> class stack;      //前置声明
    template<class T,class Sequence>
    bool operator==(const stack<T, Sequence>& x,const stack<T, Sequence>& y){
        return x.c==y.c;
    }
    template<class T,class Sequence>
    bool operator<(const stack<T, Sequence>& x,const stack<T, Sequence>& y){
        return x.c<y.c;

    }
    template<class T,class Sequence=wj::deque<T>>
    class stack{
        //声明友元
        friend bool operator== <T,Sequence>(const stack<T, Sequence>& x,const stack<T, Sequence>& y);
        friend bool operator< <T, Sequence>(const stack<T, Sequence>& x,const stack<T, Sequence>& y);
        public:
            //一些声明
            typedef typename Sequence::value_type       value_type;
            typedef typename Sequence::size_type        size_type;
            typedef typename Sequence::reference        reference;
            typedef typename Sequence::const_reference  const_reference;
        protected:
            //底层容器
            Sequence c;
        public:
            //以下操作完全利用Sequence c的操作，完成stack的操作
            bool empty() const { return c.empty(); }
            size_type size() const { return c.size(); }
            reference top() { return c.back(); }
            const_reference top() const { return c.back(); }
            //stack是先进后出
            void push(const value_type& x) { c.push_back(x); }
            void pop() { c.pop_back(); }
    };
}


#endif  /* __WJSTACK__ */