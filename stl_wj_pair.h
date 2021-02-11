#ifndef __WJPAIR__
#define __WJPAIR__

/*
    本文件实现pair
*/

namespace wj{
    template<class T1,class T2>
    struct pair{
        typedef T1 first_type;
        typedef T2 second_type;

        T1 first;
        T2 second;

        pair():first(T1()),second(T2()) {}
        pair(const T1& a,const T2& b):first(a),second(b) {}
        template<class U1,class U2>
        pair(const pair<U1,U2>& p):first(p.first),second(p.second){}
    };

    template<class T1,class T2>
    inline bool operator==(const pair<T1,T2>& x,const pair<T1,T2>& y){
        return x.first==y.first && x.second==y.second;
    }

    template<class T1,class T2>
    inline bool operator<(const pair<T1,T2>& x,const pair<T1,T2>& y){
        //x的第一元素小于y的第一元素，或x的第一元素不大于y的第一元素而
        //x的第二元素小于y的第二元素，才视为x小于y
        return x.first<y.first || (!(y.first<x.first)&&x.second<y.second);
    }

    template<class T1,class T2>
    inline pair<T1,T2> make_pair(const T1& x,const T2& y){
        return pair<T1,T2>(x,y);
    }
}

#endif /* __WJPAIR__ */