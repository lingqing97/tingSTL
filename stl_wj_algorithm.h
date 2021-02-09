#ifndef __WJALGORITHM__
#define __WJALGORITHM__

/*
    本文件实现部分常用算法
*/

#include"stl_wj_vector.h"   //for class vector

namespace wj{
    //heap operations.

    template<class RandomAccessIterator,class Distance,class T>
    void __push_heap(RandomAccessIterator first,Distance holdIndex,
                     Distance topIndex, T value)
    {
        Distance parent=(holdIndex-1)/2;    //找出父节点下标
        while(holdIndex>topIndex && *(first+parent)<value){
            //将父节点下移
            *(first+holdIndex)=*(first+parent);
            //计算下一个父节点
            holdIndex=parent;
            parent=(holdIndex-1)/2;
        }
        //找到属于自己的位置,令其值为values
        *(first+holdIndex)=value;
    }

    template<class RandomAccessIterator,class Distance,class T>
    inline void __push_heap_aux(RandomAccessIterator first,
                                RandomAccessIterator last, Distance*, T*)
    {
        //Distance((last-first)-1)即最后一个元素（即已经插入到尾端的元素）的下标
        __push_heap(first,Distance((last-first)-1),Distance(0),T(*(last-1)));
    }

    template<class RandomAccessIterator>
    inline void push_heap(RandomAccessIterator first,RandomAccessIterator last){
    //    push_heap算法：接受两个迭代器，用来表现一个heap底部容器vector的头尾[first,last)，
    //                  并且新元素已经插入到底部容器的最尾端
        __push_heap_aux(first,last,distance_type(first),value_type(first));
    }

    template<class RandomAccessIterator,class Distance,class T>
    void __adjust_heap(RandomAccessIterator first,Distance holdIndex,Distance len,T value){
        //以下操作分为两步:
        //step1: 将根节点的值下降到最尾端
        //step2: 执行一次up操作以维持heap的次序特性
        Distance topIndex=holdIndex;
        Distance secondChild=2*holdIndex+2;     //洞节点的右孩子节点
        while(secondChild<len){
            //取左右孩子节点中的较大值
            if(*(first+secondChild)<*(first+(secondChild-1)))
                secondChild--;
            //用左/右孩子的值取代洞值
            *(first+holdIndex)=*(first+secondChild);
            holdIndex=secondChild;
            secondChild=2*holdIndex+2;
        }
        if(secondChild==len)    //没有右子节点，只有左子节点
        {
            //令洞值为左孩子值
            *(first+holdIndex)=*(first+secondChild-1);
            holdIndex=secondChild-1;
        }
        //此时不一定满足heap的次序特性，需要进行一个up操作
        __push_heap(first,holdIndex,topIndex,value);
    }

    template<class RandomAccessIterator,class T,class Distance>
    inline void __pop_heap(RandomAccessIterator first,
                           RandomAccessIterator last,
                           RandomAccessIterator result,
                           T value, Distance*)
    //[first,last)表示取值后需要调整的heap的范围
    //result表示所取得的最大值
    //value表示需要放到头部用于重调heap的值
    {
        *result=*first;
        //重新调整heap,洞号为0(亦即树根处),欲调整值为value(原尾值)
        __adjust_heap(first,Distance(0),Distance(last-first),value);
    }

    template<class RandomAccessIterator,class T>
    inline void __pop_heap_aux(RandomAccessIterator first,
                               RandomAccessIterator last, T*)
    {
        //首先设定欲调整值为尾值，然后将首值调至尾节点，然后重整[first,last-1)使之重新成一个合格的heap
        __pop_heap(first,last-1,last-1,T(*(last-1)),distance_type(first));
    }

    template<class RandomAccessIterator>
    inline void pop_heap(RandomAccessIterator first,RandomAccessIterator last){
        //pop_heap算法:接受两个迭代器，用来表现一个heap底部容器vector的头尾
        __pop_heap_aux(first,last,value_type(first));
    }

    template<class RandomAccessIterator>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        //这里只比较到只剩两个元素，是因为，当只剩两个元素时，这两个元素已经有序了
        while(last-first>1){
            //没执行一次pop_heap,操作范围缩小一格
            wj::pop_heap(first,last--);
        }
    }

    template<class RandomAccessIterator,class T,class Distance>
    void __make_heap(RandomAccessIterator first,
                     RandomAccessIterator last,
                     T*, Distance*)
    {
        if(last-first<2) return;    //如果长度为0或1,不必重新排序
        Distance len=last-first;
        //找出最后一个非叶子节点
        Distance parent=(len-2)/2;
        while(true){
            __adjust_heap(first,parent,len,T(*(first+parent)));
            if(parent==0) return;   //走完根节点，退出循环
            parent--;               //移动到前一个非叶子节点
        }
    }

    template<class RandomAccessIterator>
    inline void make_heap(RandomAccessIterator first,RandomAccessIterator last)
    {
        __make_heap(first,last,value_type(first),distance_type(first));
    }
}


#endif /* __WJALGORITHM__ */