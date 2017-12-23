#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <cstddef>
#include <type_traits>
template <typename T>
struct log_allocator {
    using value_type = T;
    using size_type = size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    log_allocator() = default;
    log_allocator(size_t cap) : cap_(cap) {}

    pointer allocate(size_t num) {
        if(len_ != 0 && len_ < cap_) {
            ++len_;
            return cur_ptr + len_ - 1;
        }

        std::cout<<"calling allocate "<<num<<std::endl;
        auto ptr = std::malloc(cap_ * num * sizeof(T));
        if (ptr == nullptr) {
            throw std::bad_alloc();
        }

        len_ = 1;
        cur_ptr = static_cast<T*> (ptr);

        return cur_ptr;
    }

    template<typename T1>
    struct rebind {
        typedef log_allocator<T1> other;
    };

    log_allocator(const log_allocator&) = default;

    template<typename T1>
    log_allocator(const log_allocator<T1>& t) {
        cap_ = t.cap_;
    }

    void deallocate(T* ptr, size_t) {
        std::cout<<"calling deallocate"<<std::endl;
        if(len_ != 0)
            return;

        std::cout<<"clean allocation"<<std::endl;
        delete ptr;
    }

    template <typename U, typename... Args>
    void construct(U* ptr, Args&&... args) {
        std::cout<<"calling constructor"<<std::endl;
        ::new((void*) (ptr)) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U *ptr) {
        std::cout<<"calling destructor"<<std::endl;
        --len_;
        ptr->~U();
    }


    size_t cap_ = 10;
    size_t len_ = 0;
    pointer cur_ptr;

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


template <typename T, typename Alloc = std::allocator<Node<T>>>
class List {
    using value_type = T;
    using alloc_type = Alloc;
    Node<T>* head = nullptr;


public:
    List () = default;
    void push_back(const T& val ) {

        auto ptr = alloc.allocate(1);
        alloc.construct(ptr, val);
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

private:
    alloc_type alloc;
    size_t size_;

};


int main(int, char **)
{


    std::map<int,int> m_origin;
    for(int i = 1; i <= 10; ++i) {
        if (!m_origin.empty())
            m_origin[i] = m_origin[i-1] * i;
        else
            m_origin[i] = 1;
    }

    for (auto &m_val : m_origin) {
        int key = 0;
        int val = 0;
        std::tie(key, val) = m_val;
        std::cout<<key<<" "<<val<<std::endl;

    }

    log_allocator<std::pair<const int, int>> la(10);
    std::less<const int> less_object;
    std::map<int,int,std::less<const int>, log_allocator<std::pair<const int, int>>> m(less_object, la);
    for(int i = 1; i <= 10; ++i) {
        if (!m.empty())
            m[i] = m[i-1] * i;
        else
            m[i] = 1;
    }


    for (auto &m_val : m) {
        int key = 0;
        int val = 0;
        std::tie(key, val) = m_val;
        std::cout<<key<<" "<<val<<std::endl;

    }

    List<int> l;
    for (int i = 0; i < 10; ++i) {
        l.push_back(i);
    }

    l.print();


    return 0;
}
