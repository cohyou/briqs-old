#include "query.h"

namespace briqs {
    bool is_whitespace(char c) {
        return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
    }

    /*
    std::string get_state_name(State s) {
        std::string state_names[] = { "NORMAL", "SYMBOL", "STRING", "STREND", "COMMNT", "FINISH" };
        return state_names[s];
    }
    */

    /*
    std::string Tokn::tree() {
        return indent() + "TOKN<" + name() + ">";
    }

    std::string Tokn::to_s() {
        return "TOKN<" + name() + ">";
    }
    */

    Lexer::Lexer(const std::stringstream& ss, Baseplate *p)
        : plate_(p)
        , current_state(NORMAL)
        , tokn_name("") {
        input << ss.rdbuf();
        consume();
    }

    Tokn *Lexer::next_token() {
        while (!finished) {
            scan(current_state, current_char);
            // std::cout << "wowow" << std::endl;
        }
        Tokn *new_token = plate_->make<Tokn>(tokn_type, tokn_name);

        finished = false;
        tokn_name = "";

        return new_token;
    }

    Baseplate *Lexer::plate()
        { return plate_; }

    Lexer::~Lexer()
        { delete plate_; }

    void Lexer::consume()
        { current_char = input.get(); }

    void Lexer::scan(State s, char c) {
        switch (s) {
            case NORMAL:
            if (is_whitespace(c)) { ; }
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
            if (is_whitespace(c)) {
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
                finished = true; current_state = NORMAL; consume();
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

    void Lexer::finish(ToknType t, std::string n, State next) {
        finished = true;
        tokn_type = t;
        tokn_name = n;
        current_state = next;
    }

    Parser::Parser(const std::stringstream& ss)
        : lexer(new Lexer(ss, new Baseplate())) {
        root = lexer->plate()->make<Cell>();
        root->set_gptr(none);
        current_node = root;

        auto smbl = lexer->plate()->make<Smbl>("list");
        add_child(smbl);

        consume();
    }

    void Parser::start() {
        while (current_token->type() != EOF_)
            { element(); }
    }

    void Parser::print()
        { root->tree(); }

    Parser::~Parser()
        { delete lexer; }

    void Parser::consume()
        { current_token = lexer->next_token(); }

    void Parser::before() {
        // auto *l = lexer->plate()->make<Cell>();
        auto *new_cell = add_child<Cell>(nullptr);
        node_stack.push(current_node);
        current_node = new_cell;
    }

    void Parser::after() {
        current_node = node_stack.top();
        node_stack.pop();
    }

    void Parser::element() {
        switch (current_token->type()) {
            case TOKN:
            add();
            match(current_token->type());
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
            std::cout << "!!!! SNTX ERRR !!!! [" << current_token->to_s() << "]" << std::endl;
            current_token = lexer->plate()->make<Tokn>(EOF_, "EOF_");
            break;
        }
    }

    void Parser::elements() {
        while (current_token->type() != EOF_ &&
               current_token->type() != RBCT) {
            before();
            element();
            after();
        }
    }

    /*
    Stiqplate *Parser::plate() {
        return input->plate;
    }

    unsigned int Parser::briq_count() {
        return input->plate->briq_count();
    }
    */

    void Parser::match(int x) {
        if (current_token->type() == x) {
            consume();
        } else {
            // std::cout << indent() << "!!!! SNTX ERRR !!!! (" << current_token->to_s() << ")" << std::endl;
            std::cout << "!!!! SNTX ERRR !!!! (" << current_token->to_s() << ")" << std::endl;
        }
    }

    void Parser::list() {
        match(LBCT);
        // depth++;
        elements();
        // depth--;
        match(RBCT);
    }

    void Parser::quote() {
        match(SGQT);
        before();
        add_child(lexer->plate()->make<Smbl>("Q"));
        element();
        after();
    }

    bool Parser::is_digit(std::string n) {
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

    void Parser::add() {
        std::string n = current_token->to_s();

        if (n == "F") {
            add_child(fval);
        } else if (n == "T") {
            add_child(tval);
        } else if (n == "N") {
            add_child(none);
        } else {
            /*
            if (n == "Q" || n == "?" || n == "^" || n == ":") {
                SpfmType t;
                if (n == "Q") t = QUOT;
                if (n == "?") t = COND;
                if (n == "^") t = LMBD;
                if (n == ":") t = DEFN;
                current_node->add_child(input->plate->make_spfm(t));
            } else if (n == "@" || n == "=" || n == "L" || n == "G" || n == "~") {
                FuncType t;
                if (n == "@") t = ATOM;
                if (n == "=") t = EQL_;
                if (n == "L") t = CAR_;
                if (n == "G") t = CDR_;
                if (n == "~") t = CONS;
                current_node->add_child(input->plate->make_func(t));
            } else*/ if (is_digit(n)) {
                unsigned long ul;
                std::istringstream iss(n);
                iss >> ul;
                add_child(lexer->plate()->make<Ui64>(ul));
            } else {
                add_child(lexer->plate()->make<Smbl>(current_token->to_s()));
            }
        }
    }

    void Parser::text() {
        match(DBQT);
        // depth++;
        Text *text = lexer->plate()->make<Text>(current_token->to_s());
        add_child(text);
        match(TOKN);
        // depth--;
        match(DBQT);
    }

    /*
    string Parser::indent() {
        string s(2 * depth, ' ');
        return s;
    }
    */

    template <class T>
    Cell* Parser::make_list_item(T* briq) {
        auto cell = lexer->plate()->make<Cell>();
        cell->set_lptr(briq);
        cell->set_gptr(none);
        return cell;
    }

    template <class T>
    Cell* Parser::add_child(T* briq) {
        // if (!briq) return nullptr;

        auto new_cell = make_list_item(briq);

        auto c = current_node->l();
        if (!c) {
            current_node->set_lptr(new_cell);
        } else {
            while (c) {
                if (!c->g()) {
                    c->set_gptr(new_cell);
                    break;
                }
                c = c->g();
            }
        }

        return new_cell;
    }

    Scope::Scope() : name("global"), enclosingScope(NULL) {}
    Scope::Scope(std::string n, Scope *parent) : name(n), enclosingScope(parent) {}

    std::string Scope::getScopeName() { return name; }
    Scope *Scope::getEnclosingScope() { return enclosingScope; }
    void Scope::define(std::string sym, Briq *content) { symbolTable[sym] = content; }
    Briq *Scope::resolve(std::string name) {
        Briq *result;
        result = symbolTable[name];
        if (result != NULL) return result;
        if (enclosingScope != NULL) return enclosingScope->resolve(name);
        std::cout << "!!!! CANT RSLV !!!! NAME: [" << name << "]" << std::endl;
        return NULL;
    }

    Briq *Stiq::eval(Briq *briq) {
        if (briq->is_self_evaluating()) {
            return briq;
        } else if (briq->type() == SMBL) {
            return briq;
        }
        return nullptr;
    }

    Briq* quote(Briq* b)
        { return b; }

    Briq* cond(Briq* args)
    {
        // TODO: should decide how evaluate first argument to Bool*
        return args;
    }

    Briq *lambda(Briq* args)
    {
        // TODO: should decide internal specification of 'lambda form'
        return args;
    }

    Briq *define(Briq* args)
    {
        // TODO: should implement this method
        // TODO: need environment for this method
        return args;
    }


    // functions
    /*
    const Bool* atom(const Briq* b)
        { return btob(is_atom(b)); }

    template<class L, class G>
    Cell<L, G>* cons(L* l, G* g)
    {
        // TODO: should use Baseplate's make_cell
        return new Cell<L, G>(l, g);
    }

    Briq* car(const Briq* b)
    {
        msg_exit("required list for car, but it's not list.");
        return nullptr; // never reached
    }

    template<class L, class G>
    G* cdr(const Cell<L, G>* c)
    {
        return c->g();
    }

    Briq* cdr(const Briq* b)
    {
        msg_exit("required list for cdr, but it's not list.");
        return nullptr; // never reached
    }

    const Bool* eq(const Briq* b1, const Briq* b2)
    {
        // TODO: should make function resulting not 'Bool*' but 'bool'?
        // TODO: should re-think equality of briq
        return btob(b1 == b2);
    }
    */
} // namespace briqs
