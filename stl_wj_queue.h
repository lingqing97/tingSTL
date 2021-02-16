#ifndef __WJQUEUE__
#define __WJQUEUE__

/*
    本文件实现queue
*/

#include"stl_wj_deque.h"    //for class deque

namespace wj{
    template<class,class> class queue;          //前置声明

    template<class T,class Sequence>
    bool operator==(const queue<T, Sequence>& x,const queue<T, Sequence>& y){
        return x.c==y.c;
    }

    template<class T,class Sequence>
    bool operator<(const queue<T, Sequence>& x,const queue<T, Sequence>& y){
        return x.c<y.c;
    }

    template<class T,class Sequence=wj::deque<T>>
    class queue{
        //声明友元
        friend bool operator== <T, Sequence>(const queue<T, Sequence>& x,const queue<T, Sequence>& y);
        friend bool operator< <T, Sequence>(const queue<T, Sequence>& x,const queue<T, Sequence>& y);
        public:
            typedef typename Sequence::value_type       value_type;
            typedef typename Sequence::size_type        size_type;
            typedef typename Sequence::reference        reference;
            typedef typename Sequence::const_reference  const_reference;
        protected:
            Sequence c;     //底层容器
        public:
            //以下操作完全利用Sequence c的操作，完成queue的操作
            bool empty() const { return c.empty(); }
            size_type size() const { return c.size(); }
            reference front() { return c.front(); }
            const_reference front() const { return c.front(); }
            reference back()  { return c.back(); }
            const_reference back() const { return c.back(); }
            //queue是先进先出
            void push(const value_type& x) { c.push_back(x); }
            void pop() { c.pop_front(); }
    };

}

#endif /* __WJDEQUE__ */