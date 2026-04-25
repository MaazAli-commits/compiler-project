#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "vm.h"

using namespace std;

int main(int argc, char* argv[]) {
    string source;

    if (argc >= 2) {
        // Read from file
        ifstream file(argv[1]);
        if (!file) {
            cerr << "Could not open file: " << argv[1] << endl;
            return 1;
        }
        stringstream ss;
        ss << file.rdbuf();
        source = ss.str();
    } else {
        // REPL mode
        cout << "C-like Compiler REPL (type 'exit' to quit)\n";
        string line, fullInput;
        while (true) {
            cout << ">> ";
            getline(cin, line);
            if (line == "exit") break;
            fullInput += line + "\n";
            // Simple heuristic: run when we see a complete statement
            if (!fullInput.empty() && fullInput.back() == '\n') {
                try {
                    Lexer lexer(fullInput);
                    auto tokens = lexer.tokenize();
                    Parser parser(tokens);
                    auto ast = parser.parseProgram();
                    Codegen codegen;
                    codegen.generate(ast);
                    VM vm(codegen.instructions, codegen.variables);
                    vm.run();
                } catch (exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
                fullInput.clear();
            }
        }
        return 0;
    }

    try {
        // 1. Lex
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        // 2. Parse
        Parser parser(tokens);
        auto ast = parser.parseProgram();

        // 3. Codegen (with constant folding)
        Codegen codegen;
        codegen.generate(ast);

        // 4. Run VM (with profiling)
        VM vm(codegen.instructions, codegen.variables);
        vm.run();

        // 5. Print profile
        //vm.printProfile();

    } catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}