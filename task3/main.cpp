#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <cstddef>
#include <type_traits>
#include <memory>
#include <unordered_set>

auto block_deleter = [] (auto ptr) {
    free(static_cast<void*>(ptr));
};


template <typename T>
struct log_allocator {
    using value_type = T;
    using size_type = size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    log_allocator() = default;
    log_allocator(size_t cap);

    using block_sptr = std::shared_ptr<char>;

    pointer allocate(size_t num) {
        if(len_ != 0 && len_ < cap_) {
            ++len_;
            return cur_ptr + len_ - 1;
        }


        auto u_ptr  = block_sptr (static_cast<char*>(std::malloc(cap_ * num * sizeof(T))),block_deleter);


        if (u_ptr == nullptr) {
            throw std::bad_alloc();
        }

        auto row_ptr = u_ptr.get();

        data_.insert(u_ptr);

        len_ = 1;
        cur_ptr = reinterpret_cast<T*> (row_ptr);

        return cur_ptr;
        //        return nullptr;
    }

    template<typename T1>
    struct rebind {
        typedef log_allocator<T1> other;
    };

    log_allocator(const log_allocator&src) = default;


    template<typename T1>
    log_allocator(const log_allocator<T1>& t) {
        cap_ = t.cap_;
    }

    void deallocate(T*, size_t) {
    }

    template <typename U, typename... Args>
    void construct(U* ptr, Args&&... args) {
        ::new((void*) (ptr)) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U *ptr) {
        --len_;
        ptr->~U();
    }


    size_t cap_ = 10;
    size_t len_ = 0;
    pointer cur_ptr;
    std::unordered_set<block_sptr> data_;

};


template <typename T>
struct Node {
    Node *next = nullptr;
    T value_;
    template<typename... Args>
    Node(Args... args) {
        value_ = T(std::forward<Args>(args)...);
    }
};


template <typename T, typename Alloc = std::allocator<T>>
class List {
    using value_type = T;
    using node_type = Node<T>;
    using alloc_type = Alloc;

    template<typename _Alloc_type>
    struct List_rep {
        List_rep() = default;
        List_rep(_Alloc_type &&a) : _a(a) {}

        auto allocate(size_t l) {
            return _a.allocate(l);
        }

        template <typename U, typename... Args>
        void construct(U* ptr, Args&&... args) {
            _a.construct(ptr, std::move(args)...);
        }

        void push_back(const T& val ) {
            auto ptr = allocate(1);
            construct(ptr, val);
            if (head != nullptr) {
                ptr->next = head;
                head = ptr;
            } else {
                head = ptr;
            }
        }


        void print() {
            auto cur = head;
            while(cur != nullptr) {
                std::cout<<cur->value_<<" ";
                cur = cur->next;
            }
            std::cout<<std::endl;
        }



        ~List_rep() {
            while (head != nullptr) {
                auto ptr = head;
                head = head->next;
                _a.destroy(ptr);
                _a.deallocate(ptr, sizeof (T));
            }
        }

        Node<T>* head = nullptr;
        _Alloc_type _a;
    };


    using _Pair_alloc_type = typename __gnu_cxx::__alloc_traits<Alloc>::template
    rebind<node_type>::other;

    using List_ds = List_rep<_Pair_alloc_type>;



public:
    List () : ds_instance(_Pair_alloc_type(Alloc())) {}
    template <template <class U> class _Alloc>
    List(_Alloc<T> &a)  : ds_instance(_Pair_alloc_type(a)) {
    }

    void push_back(const T& val ) {
        ds_instance.push_back(val);
    }


    void print() {
        ds_instance.print();
    }



private:
    List_ds ds_instance;
    size_t size_;

};


template <typename Container>
void insertFactorial(Container &container, int val) {

    for(int i = 0; i < val; ++i) {
        if (i != 0)
            container[i] = container[i-1] * i;
        else
            container[i] = 1;
    }
}

template <typename Container>
void printMap(const Container& container) {
    for (auto &pair_val : container) {
        int key = 0;
        int val = 0;
        std::tie(key, val) = pair_val;
        std::cout<<key<<" "<<val<<std::endl;
    }
}

int main(int, char **)
{


    std::map<int,int> m_origin;
    insertFactorial(m_origin, 10);
    printMap(m_origin);

    log_allocator<std::pair<const int, int>> la(10);
    std::less<const int> less_object;
    std::map<int,int,std::less<const int>, log_allocator<std::pair<const int, int>>> m(less_object, la);
    insertFactorial(m, 10);
    printMap(m);

    List<int> l;
    for (int i = 0; i < 10; ++i) {
        l.push_back(i);
    }

    l.print();


    log_allocator<int> ll(10);
    List<int,log_allocator<int>> l_custom_alloc(ll);
    for (int i = 0; i < 10; ++i) {
        l_custom_alloc.push_back(i);
    }

    l_custom_alloc.print();


    return 0;
}

template<typename T>
log_allocator<T>::log_allocator(size_t cap) : cap_(cap) {}
