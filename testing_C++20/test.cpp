#include <iostream>

typedef struct daddy
{
        int daddy_age;

        void daddy_say_hi()
        {
                std::cout << "Daddy, Hi" << std::endl;
        }
} _daddy;

class dad
{
private:
        int a;

public:
        int b;

        dad()
        {}



        ~dad()
        {}
};

template<typename T>
concept HasDaddyAge = requires (T t)
{
        t.dad_hi();
};

template<typename T>
struct has_daddy
{
        static constexpr bool has_mem = requires(T a) { a.dad_hi(); };
        static constexpr bool is_same_type = requires { std::is_same<T, dad>::value; };
};

template<typename T>
concept d = requires(T t) {
        std::is_same<T, dad>::value || std::is_same<T, dad>::value;
        has_daddy<T>::has_mem;
};

template<typename T>
        requires std::integral<T> //std::floating_point<T>
void a(T b)
{
        std::cout << b << std::endl;
}

template<typename T>//requires(requires(T t) {{t} -> std::same_as<dad>;})
        requires d<T>//has_daddy<T>::is_same_type
void abc(T a)
{
        std::cout << "hey22!" << std::endl;
}

template<typename T>
        requires has_daddy<T>::has_mem
void bb(T a)
{
        std::cout << "Hey" << std::endl;
}

void y(HasDaddyAge auto mem)
{
        std::cout << "Sure" << std::endl;
}

int main(int args, char *argv[])
{
        dad d;
        abc<dad> (d);//bb<dad> (d);
        //d.b = 10;
        //a(19);
        //y(d);
        //a<int> (d.b);
        return 0;
}
