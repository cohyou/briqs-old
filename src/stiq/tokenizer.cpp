#include <cstdio>
#include "tokenizer.h"

Tokenizer::Tokenizer(const stringstream& ss, Stiqplate *p)
    : plate(p),
    current_state(NORMAL),
    next_state(NORMAL),
    tokn_name("") {
    input << ss.rdbuf();
    consume();
}

Tokenizer::~Tokenizer() {
    delete plate;
}

Tokn *Tokenizer::next_token() {
    while (current_state != FINISH) {
        scan(current_state, current_char);
    }
    Tokn *new_token = plate->make_tokn(tokn_type, tokn_name);
    current_state = next_state;
    tokn_name = "";
    return new_token;
}

void Tokenizer::scan(State s, char c) {
    switch (s) {
        case NORMAL:
        if (isws(c)) { ; }
        else if (c == '(') { finish(LBCT, "("); }
        else if (c == ')') { finish(RBCT, ")"); }
        else if (c == '"') { finish(DBQT, "\"", STRING); }
        else if (c == '\'') { finish(SGQT, "'"); }
        else if (c == ';') { current_state = COMMNT; }
        else if (c == EOF) { finish(EOF_, "EOF_"); return; }
        else { current_state = SYMBOL; tokn_name += c; }
        consume();
        break;

        case SYMBOL:
        if (isws(c)) {
            consume();
            finish(TOKN, tokn_name);
        } else if (c == '(' || c == ')' || c == '"' || c == EOF) {
            finish(TOKN, tokn_name);
        } else { tokn_name += c; consume(); }
        break;

        case STRING:
        if (c == EOF) finish(TOKN, tokn_name);
        else if (c == '"') {
            finish(TOKN, tokn_name, STREND);
        } else { tokn_name += c; consume(); }
        break;

        case STREND:
        finish(DBQT, "\"");
        consume();
        break;

        case COMMNT:
        if (c == EOF) {
            current_state = FINISH; next_state = NORMAL; consume();
        }
        else if (c == '\n') {
            current_state = NORMAL;
            consume();
        } else { consume(); }
        break;

        default:
        break;
    }
}

void Tokenizer::consume() {
    current_char = input.get();
}

void Tokenizer::finish(ToknType t, string n, State next) {
    current_state = FINISH;
    tokn_type = t;
    tokn_name = n;
    next_state = next;
}

bool Tokenizer::isws(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

string Tokenizer::get_state_name(State s) {
    string state_names[] = { "NORMAL", "SYMBOL", "STRING", "STREND", "COMMNT", "FINISH" };
    return state_names[s];
}
