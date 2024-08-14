#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <cstdint>

template<typename ... Lambdas>
class Visitor : public Lambdas...
{
public:
    Visitor(Lambdas&& ... lambdas)
        : Lambdas{lambdas}...
    {}

    using Lambdas::operator()...;
};

int main()
{
    Visitor v{
                []
                {
                    std::cout << "I am A\n";
                },
                [] (int) 
                {
                    std::cout << "I am B\n";
                },
                [] (std::string) 
                {
                    std::cout << "I am C\n";
                }
            };
    v();
    v(10);
    v("abacaba");

    return 0;
}
