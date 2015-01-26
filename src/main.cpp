#include <iostream>
#include <fstream>
#include <sstream>
#include "interpreter.h"

int main(int argc, char *argv[]) {
    fstream fs(argv[1]);
    stringstream ss;
    ss << fs.rdbuf();
    Interpreter interpreter(ss);
    cout << interpreter.eval() << endl;
}
