#include <stack>
#include "cell.h"
#include "tokenizer.h"

class Abstractor {
public:
    Abstractor(const stringstream& ss);
    ~Abstractor();
    void start();
    Stiqplate *plate();
    unsigned int briq_count();
public:
    Cell *root;
private:
    Tokenizer *input;
    Tokn *lookahead;
    stack<Cell *> node_stack;
    Cell *current_node;

    void consume();

    void match(int x);
    void before();
    void after();

    void list();
    void elements();
    void element();
    void text();

    void add();
    void quote();
    void comment();

    bool is_digit(string n);
private:
    int depth;
    string indent();
};
