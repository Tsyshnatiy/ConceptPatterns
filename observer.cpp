#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <cstdint>

// https://refactoring.guru/design-patterns/observer


template<typename TS>
concept Subscriber = requires(TS t)
{
    { t.update() };
};

template<Subscriber ... Args>
class PublisherAllOnStart
{
public:
    PublisherAllOnStart(Args& ... args)
        : _args(std::forward<Args&>(args)...)
    {

    }

    void notify()
    {
        std::cout << "I am PublisherAllOnStart\n";
        std::apply
        (
            [](Args&... tupleArgs)
            {
                ((tupleArgs.update()), ...);
            }, 
            _args
        );
    }

private:
    std::tuple<Args&...> _args;
};

struct EmptyPublisher
{
    void notify()
    {

    }
};

template<Subscriber S, typename Next>
class PublisherOneByOne
{
public:
    PublisherOneByOne(S* s, Next* n)
        : _subscriber{s}
        , _next(n)
    {
    }

    template<Subscriber SS>
    auto addSubscriber(SS* s)
    {
        return PublisherOneByOne<SS, PublisherOneByOne<S, Next>>{s, this};
    }

    void notify()
    {
        if (_subscriber)
        {
            _subscriber->update();
        }
        if (_next)
        {
            _next->notify();
        }
    }

private:
    S* _subscriber;
    Next* _next;
};

struct SubscriberA
{
public:
    SubscriberA(int aa)
        : _a{aa}
    {
    } 

    void update()
    {
        std::cout << "I am subscriber A\n";
    }

private:
    int _a;
};

struct SubscriberB
{
public:
    SubscriberB(std::string bb)
        : _b{bb}
    {

    } 

    void update()
    {
        std::cout << "I am subscriber B\n";
    }

private:
    std::string _b;
};

int main()
{
    SubscriberA sA{10};
    SubscriberB sB{"abacaba"};

    PublisherAllOnStart pubOnStart{sA, sB};
    pubOnStart.notify();

    EmptyPublisher empub;
    PublisherOneByOne<SubscriberB, EmptyPublisher> pub1(&sB, &empub);
    auto pub2 = pub1.addSubscriber(&sA);
    pub2.notify();

    return 0;
}
