#ifndef _WJALLOC_
#define _WJALLOC_

#include<new>               //for placement new,bad_alloc
#include<cstddef>           //for ptrdiff_t,size_t
#include<cstdlib>           //for exit(),malloc()
#include<climits>           //for UNIT_MAX
#include<iostream>          //for cerr

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
        for(;;){
            my_alloc_oom_handler=__malloc_alloc_oom_handler;
            if(my_alloc_oom_handler==0){
                throw std::bad_alloc();
            }
            (*my_alloc_oom_handler)();
            result=malloc(n);
            if(result) return result;
        }
    }


    template<typename T>
    class allocator{
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
                typedef allocator<U> other;
            };

            //allocate和deallocate
            pointer allocate(size_type n,const void* p=0){
                pointer buffer=(T*) malloc(n*sizeof(T));
                //内存不足这里不进行处理
                if(buffer==0){
                    /*
                    std::cerr<<"out of memory"<<std::endl;
                    exit(1);
                    */
                    buffer=(T*)oom_alloc(n*sizeof(T));
                }
                return buffer;
            }
            void deallocate(pointer p,size_type n){
                if(p!=nullptr){
                    free(p);
                }
            }
            //construct和destory
            void construct(pointer p,const T& value){
                new(p) T(value);            //使用placement new,在地址p出构造T,并将value值传给T
            }
            void destory(pointer p,size_type n){
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
}

#endif