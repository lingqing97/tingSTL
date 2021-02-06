#ifndef __WJVECTOR__
#define __WJVECTOR__

#include<algorithm>
#include"stl_memory.h"
#include<cstddef>           //for ptrdiff_t
#include<memory>            //for uninitialized_fill_n

namespace wj{
    template<class T,class Alloc=wj::alloc<T>>
    class vector{
        public:
            //常规声明
            typedef T                 value_type;
            typedef T*                pointer;
            typedef T&                reference;
            typedef const T&          const_reference;
            typedef ptrdiff_t         difference_type;
            typedef size_t            size_type;
            typedef value_type*       iterator;
            typedef const value_type* const_iterator;
        protected:
            //声明一个内存分配器
            typedef wj::simple_alloc<value_type,Alloc> data_allocator;

            iterator start;             //表示目前使用空间的头
            iterator finish;            //表示目前使用空间的尾
            iterator end_of_storage;    //表示目前可用空间的尾

        protected:
            //deallocate用于释放内存
            void deallocate(){
                if(start)
                    //注释：这里普通指针的差值是:地址差/sizeof(T)
                    data_allocator::deallocate(start,end_of_storage-start);
            }
            //配置空间并填满内容
            iterator allocate_and_fill(size_type n,const T& x){
                iterator result=data_allocator::allocate(n);
                std::uninitialized_fill_n(result,n,x);
                return result;
            }
            void fill_initialize(size_type n,const T& value){
                start=allocate_and_fill(n,value);
                finish=start+n;
                end_of_storage=finish;
            }
            void insert_aux(iterator position,const T& x);
        public:
            //常见的一些简单操作
            iterator begin() { return start; }
            const_iterator begin() const { return start; }

            iterator end() { return finish; }
            const_iterator end() const { return finish; }

            size_type size() const { return end()-begin(); }
            size_type capacity() const{ return end_of_storage-begin(); }

            bool empty() const { return begin()==end(); }
            reference operator[](size_type n) { return *(begin()+n); }

            reference front() { return *begin();}
            const_reference front() const { return *begin(); }

            reference back() { return *(end()-1); }
            const_reference back() const { return *(end()-1); }
        public:
            vector():start(nullptr),finish(nullptr),end_of_storage(nullptr){}
            vector(size_type n,const T& value) { fill_initialize(n,value); }
            vector(int n,const T& value) { fill_initialize(n,value); }
            vector(long n,const T& value) { fill_initialize(n,value); }
            explicit vector(size_type n) { fill_initialize(n,T());}
            ~vector(){
                wj::destory(start,finish);      //析构
                deallocate();                   //释放内存
            }
        public:
            //插入和删除相关操作
            void push_back(const T& x){
                if(finish!=end_of_storage){
                    //还有备用空间
                    wj::construct(finish,x);
                    ++finish;
                }
                else{
                    //没有备用空间
                    insert_aux(end(),x);
                }
            }
            //从position开始，插入n个元素，元素初值为x
            void insert(iterator position,size_type n,const T& x);
            //将尾端元素拿掉，并调整大小
            void pop_back(){
                --finish;
                wj::destory(finish);
            }
            //清除[first,last)中的所有元素
            iterator erase(iterator first,iterator last){
                iterator i=std::copy(last,finish,first);
                wj::destory(i,finish);
                finish=finish-(last-first);
                return first;
            }
            //清除某个位置上的元素
            iterator erase(iterator position){
                if(position+1!=end())
                    std::copy(position+1,finish,position);
                --finish;
                wj::destory(finish);
                return position;
            }
            void clear() { erase(begin(),end()); }
    };
    template<class T,class Alloc>
    void vector<T,Alloc>::insert_aux(iterator position,const T& x){
        if(finish!=end_of_storage){
            //还有备用空间
            //在备用空间起始处构造一个元素，并以vector最优一个元素值为其初值
            wj::construct(finish,*(finish-1));
            //调整水位
            ++finish;
            T x_copy=x;
            //将positionk和其后面元素的位置往后挪
            std::copy_backward(position,finish-2,finish-1);
            *position=x_copy;
        }
        else{
            //已无备用空间
            const size_type old_size=size();
            //采用二倍扩充的原则
            const size_type len=old_size!=0?2*old_size:1;
            iterator new_start=data_allocator::allocate(len);
            iterator new_finish=new_start;
            try{
                //复制前半段内容到新vector
                new_finish=std::uninitialized_copy(start,position,new_start);
                //为新元素设定初值x
                wj::construct(new_finish,x);
                ++new_finish;
                //复制后半段内容到新vector
                new_finish=std::uninitialized_copy(position,finish,new_finish);
            }
            catch(...){
                //处理异常
                wj::destory(new_start,new_finish);
                data_allocator::deallocate(new_start,len);
                throw;
            }
            //析构并释放原vector
            wj::destory(begin(),end());
            deallocate();
            //调整迭代器，指向新vector
            start=new_start;
            finish=new_finish;
            end_of_storage=new_start+len;
        }
    }
    template<class T,class Alloc>
    void vector<T,Alloc>::insert(iterator position,size_type n,const T& x){
        if(n!=0){
            if(size_type(end_of_storage-finish)>=n){
                //备用空间大于等于“新增元素个数”
                T x_copy=x;
                //以下计算插入点之后的现有元素个数
                const size_type elems_after=finish-position;
                iterator old_finish=finish;
                if(elems_after>n){
                    //“插入点之后的现有元素个数”大于“新增元素个数”
                    std::uninitialized_copy(finish-n,finish,finish);
                    finish+=n;      //将vectro尾段标记后移
                    std::copy_backward(position,old_finish-n,old_finish);
                    std::fill(position,position+n,x_copy);      //从插入点开始填入新值
                }
                else{
                    //"插入点之后的现有元素个数"小于等于“新增元素个数”
                    std::uninitialized_fill_n(finish,n-elems_after,x_copy);
                    finish+=n-elems_after;
                    std::uninitialized_copy(position,old_finish,finish);
                    finish+=elems_after;
                    std::fill(position,old_finish,x_copy);
                }
            }
            else{
                //备用空间小于“新增元素个数”
                //首先决定新长度：旧长度的两倍，或旧长度+新增元素个数
                const size_type old_size=size();
                const size_type len=old_size+(old_size>n?old_size:n);
                //配置新的vector空间
                iterator new_start=data_allocator::allocate(len);
                iterator new_finish=new_start;
                try{
                    //首先向就vector的插入点之前的元素复制到新空间
                    new_finish=std::uninitialized_copy(start,position,new_start);
                    //再将新增元素（初值皆为n）填入新空间
                    new_finish=std::uninitialized_fill_n(new_finish,n,x);
                    //再将旧vector的插入点之后的元素复制到新空间
                    new_finish=std::uninitialized_copy(position,finish,new_finish);
                }
                catch(...){
                    wj::destory(new_start,new_finish);
                    data_allocator::deallocate(new_start,len);
                    throw;
                }
                //清除并释放旧的vector
                destory(start,finish);
                deallocate();
                start=new_start;
                finish=new_finish;
                end_of_storage=new_start+len;
            }
        }
    }
}


#endif /* __WJVECTOR__ */