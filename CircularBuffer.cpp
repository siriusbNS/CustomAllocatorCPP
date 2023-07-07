#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include<set>
template <class T_a>
class MyAllocator
{
private:
    T_a* Data;
    bool* arr_checker;
    int size = 1024*1024*10;

public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T_a* pointer;
    typedef const T_a* const_pointer;
    typedef T_a& reference;
    typedef const T_a& const_reference;
    typedef T_a value_type;
    MyAllocator()
    {
        Data = (T_a*) malloc(size*sizeof(T_a));
        arr_checker = new bool [size*sizeof(T_a)];

    }
    int Get_Size() const
    {
        return this->size;
    }
    T_a *Get_Data()const
    {
        return Data;
    }
    bool *Get_arr()const
    {
        return arr_checker;
    }
    template <class T>
    MyAllocator(const MyAllocator<T>&other)
    {
        size=other.Get_Size();
        Data = (T_a*) malloc(size*sizeof(T_a));
        arr_checker = new bool [size*sizeof(T_a)];
        for(size_t i = 0;i<size;i++)
        {
            arr_checker[i]=other.Get_arr()[i];

        }
    }
    ~MyAllocator() = default;
    template<class Other>
    struct rebind{
        typedef MyAllocator<Other> other;
    };
    T_a* allocate(size_t n)
    {
        size_t buff;
        bool check = false;
        for(size_t i = 0;i<size*sizeof(T_a);i++)
        {
            if(arr_checker[i]==false)
            {
                if(arr_checker[i+n]==false && (i+n)<=this->size)
                {
                    buff=i;
                    check=true;
                    for(size_t j = i;j<i+n;j++)
                    {
                        arr_checker[j]=true;
                    }
                    break;
                }

            }
        }
        if(check==false)
        {
            throw std::exception();
        }
        return &Data[buff+n];
    }

    void deallocate(T_a*p,size_t n)
    {
        T_a* beg = &Data[0];
        if(p>=beg)
        {
            size_t position = p-beg;
            size_t end_pos;
            if(n>this->size-position)
            {
                end_pos = size-position;
            }
            else{
                end_pos = n;
            }
            for(size_t i = position;i<end_pos;i++)
            {
                arr_checker[i]=false;
            }
        }
    }
    template<class ...Args>
    T_a* construct(T_a* p,Args ...args)
    {
        return new(p) T_a(std::forward<Args>(args)...);
    }
    void destroy(T_a* p)
    {
        deallocate(p,1);
        p->~T_a();
    }


};
template <class T, class U>
bool operator==(const MyAllocator <T>&, const MyAllocator <U>&) { return true; }
template <class T, class U>
bool operator!=(const MyAllocator <T>&, const MyAllocator <U>&) { return false; }
int main()
{
    std::set<int, std::less<>, MyAllocator<int>> my_set;
    my_set.insert(3);
    my_set.insert(1);
    my_set.insert(2);
    my_set.insert(5);
    my_set.insert(4);
    std::cout << "set ";
    for (int num : my_set)
        std:: cout << num << " ";
    std::cout << std::endl;
    std::vector<char,MyAllocator<char>> p;
    p.resize(100000,'t');
    std::cout<<"\nvector:";
    for(int i = 0;i<p.size()/10000;i++)
    {
        std::cout<<p[i]<<" ";
    }


}
