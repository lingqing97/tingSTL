#ifndef __WJFUNCTION__
#define __WJFUNCTION__

namespace wj{
    //identity

    template<class Arg,class Result>
    struct unary_function{
        typedef Arg argument_type;
        typedef Result result_type;
    };

    template<class T>
    struct identity:public unary_function<T,T>
    {
        const T& operator()(const T& x) const { return x;}
    };

    //select1st
    template<class _Pair>
    struct select1st:public unary_function<_Pair,_Pair>{
        const typename _Pair::first_type operator()(const _Pair& _x) const{
            return _x.first;
        }
    };
}

#endif /*__WJFUNCTION__ */