#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <cstdint>
#include <memory>

// https://www.modernescpp.com/index.php/type-erasure/

template<typename T>
concept Drawable = requires(T t)
{
    { t.draw() };
};

class TypeErasure
{
public:
    template<Drawable Type>
    TypeErasure(Type&& arg)
        : _concept{std::make_unique<Model<Type>>(std::move(arg))}
    {

    }
    void draw()
    {
        _concept->draw();
    }

    struct Concept
    {
        virtual ~Concept() = default;
        virtual void draw() = 0;
    };

    template<Drawable Type>
    struct Model : public Concept
    {
    public:
        Model(Type&& arg)
            : _arg{std::move(arg)}
        {}

        void draw() override
        {
            _arg.draw();
        }

    private:
        Type _arg;
    };

private:
    // To enable copy and move construction
    std::shared_ptr<Concept> _concept;
};

struct DrawA
{
    void draw()
    {
        std::cout << "Drawing A\n";
    }
};

struct DrawB
{
    void draw()
    {
        std::cout << "Drawing B\n";
    }
};

int main()
{
    std::vector<TypeErasure> erasures{DrawA{}, DrawB{}};

    for (auto& er : erasures)
    {
        er.draw();
    }
    return 0;
}
