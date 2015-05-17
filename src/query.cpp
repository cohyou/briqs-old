#include "query.h"

namespace briqs {
    void log(std::string s)
        { std::cout << s << std::endl; }

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

    Scope::Scope() : name(""), enclosing_scope(nullptr) {}
    Scope::Scope(std::string n, Scope *parent) : name(n), enclosing_scope(parent) {}

    std::string Scope::get_scope_name() { return name; }
    Scope *Scope::get_enclosing_scope() { return enclosing_scope; }
    void Scope::define(std::string sym, Briq *content) { symbol_table[sym] = content; }
    Briq *Scope::resolve(std::string name) {
        Briq *result;
        result = symbol_table[name];
        if (result != nullptr) return result;
        if (enclosing_scope != nullptr) return enclosing_scope->resolve(name);
        std::cout << "!!!! CANT RSLV !!!! NAME: [" << name << "]" << std::endl;
        return nullptr;
    }

    Stiq::Stiq(const std::stringstream& ss, Baseplate *p)
        : plate(p)
        , current_state(NORMAL)
        , tokn_name("")
        {
        // Lexer
        input << ss.rdbuf();
        consume_char();

        // Parser
        root = plate->make<Cell>();
        root->set_gptr(none);
        current_node = root;

        auto smbl = plate->make<Smbl>("list");
        add_child(smbl);

        consume_token();

        // Evaluator
        auto primitives = new Primitives();
        scope_stack.push(primitives);
        auto global_scope = new Scope("global", primitives);
        scope_stack.push(global_scope);
    }

    Stiq::~Stiq() {
        delete plate;
    }

    // Lexer
    Tokn *Stiq::next_token() {
        while (!finished) {
            scan(current_state, current_char);
        }
        Tokn *new_token = plate->make<Tokn>(tokn_type, tokn_name);

        finished = false;
        tokn_name = "";

        return new_token;
    }

    void Stiq::consume_char()
        { current_char = input.get(); }

