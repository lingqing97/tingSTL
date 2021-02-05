#ifndef __WJITERATOR__
#define __WJITERATOR__

/*--------------------------------未测试版本--------------------------------*/

#include<cstddef>       //for ptrdiff_t

namespace wj{

    //定义五种迭代器标记
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag:public input_iterator_tag{};
    struct bidirectional_iterator_tag:public forward_iterator_tag{};
    struct random_access_iterator_tag:public bidirectional_iterator_tag{};

    //定义迭代器模板
    template<class category,
            class T,
            class Distance=ptrdiff_t,
            class Pointer=T*,
            class Reference=T&>
    struct iterator{
        typedef category        iterator_category;
        typedef T               value_type;
        typedef Distance        difference_type;
        typedef Pointer         pointer;
        typedef Reference       reference;
    };

    //定义特性萃取剂 iterator_traits
    template<class Iterator>
    struct iterator_traits{
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type        value_type;
        typedef typename Iterator::difference_type   difference_type;
        typedef typename Iterator::pointer           pointer;
        typedef typename Iterator::reference         reference;
    };

    //针对原生指针而设计的偏特化版本
    template<class T>
    struct iterator_traits<T*>{
        typedef random_access_iterator_tag           iterator_category;
        typedef T                                    value_type;
        typedef ptrdiff_t                            difference_type;
        typedef T*                                   pointer;
        typedef T&                                   reference;
    };
    //针对const原生指针而设计的偏特化版本
    template<class T>
    struct iterator_traits<const T*>{
        typedef random_access_iterator_tag           iterator_category;
        typedef T                                    value_type;
        typedef ptrdiff_t                            difference_type;
        typedef const T*                             pointer;
        typedef const T&                             reference;
    };
    //这个函数可以很方便地决定某个迭代器的类型
    template<class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&){
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    //这个函数可以很方便决定某个迭代器的distance type
    template<class Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&){
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    //这个函数可以很方便地决定某个迭代器的value type
    template<class Iterator>
    inline typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&){
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    //以下是整组的distance函数
    template<class InputIterator,class Distance>
    inline void
    __distance(InputIterator first,InputIterator last,Distance &n,input_iterator_tag){
        while(first!=last){
            ++first;
            ++n;
        }
    }

    template<class RandomAccessIterator,class Distance>
    inline void
    __distance(RandomAccessIterator first,RandomAccessIterator last,Distance &n,random_access_iterator_tag){
        n+=last-first;
    }

    template<class InputIterator,class Distance>
    inline void
    distance(InputIterator first,InputIterator last,Distance &n){
        typedef typename iterator_traits<InputIterator>::iterator_category iterator_category;
        __distance(first,last,n,iterator_category());
    }

    //以下是整组advance函数
    template<class InputIterator, class Distance>
    inline void
    __advance(InputIterator& i, Distance n,input_iterator_tag){
        while(n--) ++i;
    }

    template<class BidirectionalIterator,class Distance>
    inline void
    __advance(BidirectionalIterator& i,Distance n,bidirectional_iterator_tag){
        if(n>=0){
            while(n--) ++i;
        }
        else{
            while(n++) --i;
        }
    }

    template<class InputIterator,class Distance>
    inline void
    advance(InputIterator& i,Distance n){
        typedef typename iterator_traits<InputIterator>::iterator_category iterator_category;
        __advance(i,n,iterator_category());
    }

}

#endif