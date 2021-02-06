#ifndef __WJARRAY__
#define __WJARRAY__

/*
    本文件实现array
*/

#include<cstddef>       //for std::size_t

namespace wj{
    template<class T,std::size_t N>
    struct array{
        typedef T                 value_type;
        typedef T*                pointer;
        typedef value_type*       iterator;
        typedef const value_type* const_iterator;
        typedef size_t            size_type;
        typedef T&                reference;

        //内部数据
        value_type _M_instance[N?N:1];

        iterator begin() { return iterator(&_M_instance[0]); }
        const_iterator cbegin() const { return iterator(&_M_instance[0]); }
        iterator end() { return iterator(&_M_instance[N]); }
        const_iterator cend() const { return iterator(&_M_instance[N]); }

        //capacity
        size_type size() const { return N; }
        size_type max_size() const { return N; }
        bool empty() const { return size()==0; }

        //element access
        reference operator[](size_type n) { return _M_instance[n]; }
        reference at(size_type n) {
            if(n<0||n>=N){
                //这里只是将异常抛出
                throw;
            }
            return _M_instance[n];
        }
        reference front() { return _M_instance[0]; }
        reference back() { return _M_instance[N-1]; }
    };
}


#endif