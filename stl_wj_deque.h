#ifndef __WJDEQUE__
#define __WJDEQUE__

/*
    本文件实现deque
*/

#include<cstddef>       //for size_t
#include"stl_memory.h"
#include<memory>        //for uninitialized_fill
#include<algorithm>    //for copy() and copy_backward()

namespace wj{
    //设置缓冲区大小
    //如果n不为0，则缓冲区大小由使用者设定
    //否则，由程序设定
    inline size_t __deque_buf_size(size_t n,size_t sz)
    {
        return n!=0?n:(sz<512?size_t(512/sz):size_t(1));
    }
    //定义deque的迭代器
    template<class T,class Ref,class Ptr,size_t BufSiz>
    struct __deque_iterator{

        typedef __deque_iterator<T,T&,T*,BufSiz> iterator;
        typedef __deque_iterator<T,const T&,const T*,BufSiz> const_iterator;
        static size_t buffer_size() { return __deque_buf_size(BufSiz,sizeof(T)); }

        //定义五种型别
        typedef wj::random_access_iterator_tag      iterator_category;
        typedef T                                   value_type;
        typedef T*                                  pointer;
        typedef T&                                  reference;
        typedef ptrdiff_t                           difference_type;

        //其他声明
        typedef size_t                              size_type;
        typedef T**                                 map_pointer;
        typedef __deque_iterator                    self;

        //声明位置标兵
        T*      cur;            //指向当前缓冲区的当前元素
        T*      first;          //指向当前缓冲区的头部
        T*      last;           //指向当前缓冲区的尾部
        map_pointer node;       //指向缓冲区管控中心

        //ctor
        __deque_iterator():cur(nullptr),first(nullptr),last(nullptr),node(nullptr){}
        __deque_iterator(const iterator& x):cur(x.cur),first(x.first),last(x.last),node(x.node){}
        __deque_iterator(T* x,map_pointer y):cur(x),first(*y),last(*y+buffer_size()),node(y){}

        //函数目的：跳到另一个缓冲区
        void set_node(map_pointer new_node){
            node=new_node;
            first=*new_node;
            last=first+difference_type(buffer_size());
        }

        //以下运算符重载使得deque看起来是连续存储的

        reference operator*() const { return *cur; }
        pointer operator->() const { return &(operator*()); }   //STL的经典操作，调用operator*实现operator->

        //两个iterator之间的距离等于:
        //(1)两个iterator之间的buffers的总长度
        //(2)itr至其buffer头部的长度
        //(3)x至其buffef尾部的长度
        difference_type operator-(const self& x) const{
            return difference_type(buffer_size())*(node-x.node-1)+(cur-first)+(x.last-x.cur);
            //                     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   ^^^^^^^^^^  ^^^^^^^^^^^^
            //                     首尾buffers之间的总长度   当前buffer到头部的长度  x到其buffer末尾的长度
        }

        self& operator++(){
            ++cur;                  //移动到下一个元素
            if(cur==last){          //如果已达所在缓冲区的尾端
                set_node(node+1);   //就切换至下一缓冲区
                cur=first;          //的第一个元素
            }
            return *this;
        }

        self operator++(int){
            self tmp=*this;
            ++*this;                //STL中的标准实现都是后置版本调用前置版本
            return tmp;
        }

        self& operator--(){
            if(cur==first){         //如果已达所在缓冲区的头部
                set_node(node-1);   //就切换至前一缓冲区
                cur=last;           //的最后一个元素
            }
            --cur;                  //由于采用前闭后开，所以cur应该移动至尾指针的前一个元素
            return *this;
        }

        self  operator--(int){
            self tmp=*this;
            --*this;
            return tmp;
        }

