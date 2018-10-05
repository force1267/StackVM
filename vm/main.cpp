#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
typedef uint32_t i32;

#include "storage.cpp"
#include "vm.cpp"


int main(int argc, char** argv) {
	
	if (argc < 2) {
		cout << "Usage: " << argv[0] << " <program-binary> <optional-memory-size>" << endl;
		return 0;
	}

	ifstream r(argv[1], ios::binary);
	i32 i;
	vector<i32> program;

	while (r.read((char*)&i, sizeof(i))) {
		program.push_back(i);
	}
    InMemoryStorage storage(program);
	int memsize = 1e6;
	if(argc > 2)
		istringstream(argv[2]) >> memsize;
    StackVM vm(memsize);
    vm.boot(storage);
    return 0;
}