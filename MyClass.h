#pragma once
#include <string>

namespace MyClassNamespace
{

    class MyClass
    {
    public:

        // Constructor
        MyClass(int num = 5, std::string str = "default") : _num(num), _str(str)
        {
            printf("MyClass: Constructor\n");
        }

        // Destructor
        ~MyClass()
        {
            printf("MyClass: Destructor\n");
        }

        // Copy constructor
        MyClass(const MyClass& other) : _num(other._num), _str(other._str)
        {
            printf("MyClass: copy constructor\n");
        }

        // Move constructor
        MyClass(MyClass&& other) : _num(other._num), _str(other._str)
        {
            printf("MyClass: Move constructor\n");
        }

        // Copy assignment
        MyClass& operator=(const MyClass& other)
        {
            printf("MyClass: Copy assignment\n");
            if (&other != this) {
                _num = other._num;
                _str = other._str;
            }
            return *this;
        }

        // Move assignment 
        MyClass& operator=(const MyClass&& other)
        {
            printf("MyClass: Move assignment \n");
            if (&other != this) {
                _num = other._num;
                _str = other._str;
            }
            return *this;
        }

        void setNum(int num)
        {
            _num = num;
        }

        void print()
        {
            printf("MyClass content: num=%d, str=%s\n", _num, _str.c_str());
        }

        std::string toJson() const
        {
            return "MyClass JSON string";
        }

    private:
        int _num = -2;
        std::string _str = "test";
    };

}