        //下面实现随机存取，迭代器可以直接跳跃n个距离
        self& operator+=(difference_type n){
            difference_type offset=n+(cur-first);
            if(offset>=0&&offset<difference_type(buffer_size())){
                //目标位置在同一缓冲区内
                cur+=n;
            }
            else{
                //目标位置不在同一缓冲区内
                //计算缓冲区的offset
                difference_type node_offset=
                    offset>0?offset/difference_type(buffer_size()):-difference_type((-offset-1)/buffer_size())-1;
                //跳转至正确的缓冲区
                set_node(node+node_offset);
                //切换至正确的元素
                cur=first+(offset-node_offset*difference_type(buffer_size()));
            }
            return *this;
        }

        self operator+(difference_type n) const{
            self tmp=*this;
            return tmp+=n;
        }
        self& operator-=(difference_type n){
            //利用operator+=实现
            return *this+=-n;
        }
        self operator-(difference_type n) const{
            self tmp=*this;
            //调用operator-=实现
            return tmp-=n;
        }
        reference operator[](difference_type n) const { return *(*this+n); }

        //以下是重载比较运算符
        bool operator==(const self& x) const { return cur==x.cur; }
        bool operator!=(const self& x) const { return !(*this==x); }
        bool operator<(const self& x) const {
            return (node==x.node)?(cur<x.cur):(node<x.node);
        }
    };

    //定义deque
    template<class T,size_t BufSiz=0>
    class deque{
        public:
            //一些声明
            typedef T               value_type;
            typedef value_type*     pointer;
            typedef T&              reference;
            typedef const T&        const_reference;
            typedef size_t          size_type;
            typedef ptrdiff_t       difference_type;
        public:
            typedef __deque_iterator<T,T&,T*,BufSiz> iterator;
        protected:
            //元素的指针的指针
            typedef pointer*        map_pointer;
        protected:
            //Data members
            iterator        start;      //指向第一个缓存区的迭代器
            iterator        finish;     //指向最后一个缓冲区的迭代器
            map_pointer     map;        //指向map,map是块连续空间
                                        //其每个元素都是指针，指向一个缓冲区
            size_type       map_size;   //map内有多少指针，亦即多少缓冲区
        public:
            //一些简单的操作
            iterator begin() { return start; }          //返回首尾
            iterator end()   { return finish; }         //迭代器
            reference operator[](size_type n){
                //调用__deque_iterator<>::opeartor[]
                return start[difference_type(n)];
            }
            reference front() { return *start; }    //调用__deque_iterator<>::opeartor*
            reference back(){
                iterator tmp=finish;
                --tmp;
                return *tmp;
            }
            size_type size() const { return finish-start; }
            size_type max_size() const { return size_type(-1); }
            bool empty() const { return finish==start; }
        protected:
            //deque的内存管理

            //计算缓冲区大小
            static size_type buffer_size() { return __deque_buf_size(BufSiz,sizeof(T)); }
            //缓冲区默认最小size
            static size_type initial_map_size() { return 8; }

            //专属之空间配置器，每次配置一个元素大小
            typedef wj::simple_alloc<value_type,wj::alloc<value_type>>  data_allocator;
            //专属之空间配置器，每次配置一个指针大小
            typedef wj::simple_alloc<pointer,wj::alloc<pointer>>     map_allocator;

            //分配一个缓冲区内存
            pointer allocate_node() { return data_allocator::allocate(buffer_size()); }
            //释放一个缓冲区内存
            void deallocate_node(pointer p)  { data_allocator::deallocate(p,buffer_size()); }

            //函数目的:负责产生并安排好deque的结构，并将元素的初值设定妥当
            void fill_initialize(size_type,const value_type& value);
            //函数目的：负责产生并安排好deque的结构
            void create_map_and_nodes(size_type);
            //函数目的：销毁已经产生并安排好的deque结果
            void destroy_map_and_nodes(){
                for(map_pointer n=start.node;n<=finish.node;++n)
                    deallocate_node(*n);
                map_allocator::deallocate(map,map_size);
            }

