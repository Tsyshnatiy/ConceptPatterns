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

template<typename ... CBS>
class Visitor : public CBS...
{
public:
    Visitor(CBS ... as)
        : args{as...}
    {}

    using CBS::operator()...;

private:
    std::tuple<CBS...> args;
};

template<typename TV, typename TN>
struct List
{
    TV value;
    TN* next = nullptr;
};

template<typename TV>
struct List<TV, void>
{
    TV value;
    List<int, void>* next = nullptr;
};

struct A
{

};

template<typename TList, typename Visitor>
void apply(TList* l, Visitor& v)
{
    v(l->value);

    if (l->next)
    {
        apply(l->next, v);
    }
}

int main()
{
    Visitor a
    {
        [](int a) { std::cout << a << std::endl; },
        [](auto a) { std::cout << "Generic " << a << std::endl; },
        [](const char* a) { std::cout << a << std::endl; },
        [](const A& a) { std::cout << "A\n"; },
    };

    List<int, void> tail;
    tail.value = 10;

    List<const char*, decltype(tail)> t1;
    t1.value = "abacaba";
    t1.next = &tail;

    List<int, decltype(t1)> t2;
    t2.value = 15;
    t2.next = &t1;

    List<A, decltype(t2)> t3;
    t3.value = A{};
    t3.next = &t2;

    apply(&t3, a);

    return 0;
}
