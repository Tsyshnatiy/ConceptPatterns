#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <cstdint>
#include <memory>

// https://refactoring.guru/ru/design-patterns/decorator

template<typename T>
concept Highlightable = requires(T t)
{
    { t.highlight() };
};

class Button
{
public:
    void highlight()
    {
        std::cout << "I am button\n";
    }
};

template<Highlightable H>
class Decoration
{
public:
    Decoration(H&& h)
        : _next{std::move(h)}
    {}

    void highlight()
    {
        std::cout << "I am decoration\n";

        _next.highlight();
    }

private:
    H _next;
};

template<Highlightable H>
class Client
{
public:
    Client(H& h)
        : _highlightable{h}
    {}

    void hover()
    {
        _highlightable.highlight();
    }

private:
    H& _highlightable;
};

int main()
{
    Decoration decoration{Button{}};
    Client client{decoration};

    client.hover();

    return 0;
}
