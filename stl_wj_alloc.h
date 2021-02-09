#ifndef _WJALLOC_
#define _WJALLOC_

#include<new>               //for placement new,bad_alloc
#include<cstddef>           //for ptrdiff_t,size_t
#include<cstdlib>           //for exit(),malloc()
#include<climits>           //for UNIT_MAX
#include<iostream>          //for cerr

/*
    这里写的alloc只是一个toy,不具有参考意义
*/

namespace wj{
    //声明new handler函数
    void (* __malloc_alloc_oom_handler)()=0;
    //仿真C++的set_new_handler
    void (* set_malloc_handler(void (*f)()))()
    {
        void (*old)()=__malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler=f;
        return old;
    }
    //定义内存不足处理函数
    void* oom_alloc(size_t n){
        void (*my_alloc_oom_handler)();
        void *result;
        for(;;){            //不断尝试释放，配置，再释放，再配置...
            my_alloc_oom_handler=__malloc_alloc_oom_handler;
            if(my_alloc_oom_handler==0){
                //未配置new handler函数，直接抛出异常
                throw std::bad_alloc();
            }
            (*my_alloc_oom_handler)();          //调用new handler尝试获得更多内存
            result=malloc(n);                   //再次尝试配置内存
            if(result) return result;
        }
    }


    template<typename T>
    class alloc{
        public:
            //一组typedef
            typedef T               value_type;
            typedef T*              pointer;
            typedef const T*        const_pointer;
            typedef T&              reference;
            typedef const T&        const_reference;
            typedef size_t          size_type;
            typedef ptrdiff_t       difference_type;

            //定义内置的模板版本
            template<typename U>
            struct rebind {
                typedef alloc<U> other;
            };

            //allocate和deallocate
            static pointer allocate(size_type n,const void* p=0){
                pointer buffer=(T*) malloc(n);
                //内存不足这里不进行处理
                if(buffer==0){
                    /*
                    std::cerr<<"out of memory"<<std::endl;
                    exit(1);
                    */
                    buffer=(T*)oom_alloc(n);
                }
                return buffer;
            }
            static void deallocate(pointer p,size_type n){
                if(p!=nullptr){
                    free(p);
                }
            }
            //construct和destory
            static void construct(pointer p,const T& value){
                new(p) T(value);            //使用placement new,在地址p出构造T,并将value值传给T
            }
            static void destory(pointer p,size_type n){
                p->~T();                    //调用T的析构函数销毁之
            }
            //mas_size
            size_type max_size() const{
                return size_type(UINT_MAX/sizeof(T));
            }
            //address和const_address
            pointer address(reference x){
                return (pointer)(&x);
            }
            const_pointer const_address(const_reference x){
                return (const_address)(&x);
            }
    };

    //对alloc进行一个简单的封装
    template<class T,class Alloc=alloc<T>>
    struct simple_alloc{
        static T* allocate(size_t n){
            return (n==0)?nullptr:Alloc::allocate(n*sizeof(T));
        };
        static T* allocate(void){
            return Alloc::allocate(sizeof(T));
        }
        static void deallocate(T* p,size_t n){
            if(n!=0) Alloc::deallocate(p,n*sizeof(T));
        }
        static void deallocate(T* p){
            Alloc::deallocate(p,sizeof(T));
        }
    };
}

#endif