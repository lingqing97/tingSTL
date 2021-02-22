#ifndef __WJALGO__
#define __WJALGO__

/*
    本文件实现stl_algo.h部分算法
*/

#include<algorithm>

namespace wj{
    //实现set相关算法
    //求两个set并集
    template<class InputIterator1,class InputIterator2,class OutputIterator>
    OutputIterator set_union(InputIterator1 first1,InputIterator1 last1,
                                InputIterator2 first2,InputIterator2 last2,
                                OutputIterator result)
    {
        while(first1!=last1&&first2!=last2){
            if(*first1<*first2){
                *result=*first1;
                ++first1;
            }
            else if(*first2<*first1){
                *result=*first2;
                ++first2;
            }
            else{
                *result=*first1;
                ++first1;
                ++first2;
            }
            ++result;
        }
        //将尚未到达尾端的区间的所有剩余元素拷贝到目的端
        //此时[first2,last2)和[first1,last1)一定有一个是空的
        return std::copy(first2,last2,std::copy(first1,last1,result));
    }

    //求两个set交集
    template<class InputIterator1,class InputIterator2,class OutputIterator>
    OutputIterator set_intersection(InputIterator1 first1,InputIterator1 last1,
                                    InputIterator2 first2,InputIterator2 last2,
                                    OutputIterator result)
    {
        while(first1!=last1&&first2!=last2){
            if(*first1<*first2){
                ++first1;
            }
            else if(*first2<*first1){
                ++first2;
            }
            else{
                *result=*first1;
                ++result;
                ++first1;
                ++first2;
            }
        }
        return result;
    }

    //求两个set差集
    template<class InputIterator1,class InputIterator2,class OutputIterator>
    OutputIterator set_difference(InputIterator1 first1,InputIterator1 last1,
                                    InputIterator2 first2,InputIterator2 last2,
                                    OutputIterator result)
    {
        while(first1!=last1&&first2!=last2){
            if(*first1<*first2){
                *result=*first1;
                ++result;
                ++first1;
            }
            else if(*first2<*first1){
                ++first2;
            }
            else{
                ++first1;
                ++first2;
            }
        }
        return std::copy(first1,last1,result);
    }

    //求两个set对称差
    template<class InputIterator1,class InputIterator2,class OutputIterator>
    OutputIterator set_symmetric_difference(InputIterator1 first1,InputIterator1 last1,
                                    InputIterator2 first2,InputIterator2 last2,
                                    OutputIterator result)
    {
        while(first1!=last1&&first2!=last2){
            if(*first1<*first2){
                *result=*first1;
                ++result;
                ++first1;
            }
            else if(*first2<*first1){
                *result=*first2;
                ++result;
                ++first2;
            }
            else{
                ++first1;
                ++first2;
            }
        }
        return std::copy(first2,last2,std::copy(first1,last1,result));
    }


}

#endif /* __WJALGO__ */