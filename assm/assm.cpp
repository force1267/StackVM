/*
(128) .main .maint .mainc {
    .dbz push 1 handler
    2 push
    2 sub push
    12 div push
    6 sub push
    skip
    halt
}
.dbz (32) {
    754 push
    skip
    halt
}
*/


#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include"LL.cpp"
using namespace std;
typedef uint32_t i32;

// dotnames' each name-address
class note {
    public:
    string name;
    i32 address;
    note(string name, i32 address):name(name), address(address) {}
};
LinkedList<note*> dotnames;

bool isNumber(string& line) {
    char* p;
    strtol(line.c_str(), &p, 0); // 10 to disallow hex, 0 to allow
    return *p == 0;
}

bool isSkip(string& line) {
    return line[0] == '(' && line[line.length() - 1] == ')';
}

bool isDotName(string& line) {
    return line[0] == '.';
}

bool isRefName(string& line) {
    return line[0] == ':';
}

bool get_bin(string subs, i32& bin) {
    bool ok = false;
    if(isDotName(subs)) { // if .name
        note* n = 0;
        class notefinder:public LinkedList<note*>::callback {
            string name;
            note*& addr;
            public: void fn(note* nt, LinkedList<note*>::Node*) {
                if(nt->name == name) {
                    addr = nt;
                }
            }
            notefinder(string name, note*& addr):name(name), addr(addr) {}
        } findnote(subs, n);
        dotnames.forEach(&findnote);
        if(n) {
            bin = n->address;
            ok = true;
        } else {
            subs[0] = ':';
            cout << "reference " << subs << " is not decleared." << endl;
            ok = false;
        }
    } else if(isNumber(subs)) { // if i32
        istringstream(subs) >> bin;
        
        // negative numbers start with 0b10..
        // positive numbers start with 0b00..
        
        ok = true;
    } else {
        // instructions start with 0b01..
        // TODO : universal iset.h file shared between vm and assm
        // machine control
        if(subs == "halt") { // halt
            bin = 0x40000000 + 0;
            ok = true;
        }
        if(subs == "dupl") { // duplicate (tos) -> tos
            bin = 0x40000000 + 27;
            ok = true;
        }
        if(subs == "detemp") { // detemp (tmp) -> acc
            bin = 0x40000000 + 28;
            ok = true;
        }
        if(subs == "temp") { // temp (acc) -> tmp
            bin = 0x40000000 + 29;
            ok = true;
        }
        if(subs == "push") { // push (acc) -> tos
            bin = 0x40000000 + 30;
            ok = true;
        }
        if(subs == "pop") { // pop (tos) -> acc
            bin = 0x40000000 + 31;
            ok = true;
        }
        
        // arithmetics and calculations
        if(subs == "add") { // add (acc, tos) -> acc
            bin = 0x40000000 + 32;
            ok = true;
        }
        if(subs == "sub") { // sub (acc, tos) -> acc
            bin = 0x40000000 + 33;
            ok = true;
        }
        if(subs == "mul") { // mul (acc, tos) -> acc
            bin = 0x40000000 + 34;
            ok = true;
        }
        if(subs == "div") { // div (acc, tos) -> acc
            bin = 0x40000000 + 35;
            ok = true;
        }
        if(subs == "mod") { // mod (acc, tos) -> acc
            bin = 0x40000000 + 36;
            ok = true;
        }
        if(subs == "and") { // and (acc, tos) -> acc
            bin = 0x40000000 + 37;
            ok = true;
        }
        if(subs == "or") { // or (acc, tos) -> acc
            bin = 0x40000000 + 38;
            ok = true;
        }
        if(subs == "xor") { // xor (acc, tos) -> acc
            bin = 0x40000000 + 39;
            ok = true;
        }
        if(subs == "not") { // not (acc) -> acc
            bin = 0x40000000 + 40;
            ok = true;
        }
        if(subs == "lshift") { // lshift (acc, tos) -> acc
            bin = 0x40000000 + 41;
            ok = true;
        }
        if(subs == "rshift") { // rshift (acc, tos) -> acc
            bin = 0x40000000 + 42;
            ok = true;
        }

        // program flow
        if(subs == "skip") { // skip
            bin = 0x40000000 + 128;
            ok = true;
        }
        if(subs == "goto") { // goto (acc)
            bin = 0x40000000 + 129;
            ok = true;
        }
        if(subs == "test") { // test (acc, tos)
            bin = 0x40000000 + 130;
            ok = true;
        }
        if(subs == "handled") { // handled ()
            bin = 0x40000000 + 131;
            ok = true;
        }
        if(subs == "handle") { // handle (acc)
            bin = 0x40000000 + 132;
            ok = true;
        }
        if(subs == "handler") { // handler (acc, tos)
            bin = 0x40000000 + 133;
            ok = true;
        }
        
        if(subs == "bias") { // bias (acc) -> br
            bin = 0x40000000 + 134;
            ok = true;
        }
        if(subs == "err") { // err (err) -> acc
            bin = 0x40000000 + 135;
            ok = true;
        }
        if(subs == "stay") { // stay (acc)
            bin = 0x40000000 + 255;
            ok = true;
        }

        // memory control
        if(subs == "read") { // read (acc) -> acc
            bin = 0x40000000 + 256;
            ok = true;
        }
        if(subs == "write") { // write (acc, tos)
            bin = 0x40000000 + 257;
            ok = true;
        }
    }
    // if ok == false then undefined instruction
    return ok;
}