        public:
            //ctor
            deque():start(),finish(),map(nullptr),map_size(0){
                //这里会分配一个缓冲区
                create_map_and_nodes(0);
            }
            deque(int n,const value_type& value):start(),finish(),map(nullptr),map_size(0){
                fill_initialize(n,value);
            }
            deque(const deque& other):start(),finish(),map(nullptr),map_size(0){
                create_map_and_nodes(other.size());
                try{
                    std::uninitialized_copy(other.begin(),other.end(),start);
                }
                catch(...){
                    destroy_map_and_nodes();
                }
            }
            ~deque(){
                wj::destory(start,finish);
                destroy_map_and_nodes();
            }
        protected:
            //重新分配map
            void reserve_map_at_back(size_type nodes_to_add=1){
                if(nodes_to_add+1>map_size-(finish.node-map))
                    //如果map尾端的节点备用空间不足
                    //则重换一个map(配置更大的，拷贝原来的，释放原来的)
                    reallocate_map(nodes_to_add,false);
            }
            void reserve_map_at_front(size_type nodes_to_add=1){
                if(nodes_to_add>start.node-map)
                    //符合条件则重换一个map
                    reallocate_map(nodes_to_add,true);
            }
            //函数目的：为map申请更多节点或重换一个更大的map
            void reallocate_map(size_type,bool);
        public:
            //push_back和push_front
            void push_back(const value_type& t){
                if(finish.cur!=finish.last-1){
                    //最后缓冲区尚有一个以上的备用空间
                    wj::construct(finish.cur,t);
                    ++finish.cur;
                }
                else{
                    //最后缓冲区已无（或只剩一个）元素备用空间
                    push_back_aux(t);
                }
            }

            void push_front(const value_type& t){
                if(start.cur!=start.first){
                    //第一缓冲区尚有备用空间
                    wj::construct(start.cur-1,t);
                    --start.cur;
                }
                else{
                    //第一缓冲区已无备用空间
                    push_front_aux(t);
                }
            }

            //pop_back和pop_front
            //注意:对于finish节点cur指针指向当前元素的后一个位置
            //     而对于start节点cur指针就指向当前元素
            void pop_back(){
                if(finish.cur!=finish.first){
                    //最后缓冲区还有一个或更多元素
                    --finish.cur;
                    wj::destory(finish.cur);        //元素析构
                }
                else{
                    //最后缓冲区没有任何元素
                    pop_back_aux();
                }
            }

            void pop_front(){
                if(start.cur!=start.last-1){
                    //第一缓冲区还有两个或更多元素
                    wj::destory(start.cur);         //元素析构
                    ++start.cur;
                }
                else{
                    //第一缓冲区没有一个元素
                    pop_front_aux();
                }
            }
            //insert
            //函数目的：在postition处插入一个元素
            iterator insert(iterator position,const value_type& x){
                if(position.cur==start.cur){
                    //如果插入点时deque最前端交给push_front去做
                    push_front(x);
                    return start;
                }
                else if(position.cur==finish.cur){
                    //如果插入点时deque最尾段交给push_back去做
                    push_back(x);
                    iterator tmp=finish;
                    //这里为什么要--
                    --tmp;
                    return tmp;
                }
                else{
                    //交给insert_aux去做
                    return insert_aux(position,x);
                }
            }
        protected:
            void push_back_aux(const value_type& t);
            void push_front_aux(const value_type& t);
            void pop_back_aux();
            void pop_front_aux();
            iterator insert_aux(iterator pos,const value_type& x);
    };

    template<class T,size_t BufSiz>
    void deque<T,BufSiz>::fill_initialize(
        size_type n,const value_type& value)
    {
        create_map_and_nodes(n);
        map_pointer cur;
        try{
            //为每个节点的缓冲区设定初值
            for(cur=start.node;cur<finish.node;++cur)
                std::uninitialized_fill(*cur,*cur+buffer_size(),value);
            //最后一个节点的设定稍有不同
            std::uninitialized_fill(finish.first,finish.cur,value);
        }
        catch(...){
            for(map_pointer n=start.node;n<cur;++n)
                wj::destory(*n,*n+buffer_size());
            destroy_map_and_nodes();
            throw;
        }
    }

