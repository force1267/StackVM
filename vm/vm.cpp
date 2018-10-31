#ifndef STACKVM
#define STACKVM



#include<iostream>
#include<sstream>
#include<vector>
using namespace std;
typedef uint32_t i32;

#include "storage.cpp"

bool verbose = true;

/*
    instruction format :
    2 bits header
    30 bits body(data)
    header format :
    0 -> positive int
    1 -> primitive instruction
    2 -> negative int // TODO
    3 -> memory skip

**/
class StackVM {
    // registers :
    i32 br = 0; // bias register
    i32 pc = -1; // program counter

    i32 sp = 0; // stack pointer
    i32 acc = 0; // accumulator
    i32 tmp = 0; // temp register
    
    vector<i32> memory;
    
    i32 typ = 0; // type of current data
    i32 dat = 0; // current data
    
    i32 hlt = 1; // is not running ?
    i32 err = 0; // error register
    // 64 : divide by zero
    // 65 : undefined header
    // 66 : undefined instruction
    // 67 : invalid 'handled' instruction // TODO : implement

    // read instructions
    i32 getData(i32 instruction) {
        return (instruction & 0x3fffffff);
    }
    i32 getType(i32 instruction) {
        return (instruction & 0xc0000000) >> 30;
    }


    void doPrimitive() {
        i32 n;
        // instruction set
        switch(dat) {
            // case int: // (int) -> acc
                // see 'void execute()'

            // machine control
            case 0: // halt
                hlt = 1;
                break;
            case 27: // dupl (tos) -> tos
                memory[br + ++sp] = memory[br + sp - 1];
                break;
            case 28: // detemp (tmp) -> acc
                acc = tmp;
                break;
            case 29: // temp (acc) -> temp
                tmp = acc;
                break;
            case 30: // push (acc) -> tos
                memory[++sp + br] = acc;
                break;
            case 31: // pop (tos) -> acc
                acc = memory[br + sp--];
                break;
            
            // arithmetics and calculations
            case 32: // add (acc, tos) -> acc
                acc += memory[br + sp--];
                break;
            case 33: // sub (acc, tos) -> acc
                acc -= memory[br + sp--];
                break;
            case 34: // mul (acc, tos) -> acc
                acc *= memory[br + sp--];
                break;
            case 35: // div (acc, tos) -> acc
                if(memory[br + sp] != 0)
                    acc /= memory[br + sp--];
                else
                    err = 64; // devide by zero
                break;
            case 36: // mod (acc, tos) -> acc
                if(memory[br + sp] != 0)
                    acc %= memory[br + sp--];
                else
                    err = 64; // devide by zero
                break;
            case 37: // and (acc, tos) -> acc
                acc = acc & memory[br + sp--];
                break;
            case 38: // or (acc, tos) -> acc
                acc = acc | memory[br + sp--];
                break;
            case 39: // xor (acc, tos) -> acc
                acc = acc ^ memory[br + sp--];
                break;
            case 40: // not (acc) -> acc
                acc = ~acc;
                break;
            case 41: // lshift (acc, tos) -> acc
                // bitwise left shift of 'acc' ,'tos' times
                acc = acc << memory[br + sp--];
                break;
            case 42: // rshift (acc, tos) -> acc
                // bitwise right shift of 'acc' ,'tos' times
                acc = acc >> memory[br + sp--];
                break;
            case 43: // eq (acc, tos) -> acc
                // is acc == tos
                acc = acc == memory[br + sp--];
                break;
            case 44: // lt (acc, tos) -> acc
                // is acc < tos
                acc = acc < memory[br + sp--];
                break;
            case 45: // gt (acc, tos) -> acc
                // is acc > tos
                acc = acc > memory[br + sp--];
                break;
            // program flow
            case 128: // skip
                /*if(verbose)*/ cout << "skip [" << hex <<
                acc << " " << dec << sp << ":" << hex << memory[br + sp] <<
                " at " << dec << pc << " from " << br;
                if(err != 0) {
                    cout << " ,err " << err;
                }
                cout << "]" << endl;
                break;
            case 129: // goto (acc)
                pc = acc - 1;
                break;
            case 130: // test (tos, acc)
                // if(tos) pass else goto acc
                if(memory[br + sp--] == 0)
                    pc = acc - 1;
                break;
            case 131: // handled ()
                // continue last instructions
                // err = 67; // invalid 'handled' instruction
                n = br;
                tmp = memory[n+6];
                acc = memory[n+5];
                sp = memory[n+4];
                pc = memory[n+3];
                br = memory[n+2];
                // enable this handler
                memory[n+1] = 0;
                // dont touch memory[n]. it contains handler's stack size
                break;
            case 132: // handle (acc)
                // handle situation of 'acc' number
                n = memory[acc];
                if(n != 0 && memory[n] == 0) {
                    memory[n+1] = 1; // disable this handler
                    memory[n+2] = br;
                    memory[n+3] = pc;
                    memory[n+4] = sp;
                    memory[n+5] = acc;
                    memory[n+6] = tmp;
                    br = n;
                    pc = -1;
                    sp = 6;
                }
                break;
            case 133: // handler (acc, tos)
                // set a high priority task handler (interrupt handler)
                // handler for situation of 'acc' number
                // is handled by instructions starting from 'tos' number
                // higher 'acc' means higher priority
                memory[acc] = memory[br + sp--]; // TODO handler address must be at acc
                memory[memory[acc] + 1] = 0; // enable the handler by default
                break;
            case 134: // bias (acc) -> br
                br = acc;
                break;
            case 135: // err (err) -> acc
                acc = err;
                err = 0;
                break;
            case 255: // stay (acc)
                // stay 'acc' cycles
                break;

            // memory control
            case 256: // read (acc) -> acc
                // acc = mem[acc]
                acc = memory[br + acc];
                break;
            case 257: // write (acc, tos)
                //mem[acc] = tos
                memory[acc] = br + sp--;
                break;

            // undefined instruction
            default:
                if(verbose) cout << "undefined instruction " << (dat & 0x3fffffff) << endl;
                err = 66; // undefined instruction
                break;
        }
    }