vector<i32> compileToInstructions(string& contents) {
    // count words in assm content
    istringstream iss_ctr(contents);
    int words_len = 0;
    while (iss_ctr) {
        string subs;
        iss_ctr >> subs;
        words_len ++;
    }
    // returning binary
    vector<i32> bin;
    bin.reserve(words_len + 1);
    // assm queue
    class ndat {
        public:
        char type; // [c]ode [d]ot [s]kip [b]lock
        string line;
        ndat(int) {}
        ndat(char type, string line):type(type),line(line) {}
        ndat(ndat& r) {
            r.type = type;
            r.line = line;
        }
        ndat() {}
    };

    
    i32 addr = 0;

    LinkedList<ndat*> queue;
    istringstream isss(contents);
    while (isss) {
        string subs;
        isss >> subs;
        if(subs == "{" || subs == "}") {
            // pass
        } else if(subs == "{") { // TOOD : BUG ;do not use blocks
            i32 scope = 1;
            ndat *block = new ndat('b', "");
            string word = "{";
            while(scope > 0 && isss) {
                isss >> word;
                if(word == "}") {
                    scope --;
                } else if(word == "{") {
                    scope ++;
                } else {
                    block->line += " " + word;
                    if(!isRefName(word)) {
                        addr ++;
                    }
                }
            }
            queue.push(block);
            if(word[word.length() - 1] != '}' || scope != 0) {
                bin.clear();
                return bin;
            }
        } else if(isSkip(subs)) {
            ndat *skip = new ndat('s', "");
            for(int i = 1; i < subs.length() - 1; i++) {
                skip->line += subs[i];
            }
            queue.push(skip);

            i32 b;
            istringstream num(skip->line);
            num >> b;
            addr += b;
        } else if(isRefName(subs)) {
            // ndat *dot = new ndat('d', "");
            // dot->line = subs;
            // queue.push(dot);
            subs[0] = '.';
            note *n = new note(subs, addr);
            dotnames.push(n);
        } else {
            ndat *code = new ndat('c', subs);
            queue.push(code);
        }
    }

    ndat *dat;
    LinkedList<ndat*>::Node* nndat;
    addr = 0;
    while (queue.shift(dat)) {
        if(dat->type == 'b') { // block
            vector<i32> bbin = compileToInstructions(dat->line);
            for(i32 i = 0; i < bbin.size(); i++) {
                bin.push_back(bbin[i]);
            }
        } else if(dat->type == 'c') { // code
            i32 b;
            if(get_bin(dat->line, b)) {
                bin.push_back(b);
                addr ++;
            } else {
                cout << "undefined instruction : " << dat->line << endl;
                bin.clear();
                return bin;
            }
        } else if(dat->type == 's') { // skip
            i32 b;
            istringstream num(dat->line);
            num >> b;
            addr += b;
            b += 0xc0000000;
            bin.push_back(b);
        } // else if(dat->type == 'd') { // dotname
        //     note *n = new note(dat->line, addr);
        // }
        delete dat;
    }
    return bin;
}


int main(int argc, char** argv) {
    // check for input errors
	if (argc != 3) {
		cout << "Usage: " << argv[0] << " <assm-filename> <bin-output>" << endl;
		exit(1);
	}

	// read input file
	ifstream infile;
	infile.open(argv[1]);
	if (!infile.is_open()) {
		cout << "Error: could not open [" << argv[1] << "]" << endl;
		exit(1);
	}
	string line;
	string contents;
	while (getline(infile, line)) {
		contents += line + "\n";
	}
	infile.close();

    // compile to binary
	vector<i32> instructions = compileToInstructions(contents);

	// write to binary file
	ofstream ofile;
	ofile.open(argv[2], ios::binary);
	for (i32 i = 0; i < instructions.size(); i++) {
		ofile.write(reinterpret_cast<char *>(&instructions[i]), sizeof(i32));
	}
	ofile.close();
	return 0;
}