    template<class T,size_t BufSiz>
    void deque<T,BufSiz>::create_map_and_nodes(size_type num_elements)
    {
        //需要节点数=(元素个数/每个缓冲区可容纳的元素个数)+1
        //如果刚好整除，会多配一个节点
        size_type num_nodes=num_elements/buffer_size()+1;
        //一个map最少有8个节点
        //正常情况下分配num_nodes+2个节点，这里加2是为了保证map始终含有偶数个节点
        map_size=(initial_map_size()>num_nodes+2?initial_map_size():num_nodes+2);
        //分配内存给map,map中的元素是指针，返回map的首地址
        map=map_allocator::allocate(map_size);
        //nstart和nfinish指向map所拥有之全部节点的最中央区段
        //保持在最中央，可使头尾两端的扩充能量一样大
        map_pointer nstart=map+(map_size-num_nodes)/2;
        map_pointer nfinish=nstart+num_nodes-1;

        map_pointer cur;
        try{
            //为map内的每个现有节点配置缓冲区，所有缓冲区加起来都是deque的
            //可用空间(最后一个缓冲区可能会有富余空间)
            for(cur=nstart;cur<=nfinish;++cur)
                *cur=allocate_node();
        }
        catch(...){
            //"commit or rollback"
            for(map_pointer n=nstart;n<cur;++n)
                deallocate_node(*n);
            map_allocator::deallocate(map,map_size);
            throw;
        }
        //为deque内的两个迭代器start和end设定正确内容
        start.set_node(nstart);
        finish.set_node(nfinish);
        //start.cur指向first,finisth.cur指向未整除完的部分的末尾
        start.cur=start.first;
        finish.cur=finish.first+num_elements%buffer_size();
    }

    template<class T,size_t BufSiz>
    void deque<T,BufSiz>::reallocate_map(size_type nodes_to_add,
                                        bool add_at_front)
    {
        size_type old_num_nodes=finish.node-start.node+1;
        size_type new_num_nodes=old_num_nodes+nodes_to_add;

        map_pointer new_nstart;
        if(map_size>2*new_num_nodes){
            //map还有足够的剩余空间
            new_nstart=map+(map_size-new_num_nodes/2)+(add_at_front?nodes_to_add:0);
            if(new_nstart<start.node){
                //左闭右开，所以这里要加一
                std::copy(start.node,finish.node+1,new_nstart);
            }
            else{
                std::copy_backward(start.node,finish.node+1,new_nstart+old_num_nodes);
            }
        }
        else{
            //加二保证map是偶数
            //近似二倍扩充
            size_type new_map_size=map_size+(map_size>nodes_to_add?map_size:nodes_to_add)+2;
            //配置一块空间，准备给新map使用
            map_pointer new_map=map_allocator::allocate(new_map_size);
            new_nstart=new_map+(new_map_size-new_num_nodes)/2+(add_at_front?nodes_to_add:0);
            //把原map内容拷贝过来
            std::copy(start.node,finish.node+1,new_nstart);
            //释放原map
            map_allocator::deallocate(map,map_size);
            //设定新map的起始地址与大小
            map=new_map;
            map_size=new_map_size;
        }
        //重新设定迭代器start和finish
        start.set_node(new_nstart);
        finish.set_node(new_nstart+old_num_nodes-1);
        //这里不用设定cur,因为调用该函数的函数会处理
    }


