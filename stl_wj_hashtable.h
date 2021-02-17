#ifndef __WJHASHTABLE__
#define __WJHASHTABLE__

/*
    本文件实现hash table
*/
#include"stl_wj_iterator.h"
#include<algorithm>     //for lower_bound
#include"stl_memory.h"
#include<vector>
#include"stl_wj_pair.h"

namespace wj{

    //一些声明
    template<class,class,class,class,class> class hashtable;
    template<class,class,class,class,class> struct __hashtable_iterator;
    template<class,class,class,class,class> struct __hashtable_const_iterator;

    //hash table的节点定义
    template<class Value>
    struct __hashtable_node
    {
        __hashtable_node* next;
        Value val;
    };

    //hash table的迭代器
    template<class Value,class Key,class HashFcn,
            class ExtractKey,class EqualKey>
    struct __hashtable_iterator{
        typedef hashtable<Value,Key,HashFcn,ExtractKey,EqualKey> hashtable;
        typedef __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> iterator;
        typedef __hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> const_iterator;
        typedef __hashtable_node<Value> node;

        //迭代器的型别
        typedef wj::forward_iterator_tag iterator_category;
        typedef Value value_type;
        typedef ptrdiff_t difference_type;
        typedef Value&  reference;
        typedef Value* pointer;

        typedef size_t size_type;

        //data member
        node* cur;      //迭代器目前所指之节点
        hashtable* ht;  //保持对容器的连结关系

        //ctor
        __hashtable_iterator(node* n,hashtable* tab):cur(n),ht(tab){}
        __hashtable_iterator(){}

