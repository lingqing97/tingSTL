#ifndef __WJALGOBASE__
#define __WJALGOBASE__

/*
    本文件实现stl_algobase.h的部分算法
*/

namespace wj{

    //lexicographic_compare
    template<class InputIterator1,class InputIterator2>
    bool lexicographic_compare(InputIterator1 first1,InputIterator1 last1,
                        InputIterator2 first2,InputIterator2 last2)
    {
        for(;first1 != last1 && first2!=last2; ++first1,++first2){
            if(*first1<*first2){
                return true;
            }
            if(*first1>*first2){
                return false;
            }
        }
        return first1==last1&&first2!=last2;
    }

    template<class InputIterator1,class InputIterator2,class Compare>
    bool lexicographic_compare(InputIterator1 first1,InputIterator1 last1,
                        InputIterator2 first2,InputIterator2 last2,Compare comp)
    {
        for(;first1 != last1 && first2!=last2; ++first1,++first2){
            if(comp(*first1,*first2)){
                return true;
            }
            if(comp(*first2,*first1)){
                return false;
            }
        }
        return first1==last1&&first2!=last2;
    }

    //swap
    template<class T>
    inline void swap(T& a,T& b){
        T tmp=a;
        a=b;
        b=tmp;
    }
}


#endif /* __WJALGOBASE__ */