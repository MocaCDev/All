#include <iostream>
#include <string.h>

typedef struct Hey1
{
	int a;
	int b;
} _Hey1;

typedef struct Hey2
{
	int a;
	int b;
} _Hey2;

typedef struct Hey3
{
	int a;
	int b;
} _Hey3;

static const char * const all[] = {
	"Hey1","Hey2"
};

template<typename T>
unsigned char check_typename_name_is_valid()
{
	std::string name1 = typeid(T).name();
	std::string name2 = name1.substr(1, name1.length());
	const char *name = name2.c_str();

	for(int i = 0; i < sizeof(all)/sizeof(all[0]); i++)
		if(strcmp(name, all[i]) == 0) return 1;

	return 0;
}

template<typename T>
T init()
{
	if(check_typename_name_is_valid<T>())
		std::cout << "Yes" << std::endl;

	return (T) {
		.a = 0x10,
		.b = 0x20
	};
}

int main()
{
	_Hey3 h1 = init<_Hey3>();
	return 0;
}