    void Stiq::scan(State s, char c) {
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
            consume_char();
            break;

            case SYMBOL:
            if (is_whitespace(c)) {
                consume_char();
                finish(TOKN, tokn_name);
            } else if (c == '(' || c == ')' || c == '"' || c == EOF) {
                finish(TOKN, tokn_name);
            } else { tokn_name += c; consume_char(); }
            break;

            case STRING:
            if (c == EOF) finish(TOKN, tokn_name);
            else if (c == '"') {
                finish(TOKN, tokn_name, STREND);
            } else { tokn_name += c; consume_char(); }
            break;

            case STREND:
            finish(DBQT, "\"");
            consume_char();
            break;

            case COMMNT:
            if (c == EOF) {
                finished = true; current_state = NORMAL; consume_char();
            }
            else if (c == '\n') {
                current_state = NORMAL;
                consume_char();
            } else { consume_char(); }
            break;

            default:
            break;
        }
    }

    void Stiq::finish(ToknType t, std::string n, State next) {
        finished = true;
        tokn_type = t;
        tokn_name = n;
        current_state = next;
    }

    void Stiq::consume_token()
        { current_token = next_token(); }

    void Stiq::before() {
        auto *new_cell = add_child<Cell>(nullptr);
        node_stack.push(current_node);
        current_node = new_cell;
    }

    void Stiq::after() {
        current_node = node_stack.top();
        node_stack.pop();
    }

    void Stiq::element() {
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
            current_token = plate->make<Tokn>(EOF_, "EOF_");
            break;
        }
    }

    void Stiq::elements() {
        before();
        while (current_token->type() != EOF_ &&
               current_token->type() != RBCT) {
            element();
        }
        after();
    }

    void Stiq::match(int x) {
        if (current_token->type() == x) {
            consume_token();
        } else {
            std::cout << "!!!! SNTX ERRR !!!! (" << current_token->to_s() << ")" << std::endl;
        }
    }

    void Stiq::list() {
        match(LBCT);
        elements();
        match(RBCT);
    }

    void Stiq::quote() {
        match(SGQT);
        before();
        add_child(plate->make<Smbl>("Q"));
        element();
        after();
    }

    bool Stiq::is_digit(std::string n) {
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

    void Stiq::add() {
        std::string n = current_token->to_s();

        if (n == "F") {
            add_child(fval);
        } else if (n == "T") {
            add_child(tval);
        } else if (n == "N") {
            add_child(none);
        } else {
            if (is_digit(n)) {
                unsigned long ul;
                std::istringstream iss(n);
                iss >> ul;
                add_child(plate->make<Ui64>(ul));
            } else {
                add_child(plate->make<Smbl>(current_token->to_s()));
            }
        }
    }

    void Stiq::text() {
        match(DBQT);
        Text *text = plate->make<Text>(current_token->to_s());
        add_child(text);
        match(TOKN);
        match(DBQT);
    }

    template <class T>
    Cell* Stiq::make_list_item(T* briq) {
        auto cell = plate->make<Cell>();
        cell->set_lptr(briq);
        cell->set_gptr(none);
        return cell;
    }

    template <class T>
    Cell* Stiq::add_child(T* briq) {
        // if (!briq) return nullptr;

        auto new_cell = make_list_item(briq);

        auto c = current_node->l();
        if (!c || c->type() == NONE) {
            current_node->set_lptr(new_cell);
        } else {
            while (c) {
                if (!c->g() || c->g()->type() == NONE) {
                    c->set_gptr(new_cell);
                    break;
                }
                c = c->g();
            }
        }

        return new_cell;
    }


    // Parser
    Briq* Stiq::parse() {
        while (current_token->type() != EOF_)
            { element(); }

        return root;
    }

    void print(Briq *briq) {
        std::function<void(Briq*, int)> print_internal = [&](Briq* b, int i){
            std::string indent(i * 2, ' ');
            std::cout << indent << b->info() << std::endl;
            if (!b->is_atom()) {
                auto lptr = b->l();
                if (lptr) print_internal(lptr, i + 1);
                auto gptr = b->g();
                if (gptr) print_internal(gptr, i);
            }
        };
        print_internal(briq, 0);
    }

    Briq* list_of_values(Stiq* stiq, Briq* old_list) {
        // log("list_of_values "+old_list->info());
        auto old_cell = old_list;
        Briq* new_list_head = none;
        Briq* new_list_tail = none;
        while (old_cell != none) {
            // log("list_of_values loop");
            auto new_cell = stiq->make_list_item(eval(stiq, old_cell->l()));
            if (new_list_tail == none) {
                new_list_head = new_cell;
                new_list_tail = new_list_head;
            } else {
                new_list_tail->set_gptr(new_cell);
                new_list_tail = new_cell;
            }
            old_cell = old_cell->g();
        }
        // log("list_of_values return "+old_list->info());
        return new_list_head;
    }

    Briq* quote(Stiq* stiq, Briq* briq) {
        return briq->l();
    }

    Briq* Stiq::evaluate() {
        result = plate->make<Cell>();
        depth = -1;
        result->set_lptr(eval(root->l()));
        return result;
    }

    void Stiq::print(Briq* briq) {
        ::briqs::print(briq);
    }


    // Evaluator
    Briq* eval(Stiq* stiq, Briq* briq) {
        ++(stiq->depth);
        Briq *result = nullptr;
        std::string indent(stiq->depth * 2, ' ');
        log(indent + "EVAL BEGN " + briq->info());

        if (briq->is_self_evaluating()) { result = briq; }
        else if (briq->type() == SMBL)
            { result = stiq->resolve_symbol(briq); }
        else if (!briq->is_atom()) {
            Briq *ope = eval(stiq, briq->l());
            if (ope == none) {
                log(indent + "apply operator is none!");
                result =  none;
            } else if (ope->type() == QUOT) {
                result = (*ope)(stiq, briq->g());
            } else {
                Briq *args = list_of_values(stiq, briq->g());
                result = stiq->apply(ope, args);
            }
        }

        log(indent + "EVAL FNSH " + result->info());
        --(stiq->depth);
        return result;
    }

    Briq* Stiq::eval(Briq* briq) {
        return ::briqs::eval(this, briq);
    }

    Briq* Stiq::apply(Briq* proc, Briq* args) {
        if (proc->type() == PRIM) { return (*proc)(this, args); }
        return none;
    }

    Briq* Stiq::resolve_symbol(Briq *smbl) {
        std::string symbol_name = smbl->to_s();
        return scope_stack.top()->resolve(symbol_name);
    }

    Briq* Stiq::list_of_values(Briq* old_list) {
        return ::briqs::list_of_values(this, old_list);
    }

    Primitives::Primitives() {
        symbol_table = {
            {"list", new Prim(list_of_values)},
            {"Q", new Prim(quote, QUOT)},
        };
    };
} // namespace briqs
