// investigate bin files
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
typedef uint32_t i32;

i32 getData(i32 instruction) {
    return (instruction & 0x3fffffff);
}
i32 getType(i32 instruction) {
    return (instruction & 0xc0000000) >> 30;
}

int main(int argc, char** argv) {
    if (argc < 2) {
		cout << "Usage: " << argv[0] << " <program-binary>" << endl;
		return 0;
	}

	ifstream r(argv[1], ios::binary);
	i32 i;

	while (r.read((char*)&i, sizeof(i))) {
        // brute investigate: cout << getType(i) << " " << getData(i) << endl; if(false)
        if(getType(i) == 3) {
            cout << "(" << getData(i) << ") " << endl;
        } else if(getType(i) == 0 || getType(i) == 2) {
            cout << getData(i) << " ";
        } else switch(getData(i)) {
            // case int: // (int) -> acc
                // see 'void execute()'

            // machine control
            case 0: // halt
                cout << "halt" << endl;
                break;
            case 27: // dupl (tos) -> tos
                cout << "dupl";
                break;
            case 28: // detemp (tmp) -> acc
                cout << "detemp";
                break;
            case 29: // temp (acc) -> temp
                cout << "temp" << endl;
                break;
            case 30: // push (acc) -> tos
                cout << "push" << endl;
                break;
            case 31: // pop (tos) -> acc
                cout << "pop";
                break;
            
            // arithmetics and calculations
            case 32: // add (acc, tos) -> acc
                cout << "+";
                break;
            case 33: // sub (acc, tos) -> acc
                cout << "-";
                break;
            case 34: // mul (acc, tos) -> acc
                cout << "*";
                break;
            case 35: // div (acc, tos) -> acc
                cout << "/";
                break;
            case 36: // mod (acc, tos) -> acc
                cout << "\37";
                break;
            case 37: // and (acc, tos) -> acc
                cout << "&";
                break;
            case 38: // or (acc, tos) -> acc
                cout << "|";
                break;
            case 39: // xor (acc, tos) -> acc
                cout << "^";
                break;
            case 40: // not (acc) -> acc
                cout << "~";
                break;
            case 41: // lshift (acc, tos) -> acc
                cout << "<<";
                break;
            case 42: // rshift (acc, tos) -> acc
                cout << ">>";
                break;

            // program flow
            case 128: // skip
                cout << "skip" << endl;
                break;
            case 129: // goto (acc)
                cout << "goto" << endl;
                break;
            case 130: // test (tos, acc)
                cout << "test" << endl;
                break;
            case 131: // handled ()
                cout << "handled" << endl;
                break;
            case 132: // handle (acc)
                cout << "handle" << endl;
                break;
            case 133: // handler (acc, tos)
                cout << "handler" << endl;
                break;
            case 134: // bias (acc) -> br
                cout << "bias" << endl;
                break;
            case 135: // err (err) -> acc
                cout << "err";
                break;
            case 255: // stay (acc)
                cout << "stay" << endl;
                break;

            // memory control
            case 256: // read (acc) -> acc
                cout << "read";
                break;
            case 257: // write (acc, tos)
                cout << "write";
                break;

            // undefined instruction
            default:
                cout << "udef_ins(" << getData(i) << ") " << endl;
                break;
        }
    }

    return 0;
}