        //some functions
        reference operator*() const { return cur->val; }
        pointer operator->() const { return &(operator*()); }
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& it) const { return cur==it.cur; }
        bool operator!=(const iterator& it) const { return cur!=it.cur; }
    };

    template<class Value,class Key,class HashFcn,
            class ExtractKey,class EqualKey>
    struct __hashtable_const_iterator{
        typedef hashtable<Value,Key,HashFcn,ExtractKey,EqualKey> hashtable;
        typedef __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> iterator;
        typedef __hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> const_iterator;
        typedef __hashtable_node<Value> node;

        //迭代器的型别
        typedef wj::forward_iterator_tag iterator_category;
        typedef Value value_type;
        typedef ptrdiff_t difference_type;
        typedef const Value&  reference;
        typedef const Value* pointer;

        typedef size_t size_type;

        //data member
        const node* cur;      //迭代器目前所指之节点
        const hashtable* ht;  //保持对容器的连结关系

        //ctor
        __hashtable_const_iterator(const node* n,const hashtable* tab):cur(n),ht(tab){}
        __hashtable_const_iterator(){}

        //some functions
        reference operator*() const { return cur->val; }
        pointer operator->() const { return &(operator*()); }
        const_iterator& operator++();
        const_iterator operator++(int);
        bool operator==(const iterator& it) const { return cur==it.cur; }
        bool operator!=(const iterator& it) const { return cur!=it.cur; }
    };

    //一些辅助函数
    static const int __stl_num_primes=28;
    static const unsigned long __stl_prime_list[__stl_num_primes]=
    {
    53,         97,           193,         389,       769,
    1543,       3079,         6151,        12289,     24593,
    49157,      98317,        196613,      393241,    786433,
    1572869,    3145739,      6291469,     12582917,  25165843,
    50331653,   100663319,    201326611,   402653189, 805306457,
    1610612741, 3221225473ul, 4294967291ul
    };

    //找出上述28个质数之中，最接近并大于等于n的那个质数
    inline unsigned long __stl_next_prime(unsigned long n){
        const unsigned long* first=__stl_prime_list;
        const unsigned long* last=__stl_prime_list+__stl_num_primes;
        //查找第一个大于等于n的数
        const unsigned long* pos=std::lower_bound(first, last,n);
        return pos==last?*(last-1):*pos;
    }

    //hash table
    template<class Value,class Key,class HashFcn,
            class ExtractKey,class EqualKey>
    class hashtable{
        friend struct __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>;
        friend struct __hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>;
        public:
            typedef Value value_type;
            typedef Key key_type;
            typedef Value& reference;
            typedef HashFcn hasher;
            typedef EqualKey key_equal;
            typedef size_t size_type;
        private:
            typedef __hashtable_node<Value> node;
            typedef wj::simple_alloc<node> node_allocator;
        public:
            typedef __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> iterator;
            typedef __hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> const_iterator;
        private:
            //function member
            hasher hash;
            key_equal equals;
            ExtractKey get_key;

            //data member
            std::vector<node*> buckets;
            size_type num_elements;
        public:
            //bucket个数即buckets vector的大小
            size_type bucket_count() const { return buckets.size(); }
            size_type max_bucket_count() const{
                return __stl_prime_list[__stl_num_primes-1];
                //返回4294967291
            }
            size_type next_size(size_type n) const { return __stl_next_prime(n); }
            size_type size() const { return num_elements; }
            size_type max_size() const { return size_type(-1); }
            bool empty() const { return size()==0; }
        private:
            //内存管理
            node* new_node(const value_type& obj){
                node* n=node_allocator::allocate();
                n->next=nullptr;
                try{
                    wj::construct(&n->val,obj);
                    return n;
                }
                catch(...){
                    node_allocator::deallocate(n);
                }
            }

            void delete_node(node* n)
            {
                wj::destory(&n->val);
                node_allocator::deallocate(n);
            }
        private:
            //初始化
            void initialize_buckets(size_type n)
            {
                const size_type n_buckets=next_size(n);
                buckets.reserve(n_buckets);
                buckets.insert(buckets.end(),n_buckets,(node*)nullptr);
                num_elements=0;
            }
        public:
            //ctor
            hashtable(size_type n,const HashFcn& hf,const EqualKey& eql)
                        :hash(hf),equals(eql),get_key(ExtractKey()),num_elements(0)
            {
                initialize_buckets(n);
            }
            //dtor
            ~hashtable(){ clear(); }
        public:
            void resize(size_type);
            //插入元素，不允许重复
            wj::pair<iterator,bool> insert_unique(const value_type& obj)
            {
                resize(num_elements+1);      //判断是否需要重建表格，如需要就扩充
                return insert_unique_noresize(obj);
            }
            //插入元素，允许重复
            iterator insert_equal(const value_type& obj)
            {
                resize(num_elements+1);
                return insert_equal_noresize(obj);
            }
        private:
            wj::pair<iterator,bool> insert_unique_noresize(const value_type&);
            iterator insert_equal_noresize(const value_type&);
        public:
            //计算元素的hash值
            size_type bkt_num_key(const key_type& key,size_t n) const
            {
                return hash(key)%n;
            }
            size_type bkt_num_key(const key_type& key) const
            {
                return bkt_num_key(key,buckets.size());
            }
            size_type bkt_num(const value_type& obj,size_t n) const
            {
                return bkt_num_key(get_key(obj),n);
            }
            size_type bkt_num(const value_type& obj) const
            {
                return bkt_num_key(get_key(obj));
            }
        public:
            iterator end() {
                return iterator((node*)nullptr,this);
            }
            iterator begin() {
                for(size_t n=0;n<buckets.size();++n){
                    if(buckets[n]!=nullptr)
                        return iterator(buckets[n],this);
                }
                return end();
            }
        public:
            void clear();
            void copy_from(const hashtable& ht);
        public:
            iterator find(const key_type& key){
                const size_type n=bkt_num_key(key);
                node* first;
                for(first=buckets[n];first&&!equals(get_key(first->val),key);first=first->next) {}
                return iterator(first,this);
            }
            size_type count(const key_type& key){
                const size_type n=bkt_num_key(key);
                node* first;
                size_type result=0;
                for(first=buckets[n];first;first=first->next){
                    if(equals(get_key(first->val),key)) ++result;
                }
                return result;
            }
            reference find_or_insert(const value_type& obj);
    };

    template<class V, class K, class HF,class ExK,class EqK>
    void hashtable<V,K,HF,ExK,EqK>::resize(size_type num_elements_hint)
    {
        const size_type old_n=buckets.size();
        if(num_elements_hint>old_n){    //元素个数大于bucket个数就需要考虑扩充
            const size_type n=next_size(num_elements_hint);     //找出下一个质数
            if(n>old_n){
                std::vector<node*> tmp(n,(node*)nullptr);   //设立新的buckets;
                try{
                    //处理每一个旧的bucket
                    for(size_type bucket=0;bucket<old_n;++bucket){
                        node* first=buckets[bucket];
                        while(first){
                            size_type new_bucket=bkt_num(first->val,n);
                            buckets[bucket]=first->next;
                            first->next=tmp[new_bucket];
                            tmp[new_bucket]=first;
                            first=buckets[bucket];
                        }
                    }
                    //将buckets替换为新的
                    buckets.swap(tmp);
                }
                catch(...){
                }
            }
        }
    }

    //插入元素，不允许重复
    template<class V,class K,class HF,class ExK,class EqK>
    wj::pair<typename hashtable<V,K,HF,ExK,EqK>::iterator,bool>
    hashtable<V,K,HF,ExK,EqK>::insert_unique_noresize(const value_type& obj)
    {
        const size_type n=bkt_num(obj);
        node* first=buckets[n]; //找出元素所在的slot的首指针

        for(node* cur=first;cur;cur=cur->next)
            if(equals(get_key(cur->val),get_key(obj)))
                //发现存在键值相同的
                return wj::pair<iterator,bool>(iterator(cur,this),false);

        //离开以上循环时，first指向bucket所指链表的头部节点
        node* tmp=new_node(obj);
        tmp->next=first;
        buckets[n]=tmp;
        ++num_elements;
        return wj::pair<iterator,bool>(iterator(tmp,this),true);
    }

    //插入元素，运行重复
    template<class V, class K, class HF,class ExK,class EqK>
    typename hashtable<V,K,HF,ExK,EqK>::iterator
    hashtable<V,K,HF,ExK,EqK>::insert_equal_noresize(const value_type& obj)
    {
        const size_type n=bkt_num(obj);
        node* first=buckets[n];

        for(node* cur=first;cur;cur=cur->next)
            if(equals(get_key(cur->val),get_key(obj))){
                //如果发现与链表中的某键值相同，就马上插入，然后返回
                node* tmp=new_node(obj);
                tmp->next=cur->next;
                cur->next=tmp;
                ++num_elements;
                return iterator(tmp,this);
            }

        node* tmp=new_node(obj);
        tmp->next=first;
        buckets[n]=tmp;
        ++num_elements;
        return iterator(tmp,this);
    }

    template<class V, class K, class HF,class ExK,class EqK>
    void hashtable<V,K,HF,ExK,EqK>::clear()
    {
        //针对每一个bucket
        for(size_type i=0;i<buckets.size();++i){
            node* cur=buckets[i];
            //将bucket list中的每一个节点删除掉
            while(cur!=0){
                node* next=cur->next;
                delete_node(cur);
                cur=next;
            }
            buckets[i]=0;   //令bucket内容为null指针
        }
        num_elements=0;
    }

    template<class V, class K, class HF,class ExK,class EqK>
    void hashtable<V,K,HF,ExK,EqK>::copy_from(const hashtable& ht)
    {
        //先清除己方的bucket vector
        buckets.clear();
        buckets.reserve(ht.buckets.size());
        buckets.insert(buckets.end(),ht.buckets.size(),(node*)0);
        try{
            //针对bucket vector
            for(size_type i=0;i<ht.buckets.size();++i){
                //复制vector的每一个元素
                if(const node* cur=ht.buckets[i]){
                    node* copy=new_node(cur->val);
                    buckets[i]=copy;
                    //针对同一个bucket list，复制每一个节点
                    for(node* next=cur->next;next;cur=next,next=cur->next){
                        copy->next=new_node(next->val);
                        copy=copy->next;
                    }
                }
            }
            num_elements=ht.num_elements;
        }
        catch(...){
            clear();
        }
    }

    template<class V, class K, class HF,class ExK,class EqK>
    typename hashtable<V,K,HF,ExK,EqK>::reference
    hashtable<V,K,HF,ExK,EqK>::find_or_insert(const value_type& obj)
    {
        resize(num_elements+1);

        const size_type n=bkt_num(obj);
        node* first=buckets[n];
        for(node* cur=first;cur;cur=cur->next)
            if(equals(get_key(cur->val),get_key(obj)))
                return cur->val;

        node* tmp=new_node(obj);
        tmp->next=first;
        buckets[n]=tmp;
        ++num_elements;
        return tmp->val;
    }

    template<class V,class K,class HF,class ExK,class EqK>
    __hashtable_iterator<V,K,HF,ExK,EqK>&
    __hashtable_iterator<V,K,HF,ExK,EqK>::operator++()
    {
        const node* old=cur;
        cur=cur->next;
        if(!cur){
            //根据元素值，定位出下一个bucket,其起头处就是我们目的地
            //计算出下一个元素的哈希值
            size_type bucket=ht->bkt_num(old->val);
            while(!cur&&++bucket<ht->buckets.size())
                cur=ht->buckets[bucket];
        }
        return *this;
    }

    template<class V, class K, class HF,class ExK,class EqK>
    __hashtable_iterator<V,K,HF,ExK,EqK>
    __hashtable_iterator<V,K,HF,ExK,EqK>::operator++(int)
    {
        iterator tmp=*this;
        ++*this;
        return tmp;
    }

    template<class V,class K,class HF,class ExK,class EqK>
    __hashtable_const_iterator<V,K,HF,ExK,EqK>&
    __hashtable_const_iterator<V,K,HF,ExK,EqK>::operator++()
    {
        const node* old=cur;
        cur=cur->next;
        if(!cur){
            //根据元素值，定位出下一个bucket,其起头处就是我们目的地
            //计算出下一个元素的哈希值
            size_type bucket=ht->bkt_num(old->val);
            while(!cur&&++bucket<ht->buckets.size())
                cur=ht->buckets[bucket];
        }
        return *this;
    }

    template<class V, class K, class HF,class ExK,class EqK>
    __hashtable_const_iterator<V,K,HF,ExK,EqK>
    __hashtable_const_iterator<V,K,HF,ExK,EqK>::operator++(int)
    {
        const_iterator tmp=*this;
        ++*this;
        return tmp;
    }
}


#endif /* __WJHASHTABLE__ */