    template<class T,size_t BufSiz>
    void deque<T,BufSiz>::push_back_aux(const value_type& t){
        value_type t_copy=t;
        //若符合某种条件则重换一个map
        reserve_map_at_back();
        //配置一个新节点(缓冲区)
        *(finish.node+1)=allocate_node();
        try{
            //元素构造
            wj::construct(finish.cur,t_copy);
            //改变finish，令其指向新节点
            finish.set_node(finish.node+1);
            //设定finish的状态
            finish.cur=finish.first;
        }
        catch(...){
            finish.set_node(finish.node-1);
            finish.cur=finish.last-1;
            deallocate_node(*(finish.node+1));
            throw;
        }
    }

    //只有当start.cur==start.first时才会被调用
    template<class T,size_t BufSiz>
    void deque<T,BufSiz>::push_front_aux(const value_type& t){
        value_type t_copy=t;
        //若符合某种条件则必须重换一个map
        reserve_map_at_front();
        //配置一个新节点
        *(start.node-1)=allocate_node();
        try{
            //改变start,令其指向新节点
            start.set_node(start.node-1);
            //设定start的状态
            start.cur=start.last-1;
            //元素构造
            wj::construct(start.cur,t_copy);
        }
        catch(...){
            start.set_node(start.node+1);
            start.cur=start.first;
            deallocate_node(*(start.node-1));
            throw;
        }
    }

    template<class T,size_t BufSiz>
    void deque<T,BufSiz>::pop_back_aux(){
        //TODO 当finish.cur==finish.first时进入该函数
        //TODO 此时代表最后一个缓冲区已经没有元素了，所以要释放最后一个缓冲区，同时pop_back前一个缓冲区的最后一个元素
        deallocate_node(finish.first);      //释放最后一个缓冲区
        finish.set_node(finish.node-1);     //调整finish的状态，使其指向
        finish.cur=finish.last-1;           //上一个缓冲区的最后一个元素
        wj::destory(finish.cur);            //将该元素析构
    }

    template<class T,size_t BufSiz>
    void deque<T,BufSiz>::pop_front_aux(){
        //TODO start.cur==start.last-1会进入该函数
        //TODO 此时代表第一缓冲区只剩一个元素了，与pop_back的处理情况不同,注意
        wj::destory(start.cur);         //将第一缓冲区的第一个元素析构
        deallocate_node(start.first);   //释放第一缓冲区
        start.set_node(start.node+1);   //调整start的状态，使其指向
        start.cur=start.first;          //下一个缓冲区的第一个元素
    }

    template<class T,size_t BufSiz>
    typename deque<T, BufSiz>::iterator
    deque<T, BufSiz>::insert_aux(iterator pos,const value_type& x){
        //插入点之前的元素个数
        difference_type index=pos-start;
        value_type x_copy=x;
        if(index<size()/2){             //如果插入点之前的元素个数比较少
            push_front(front());        //在最前端加入与第一个元素同值的元素
            iterator front1=start;      //以下标示记号，然后进行元素移动
            ++front1;
            iterator front2=front1;
            ++front2;
            pos=start+index;
            iterator pos1=pos;
            ++pos1;
            std::copy(front2,pos1,front1);  //元素移动
        }
        else{                           //插入点之后的元素个数比较少
            push_back(back());          //在最尾端加入与最后元素同值的元素
            iterator back1=finish;      //以下标记记号，然后进行元素移动
            --back1;
            iterator back2=back1;
            --back2;
            pos=start+index;
            std::copy_backward(pos,back2,back1);    //元素移动
        }
        *pos=x_copy;
        return pos;
    }
    //定义deque的比较运算符
    template<class T,size_t BufSiz>
    bool operator==(const deque<T,BufSiz>& lhs,const deque<T,BufSiz>& rhs){
        return lhs.size()==rhs.size() && std::equal(lhs.begin(),lhs.end(),rhs.begin());
    }

    template<class T,size_t BufSiz>
    bool operator<(const deque<T,BufSiz>& lhs,const deque<T,BufSiz>& rhs){
        return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
    }
}


#endif /* __WJDEQUE__ */