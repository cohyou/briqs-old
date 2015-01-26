#include "abstractor.h"

Abstractor::Abstractor(const stringstream& ss) : input(new Tokenizer(ss, new Stiqplate())) {
    root = input->plate->make_rule("ROOT");
    current_node = root;
    consume();
}

void Abstractor::start() {
    elements();
}

Stiqplate *Abstractor::plate() {
    return input->plate;
}

unsigned int Abstractor::briq_count() {
    return input->plate->briq_count();
}

void Abstractor::consume() {
    lookahead = input->next_token();
}

void Abstractor::match(int x) {
    if (lookahead->t() == x) {
        consume();
    } else {
        cout << indent() << "!!!! SNTX ERRR !!!! (" << lookahead->name() << ")" << endl;
    }
}

void Abstractor::before() {
    List *l = input->plate->make_list();
    current_node->add_child(l);
    node_stack.push(current_node);
    current_node = l;
}

void Abstractor::after() {
    current_node = node_stack.top();
    node_stack.pop();
}

void Abstractor::list() {
    match(LBCT);
    depth++;
    elements();
    depth--;
    match(RBCT);
}

void Abstractor::elements() {
    while (lookahead->t() != EOF_ && lookahead->t() != RBCT) {
        before();
        element();
        after();
    }
}

void Abstractor::element() {
    switch (lookahead->t()) {
        case TOKN:
        add();
        match(lookahead->t());
        break;

        case LBCT:
        list();
        break;

        case SGQT:
        quote();
        break;

        case DBQT:
        text();
        break;

        default:
        cout << "!!!! SNTX ERRR !!!! [" << lookahead->name() << "]" << endl;
        lookahead = input->plate->make_tokn(EOF_, "EOF_");
        break;
    }
}

void Abstractor::quote() {
    match(SGQT);
    before();
    current_node->add_child(input->plate->make_spfm(QUOT));
    after();
    before();
    element();
    after();
}

bool Abstractor::is_digit(string n) {
    for (size_t i = 0; i < n.size(); ++i) {
        char c = n[i];
        if (i == 0 && n.size() > 1 && c == '0') return false;
        if (c == '0' || c == '1' ||c == '2' ||c == '3' ||c == '4' ||
            c == '5' || c == '6' ||c == '7' ||c == '8' ||c == '9') {
        } else {
            return false;
        }
    }

    return true;
}

void Abstractor::add() {
    string n = lookahead->name();

    if (n == "F") {
        current_node->add_child(input->plate->make_fval());
        // input->plate->vnsh_briq(lookahead);
    } else if (n == "T") {
        current_node->add_child(input->plate->make_tval());
        // input->plate->vnsh_briq(lookahead);
    } else {
        if (n == "Q" || n == "?" || n == "^" || n == ":") {
            SpfmType t;
            if (n == "Q") t = QUOT;
            if (n == "?") t = COND;
            if (n == "^") t = LMBD;
            if (n == ":") t = DEFN;
            current_node->add_child(input->plate->make_spfm(t));
            // input->plate->vnsh_briq(lookahead);
        } else if (n == "@" || n == "=" || n == "L" || n == "G" || n == "~") {
            FuncType t;
            if (n == "@") t = ATOM;
            if (n == "=") t = EQL_;
            if (n == "L") t = CAR_;
            if (n == "G") t = CDR_;
            if (n == "~") t = CONS;
            current_node->add_child(input->plate->make_func(t));
            // input->plate->vnsh_briq(lookahead);
        } else if (is_digit(n)) {
            unsigned long ul;
            istringstream iss(n);
            iss >> ul;
            current_node->add_child(input->plate->make_ui64(ul));
            // input->plate->vnsh_briq(lookahead);
        } else {
            current_node->add_child(input->plate->make_smbl(lookahead));
        }
    }
}

void Abstractor::text() {
    match(DBQT);
    depth++;
    Text *t = input->plate->make_text(lookahead->name());
    // input->plate->vnsh_briq(lookahead);
    current_node->add_child(t);
    match(TOKN);
    depth--;
    match(DBQT);
}

Abstractor::~Abstractor() {
    delete input;
}

string Abstractor::indent() {
    string s(2 * depth, ' ');
    return s;
}
