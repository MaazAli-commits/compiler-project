#pragma once
#include "codegen.h"
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

class VM {
public:
    VM(const vector<Instruction>& instructions, const vector<string>& variables);
    void run();
    void printProfile();

private:
    const vector<Instruction>& instructions;
    const vector<string>& variableNames;

    vector<int> stack;
    vector<int> memory;         // variable storage by index
    vector<int> profile;        // instruction execution count (profiling)

    int sp = 0;                 // stack pointer
    int ip = 0;                 // instruction pointer

    void push(int val);
    int pop();
};