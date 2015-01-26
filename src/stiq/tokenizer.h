#include <iostream>
#include <sstream>
#include "stiqplate.h"
#include "tokn.h"

class Tokenizer {
public:
    Tokenizer(const stringstream& ss, Stiqplate *p);
    ~Tokenizer();
public:
    Stiqplate *plate;
public:
    Tokn *next_token();
private:
    stringstream input;
    enum State {
        NORMAL,
        SYMBOL,
        STRING,
        STREND,
        COMMNT,
        FINISH,
    };
    State current_state;
    State next_state;
    char current_char;
    ToknType tokn_type;
    string tokn_name;

    void scan(State s, char c);
    void consume();
    void finish(ToknType t, string n, State next = NORMAL);

    bool isws(char c);
    string get_state_name(State s);
};
