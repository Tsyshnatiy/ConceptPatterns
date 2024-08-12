#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <cstdint>

// https://refactoring.guru/design-patterns/factory-method#:~:text=Factory%20Method%20is%20a%20creational,objects%20that%20will%20be%20created.

template<typename T>
concept Button = requires(const T t)
{
    { t.press() };
};

class MyButton_A
{
public:
    MyButton_A(int)
    {

    }
    
    void press() const
    {
        std::cout << "MyButton_A is pressed\n";
    }
};

class MyButton_B
{
public:
    MyButton_B(std::string)
    {

    }

    void press() const
    {
        std::cout << "MyButton_B is pressed\n";
    }
};

template<typename T>
concept Dialog = requires(const T t)
{
    { t.pressButton() };
};

template<typename Derived, Button B>
class DialogBase
{
public:
    template<typename ... Args>
    B create(Args ... args)
    {
        return B(std::forward<Args>(args)...);
    }

    void pressButton() const
    {
        auto* d = static_cast<const Derived*>(this);
        d->getButton().press();
    }
};

class WindowDialog : public DialogBase<WindowDialog, MyButton_A>
{
public:
    WindowDialog()
        : _btn{create(1)}
    {
    }

    const MyButton_A& getButton() const
    {
        return _btn;
    }

private:
    MyButton_A _btn;
};

class WebDialog : public DialogBase<WebDialog, MyButton_B>
{
public:
    WebDialog()
        : _btn{create("abacaba")}
    {
    }

    const MyButton_B& getButton() const
    {
        return _btn;
    }

private:
    MyButton_B _btn;
};

template<Dialog D>
class Client
{
public:
    Client()
    {
        D{}.pressButton();
    }
};

int main()
{
    Client<WindowDialog> c1;
    Client<WebDialog> c2;

    return 0;
}
