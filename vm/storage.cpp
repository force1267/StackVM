#ifndef STORAGE
#define STORAGE

#include<iostream>
#include<vector>
using namespace std;
typedef uint32_t i32;


class Storage {
    protected:
    int size;
    public:
    int getSize() { return size; };
    virtual void set(int index, i32 data) = 0;
    virtual i32 get(int index) = 0;
    virtual i32& operator[](int index) = 0;
};

class InMemoryStorage :public Storage {
    vector<i32> memory;
    public:
    InMemoryStorage(int size) {
        this->size = size;
        memory.reserve(size);
    }
    InMemoryStorage(vector<i32>& from) {
        size = from.size();
        memory.reserve(size);
        for(int i = 0; i < size; i++) {
            memory[i] = from[i];
        }
    }
    void set(int index, i32 data) { memory[index] = data; }
    i32 get(int index) { return memory[index]; }
    i32& operator[](int index) { return memory[index]; }
};

// implement class FileStorage :public Storage {};

#endif