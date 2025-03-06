#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <cstdint>
#include <ranges>
#include <thread>
#include <future>
#include <queue>
#include <functional>
#include <unordered_map>
#include <typeindex>

struct A
{
};

void print_data(int a)
{
    std::cout << a << std::endl;
}

void print_data(const std::string& a)
{
    std::cout << a << std::endl;
}

void print_data(A& a)
{
    std::cout << "Struct A\n";
}

class Printer
{
public:
    template<typename T>
    Printer(T* t)
        : _print{ nullptr }
        , _t{ t }
    {
        _print = [](void* t)
        {
            auto* ref = static_cast<std::decay_t<T>*>(t);
            print_data(*ref);
        };
    }

    static void print_data(auto& a)
    {
        std::cout << a << std::endl;
    }

    static void print_data(A&)
    {
        std::cout << "A" << std::endl;
    }

    void print()
    {
        _print(_t);
    }

private:
    void (*_print)(void*);
    void* _t;
};

template<typename Operations>
class HVector
{
public:
    HVector()
        : _storage{}
    {}

    template<typename T>
    void push_back(T&& t)
    {
        using DecayT = std::decay_t<T>;
        auto& container = _storage[std::type_index(typeid(DecayT))];
        if (!container)
        {
            container = std::make_unique<ElementContainer<DecayT>>();
        }
        auto specialized = static_cast<ElementContainer<DecayT>*>(container.get());

        specialized->push_back(std::forward<T>(t));
        _orderedObjects.emplace_back(specialized->back());
    }

    template<typename Callback>
    void for_each(Callback cb)
    {
        for (auto& o : _orderedObjects)
        {
            cb(o);
        }
    }

private:
    struct IElementContainer
    {
        virtual ~IElementContainer() = default;
    };

    template<typename T>
    struct ElementContainer : IElementContainer
    {
        void push_back(const T& t)
        {
            _elements.push_back(t);
        }

        void push_back(T&& t)
        {
            _elements.push_back(std::move(t));
        }

        T* back()
        {
            return &_elements.back();
        }

        // deque to avoid vector reallocations and dangling pointers
        // in _orderedObjects
        std::deque<std::decay_t<T>> _elements;
    };

    std::unordered_map<std::type_index, std::unique_ptr<IElementContainer>> _storage;

    std::vector<Operations> _orderedObjects;
};

int main()
{
    HVector<Printer> v;

    std::string str("aaaaaaaaaaaaaaaa bbbbbbbbbbbbb cccccc");

    v.push_back(str);
    v.push_back(10);
    v.push_back(15);
    v.push_back("abaca");
    v.push_back(A{});
    v.push_back(std::string("asdasdasdas"));

    v.for_each([](Printer& p) { p.print(); });

    return 0;
}