    // cpu cycle
    void fetch() {
        pc++;
    }
    void decode() {
        typ = getType(memory[br + pc]);
        dat = getData(memory[br + pc]);
    }
    void hpexecute() {
        if(err != 0) {
            // start error handler
            i32 n = memory[err];
            if(n != 0 && memory[n+1] == 0) {
                memory[n+1] = 1; // disable this handler
                memory[n+2] = br;
                memory[n+3] = pc;
                memory[n+4] = sp;
                memory[n+5] = acc;
                memory[n+6] = tmp;
                br = n;
                pc = -1;
                sp = 6;
            }
        }

    }
    void execute() {
        switch(typ) {
            case 0:
                acc = memory[br + pc];
                break;
            case 1:
                doPrimitive();
                break;
            case 2:
                acc = memory[br + pc];
                break;
            case 3:
                pc += dat - 1;
                break;
            // default not possible // undefined header
            default:
                break;
        }
    }
    // start cpu cycle
    void run() {
        hlt = 0;
        pc = -1;
        // br = 128; // init at boot
        if(verbose) cout << "run" << endl;
        while (!hlt) {
            fetch();
            decode();
            execute();
            hpexecute(); // high priority (handlers)
        }
        if(verbose) cout << "halt" << endl;
    }
    public:
    StackVM(int size = 1e6) {
        // reserve 1e6 words by default as memory
        memory.reserve(size);
    }
    // load program to stack from an storage
    void boot(Storage& storage) { // TODO : implement low memory error and exit
        bool booterr = false;
        i32 ptr = 0, bin;
        
        // init br with starting memory skip
        bin = storage.get(0);
        if(getType(bin) == 0b11) { // if it was memory skip
            i32 data = getData(bin);
            br = data;
            // memory[ptr++] = bin; // no memory skip header
            for(int j = 0; j < data; j++) {
                memory[ptr++] = 0; // skip with zero
            }
        }

        for(int i = 1; i < storage.getSize(); i++) {
            bin = storage.get(i);
            if(getType(bin) == 0b11) { // if it was memory skip
                i32 data = getData(bin);
                memory[ptr++] = bin;
                for(int j = 1; j < data; j++) {
                    memory[ptr++] = 0; // skip with zero
                }
            } else {
                memory[ptr++] = bin;
            }
        }
        run();
    }
};

#endif