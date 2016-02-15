#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>

#include "md5.hpp"

using namespace std;

int main() {
	char buf[1<<16];
	md5ctx m;
	while(cin) {
		cin.read(buf, sizeof buf);
		size_t n = cin.gcount();
		if(n) {
			m.update(buf, n);
		}
	}
	cout << m.finalize() << endl;
}
