#include <iostream>

#define SOME_RANDOM_SHIT

template<typename A, typename B>
struct SOME_RANDOM_SHIT Bro {
    A bro1;
    B bro2;
};

int main()
{
    struct Bro<char, double> b;

    b.bro1 = 'S';
    b.bro2 = 10.24;
    std::cout << "Hello, World" << std::endl;
}