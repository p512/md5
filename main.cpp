#include <iostream>
#include <cstring>

#include "md5.hpp"

using namespace std;

int main() {
	char const *str = "hello\n";
	cout << hashmd5(str, strlen(str)) << endl;
}
