#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <cstdint>

template<typename T>
concept Interface3rdParty = requires(T t)
{
    { t.work() } -> std::integral;
};

class Adapter3rdParty_A
{
public:
    int work() 
    {
        std::cout << "Adapter A\n";
        return 1;
    }
};

class Adapter3rdParty_B
{
public:
    std::uint8_t work()
    {
        std::cout << "Adapter B\n";
        return 2;
    }
};

template<Interface3rdParty T>
class Client
{
public:
    Client(T&& thirdParty)
        : _thirdParty{std::move(thirdParty)}
    {}

    void makeThisHappen()
    {
        std::cout << "Client uses 3rd party\n";
        std::cout << "3rd party returned " << (int)_thirdParty.work() << "\n";
    }
private:
    T _thirdParty;
};

int main()
{
    Client<Adapter3rdParty_A> clientA{Adapter3rdParty_A{}};
    clientA.makeThisHappen();

    Client<Adapter3rdParty_B> clientB{Adapter3rdParty_B{}};
    clientB.makeThisHappen();

    return 0;
}
