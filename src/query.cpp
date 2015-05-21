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

        auto smbl = plate->make<Smbl>(" ");
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
        auto* new_cell = add_child<Cell>(nullptr);
        node_stack.push(current_node);
        current_node = new_cell;
    }

    void Stiq::after() {
        if (!current_node->l()) {
            current_node->set_lptr(none);
        }
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
            make_quoted_list();
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

    void Stiq::make_quoted_list() {
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
        if (briq->is_atom()) {
            std::cout << briq->to_s();
        } else {
            std::cout << "(";
            auto c = briq;
            while (c != none) {
                print(c->l());
                c = c->g();
                if (c != none) {
                    std::cout << " ";
                }
            }
            std::cout << ")";
        }
    }

    void print_tree(Briq *briq) {
        std::function<void(Briq*, int)> print_tree_internal = [&](Briq* b, int i){
            std::string indent(i * 2, ' ');
            std::cout << indent << b->info() << std::endl;
            if (!b->is_atom()) {
                auto lptr = b->l();
                if (lptr) print_tree_internal(lptr, i + 1);
                auto gptr = b->g();
                if (gptr) print_tree_internal(gptr, i);
            }
        };
        print_tree_internal(briq, 0);
    }

    Briq* top_level(Stiq* stiq, Briq* old_list) {
        auto old_cell = old_list;
        Briq* new_list_head = none;
        Briq* new_list_tail = none;
        while (old_cell != none) {
            auto new_cell = stiq->make_list_item(old_cell->l());
            if (new_list_tail == none) {
                new_list_head = new_cell;
                new_list_tail = new_list_head;
            } else {
                new_list_tail->set_gptr(new_cell);
                new_list_tail = new_cell;
            }
            old_cell = old_cell->g();
        }
        return new_list_head;
    }

    Briq* list_of_values(Stiq* stiq, Briq* old_list) {
        auto old_cell = old_list;
        Briq* new_list_head = none;
        Briq* new_list_tail = none;
        while (old_cell != none) {
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
        return new_list_head;
    }

    Briq* quote(Stiq* stiq, Briq* briq) {
        return briq->l();
    }

    Briq* Stiq::evaluate() {
        result = plate->make<Cell>();
        depth = -1;
        result->set_lptr(eval(root->l()));
        result->set_gptr(none);
        return result;
    }

    void Stiq::print(Briq* briq) {
        ::briqs::print(briq);
    }

    void Stiq::print_tree(Briq* briq) {
        ::briqs::print_tree(briq);
    }


    // Evaluator
    Briq* eval(Stiq* stiq, Briq* briq) {
        ++(stiq->depth);
        Briq *result = nullptr;
        std::string indent(stiq->depth * 2, ' ');
        std::cout << indent << "EVAL BEGN "; print(briq); std::cout << std::endl;

        if (briq->is_self_evaluating()) { result = briq; }
        else if (briq->type() == SMBL)
            { result = stiq->resolve_symbol(briq); }
        else if (!briq->is_atom()) {
            Briq *ope = eval(stiq, briq->l());
            if (ope == none) {
                log(indent + "apply operator is none!");
                result = none;
            } else if (ope->type() == SPFM) {
                result = (*ope)(stiq, briq->g());
            } else {
                Briq *args = list_of_values(stiq, briq->g());
                result = stiq->apply(ope, args);
            }
        }

        std::cout << indent << "EVAL FNSH "; print(result); std::cout << std::endl;
        --(stiq->depth);
        return result;
    }

    Briq* Stiq::eval(Briq* briq) {
        return ::briqs::eval(this, briq);
    }

    Briq* exec_func(Stiq* stiq, Briq *lmbd, Briq *args) {
        Briq *result = nullptr;
        Briq *params = lmbd->l();
        Briq *lambda_body = lmbd->g();

        stiq->scope_stack.push(new Scope("FuncScope", stiq->scope_stack.top()));

        while (params != none) {
            Briq* param_smbl = params->l();
            std::string symbol_name = param_smbl->to_s();
            Briq* resolved_param = eval(stiq, args->l());
            stiq->scope_stack.top()->define(symbol_name, resolved_param);
            params = params->g();
            args = args->g();
        }
        print(lambda_body);
        result = sequence(stiq, lambda_body);

        Scope *top = stiq->scope_stack.top();
        delete top;
        stiq->scope_stack.pop();

        return result;
    }

    Briq* Stiq::apply(Briq* proc, Briq* args) {
        if (proc->type() == PRIM) { return (*proc)(this, args); }
        else if (proc->type() == FUNC) {
            return exec_func(this, proc, args);
        }
        return none;
    }

    Briq* Stiq::resolve_symbol(Briq *smbl) {
        std::string symbol_name = smbl->to_s();
        return scope_stack.top()->resolve(symbol_name);
    }

    Briq* Stiq::list_of_values(Briq* old_list) {
        return ::briqs::list_of_values(this, old_list);
    }

    Briq* define_symbol(Stiq* stiq, Briq* args) {
        // TODO: need environment for this method
        Briq *sym = args->l();
        Briq *cnt = eval(stiq, args->g()->l());
        stiq->scope_stack.top()->define(sym->to_s(), cnt);
        return sym;
    }

    Briq* cond(Stiq* stiq, Briq* args)
    {
        // TODO: should decide how evaluate first argument to Bool*
        Briq *result = nullptr;
        Briq *p = eval(stiq, args->l());
        if (p != none && p != fval && !(!p->is_atom() && !p->l())) {
            result = eval(stiq, args->g()->l());
        } else {
            if (args->g()->g()) {
                result = eval(stiq, args->g()->g()->l());
            } else {
                result = none;
            }
        }
        return result;
    }

    Briq* sequence(Stiq* stiq, Briq *seq_list) {
        Briq *result = none;
        Briq *statement = seq_list;
        while (statement != none) {
            if (statement->g() != none) {
                eval(stiq, statement->l());
            } else {
                result = eval(stiq, statement->l());
            }
            statement = statement->g();
        }
        return result;
    }

    std::string Func::info() const {
        log("params:"); print(func_params);
        log("\nbody:"); print(func_body);log("");
        return "FUNC";
    }

    Briq *lambda(Stiq* stiq, Briq* args)
    {
        // TODO: should decide internal specification of 'lambda form'
        Briq* result = nullptr;

        if (args->g()) {
            Briq *parameters = args->l();
            result = stiq->plate->make<Func>(args->l(), args->g());
        } else {
            log("!!!! INVL ARGS LMBD !!!!");
        }

        return result;
    }

    // functions
    Bool* atom(Stiq* stiq, Briq* args)
        { return btob(is_atom(args->l())); }

    Briq* cons(Stiq* stiq, Briq* args) {
        // TODO: should use Baseplate's make_cell
        auto new_cell = stiq->plate->make<Cell>();
        new_cell->set_lptr(args->l());
        new_cell->set_gptr(args->g()->l());
        return new_cell;
    }

    Briq* car(Stiq* stiq, Briq* args) {
        Briq *result = none;
        print(args);
        if (args->l() != none) {
            result = args->l()->l();
        } else {
            result = none;
        }

        return result;
    }

    Briq* cdr(Stiq* stiq, Briq* args) {
        Briq *result = none;

        if (args->l() != none) {
            result = args->l()->g();
        } else {
            result = none;
        }

        return result;
    }

    Briq* eq(Stiq* stiq, Briq *args) {
        // TODO: should make function resulting not 'Bool*' but 'bool'?
        // TODO: should re-think equality of briq
        Briq *result = nullptr;

        Briq *arg1 = args->l();
        Briq *arg2 = args->g()->l();

        if (arg1 == fval || arg1 == tval) {
            result = btob(arg1 == arg2);
        } else if (arg1->type() == TEXT || arg1->type() == SMBL) {
            result = btob(arg1->to_s() == arg2->to_s());
        } else {
            result = fval;
        }
        return result;
    }

    // other primitives
    Briq* clear_bucket(Stiq *stiq, Briq *args) {
        Briq* bucket_text = args->l();
        stiq->plate->clear_bucket(bucket_text->name());
        return bucket_text; // plate->load_briq(0, bucket_text->name(), 0);
    }

    Briq* setl(Stiq* stiq, Briq* args) {
        Briq* arg1 = args->l();
        Briq* arg2 = args->g()->l();
        arg1->set_lptr(arg2);
        return arg1;
    }

    Briq* setg(Stiq *stiq, Briq *args) {
        Briq* arg1 = args->l();
        Briq* arg2 = args->g()->l();
        arg1->set_gptr(arg2);
        return arg1;
    }

    Briq* save(Stiq* stiq, Briq* args) {
        Briq *save_target = args->l();
        Briq *bucket_text = args->g()->l();

        return stiq->plate->save_briq(save_target, bucket_text->name());
    }

    Briq* load(Stiq* stiq, Briq *args) {
        Briq *briq_id = args->l();
        Briq *bucket_text = args->g()->l();

        return stiq->plate->load(briq_id->ul(), bucket_text->name());
    }

    Briq* import(Stiq* stiq, Briq *args) {
        Briq *result = none;
        Briq *bucket_text = args->l();

        Briq *ent = stiq->plate->load(0, bucket_text->name());
        Briq *statement = ent->l();
        result = statement;
        while (statement) {
            result = eval(stiq, statement->l());
            statement = statement->g();
        }
        return result;
    }

    Briq* index(Stiq* stiq, Briq *args) {
        Briq *arg1 = args->l();
        return stiq->plate->make<Ui64>(arg1->get_index());
    }

    Briq* to_s(Stiq* stiq, Briq *args) {
        Briq *target = args->l();
        return stiq->plate->make<Text>(target->to_s());
    }

    Briq* print_no_break(Stiq* stiq, Briq *args) {
        Briq *target = args->l();
        std::cout << target->name();
        return none;
    }

    Briq* print_break(Stiq* stiq, Briq *args) {
        Briq *target = args->l();
        std::cout << target->name() << std::endl;
        return none;
    }

    Briq* str(Stiq* stiq, Briq *args) {
        std::stringstream ss;
        Briq* arg_list = args;
        while (arg_list) {
            Briq* arg = arg_list->l();
            ss << arg->name();
            arg_list = arg_list->g();
        }
        return stiq->plate->make<Text>(ss.str());
    }

    Briq* ln(Stiq* stiq, Briq *args) {
        return stiq->plate->make<Text>("\n");
    }

    Briq* gt(Stiq* stiq, Briq *args) {
        Briq *result = none;
        Briq *arg1 = args->l();
        Briq *arg2 = args->g()->l();
        if (arg1->type() == TEXT) {
            result = btob(arg1->vstr() > arg2->vstr());
        }
        return result;
    }

    Briq* lt(Stiq* stiq, Briq *args) {
        Briq *result = none;
        Briq *arg1 = args->l();
        Briq *arg2 = args->g()->l();
        if (arg1->type() == TEXT) {
            result = btob(arg1->vstr() < arg2->vstr());
        }
        return result;
    }

    Briq* save_recursive(Stiq* stiq, Briq *args) {
        Briq *save_target = args->l();
        Briq *bucket_text = args->g()->l();

        return stiq->plate->save_briq_recursive(save_target, bucket_text->name());
    }

    Primitives::Primitives() {
        symbol_table = {
            {" ", new Prim(top_level)},

            {"Q", new Prim(quote, SPFM)},
            {":", new Prim(define_symbol, SPFM)},
            {"?", new Prim(cond, SPFM)},
            {"^", new Prim(lambda, SPFM)},

            {"@", new Prim(atom)},
            {"=", new Prim(eq)},
            {"~", new Prim(cons)},
            {"L", new Prim(car)},
            {"G", new Prim(cdr)},

            {"list", new Prim(list_of_values)},

            {"bucket", new Prim(clear_bucket)},
            {"setl", new Prim(setl)},
            {"setg", new Prim(setg)},
            {"save", new Prim(save)},
            {"load", new Prim(load)},

            {"import", new Prim(import)},

            {"index", new Prim(index)},

            {"to_s", new Prim(to_s)},
            {"print", new Prim(print_no_break)},
            {"println", new Prim(print_break)},
            {"string", new Prim(str)},
            {"ln", new Prim(ln)},
            {"<", new Prim(lt)},
            {">", new Prim(gt)},

            {"save-recursive", new Prim(save_recursive)},
        };
    };
} // namespace briqs
