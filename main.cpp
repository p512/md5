#include <iostream>
#include <fstream>

#include "md5.hpp"

using namespace std;

int main(int argc, char **argv) {
	ifstream file;
	istream *is = &cin;
	if(argc > 1) {
		file.open(argv[1]);
		is = &file;
	}
	if(!is->good()) {
		cerr << "Failed to open file." << endl;
		return 1;
	}
	char buf[1<<16];
	md5ctx m;
	while(*is) {
		is->read(buf, sizeof buf);
		size_t n = is->gcount();
		if(n) {
			m.update(buf, n);
		}
	}
	cout << m.finalize() << endl;
}
