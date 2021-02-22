#ifndef __WJNUMERIC__
#define __WJNUMERIC__

/*
    本文件实现stl_numeric.h部分算法
*/

#include"stl_wj_function.h" //for multiplies

namespace wj{

    //accumulate
    template<class InputIterator,class T>
    T accumulate(InputIterator first, InputIterator last, T init){
        for(;first!=last;++first)
            init=init+ *first;
        return init;
    }

    template<class InputIterator,class T,class BinaryOperation>
    T accumulate(InputIterator first,InputIterator last,T init,BinaryOperation op){
        for(;first!=last;++first)
            init=op(init,*first);
        return init;
    }

    //power
    template<class T,class Interger,class MonoidOperation>
    T power(T x, Interger n, MonoidOperation op){       //快速幂算法
        if(n==0)
            return 1;
            //TODO
            //return wj::identity(op);
        else{
            while((n&1)==0){
                n>>=1;
                x=op(x,x);
            }
            T result=x;
            n>>=1;
            while(n!=0){
                x=op(x,x);
                if((n&1)!=0)
                    result=op(result,x);
                n>>=1;
            }
            return result;
        }
    }

    template<class T,class Interger>
    inline T power(T x, Interger n){
        return power(x,n,wj::multiplies<T>());
    }
}

#endif /* __WJNUMERIC_ */