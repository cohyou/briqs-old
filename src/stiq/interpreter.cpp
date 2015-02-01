#include "interpreter.h"

Interpreter::Interpreter(const stringstream& ss)
: abstractor(new Abstractor(ss)) {
    plate = abstractor->plate();
    root = plate->make_rule("ROOT");
    global_scope = new Scope();
    scope_stack.push(global_scope);

    N = plate->make_none();
    T = plate->make_tval();
    F = plate->make_fval();
}

Interpreter::~Interpreter() {
    delete abstractor;
    delete global_scope;
}

string Interpreter::eval() {
    Briq *r = 0;
    // cout << "---- BEGN EVAL ---- briq_count: " << dec << briq_count() << endl;

    abstractor->start();
    // cout << "---- MAKE ASTs ---- briq_count: " << dec << briq_count() << endl;

    plate->mark_sweep_briqs(abstractor->root);
    // cout << "_GC_ AFTR MAKE ASTs briq_count: " << dec << briq_count() << endl;

    r = eval(abstractor->root, 0);

    plate->mark_sweep_briqs(r);
    // cout << "_GC_ AFTR EVAL ROOT briq_count: " << dec << briq_count() << endl;

    string result = r->to_s();
    plate->mark_sweep_briqs(r);
    // cout << "---- FNSH EVAL ---- briq_count: " << dec << briq_count() << endl;

    return result;
}

unsigned int Interpreter::briq_count() {
    return plate->briq_count();
}

Briq *Interpreter::eval(Briq *n, const unsigned int depth) {
    // cout << string(4 * depth, ' ') << "EVAL BEGN " << n->to_s() << "" << endl;

    Briq *result = 0;

    if (n->type() == RULE && n->name() == "ROOT") { result = eval_root(n->l(), depth); }
    else if (n->kind() == SVAL) { result = eval_sval(n, depth); }
    else if (n->type() == TXT_) { result = eval_vctr(n, depth); }
    else if (n->type() == SMBL) { result = resolve_symbol(n, depth); }
    else if (n->type() == LIST && n->l()->type() == SPFM) {
        if (n->l()->sbtp3() == QUOT) { result = quote(n->g(), depth); }
        else if (n->l()->sbtp3() == DEFN) { result = define(n->g(), depth); }
        else if (n->l()->sbtp3() == COND) { result = condition(n->g(), depth); }
        else if (n->l()->sbtp3() == LMBD) { result = lambda(n->g(), depth); }
        else {
            cout << "!!!! INVL SPFM !!!!" << endl;
            result = N;
        }
    }
    else if (n->type() == RULE && n->name() == "SEQ_") { result = sequence(n, depth); }
    else if (n->type() == LIST) {
        if (n->l() && n->l()->type() == SMBL) {
            string symbol_name = n->l()->name();
            if (symbol_name == "setl") { result = setl(n->g(), depth); }
            else if (symbol_name == "setg") { result = setg(n->g(), depth); }
            else if (symbol_name == "index") { result = index(n->g(), depth); }
            else if (symbol_name == "save") { result = save(n->g(), depth); }
            else if (symbol_name == "load") { result = load(n->g(), depth); }
            else if (symbol_name == "bucket") { result = clear_bucket(n->g(), depth); }
            else if (symbol_name == "save-recursive") { result = save_recursive(n->g(), depth); }
            else if (symbol_name == "import") { result = import(n->g(), depth); }
            else if (symbol_name == "to_s") { result = to_s(n->g(), depth); }
            else if (symbol_name == "print") { result = print(n->g(), depth); }
            else if (symbol_name == "println") { result = println(n->g(), depth); }
            else if (symbol_name == "string") { result = str(n->g(), depth); }
            else if (symbol_name == "ln") { result = ln(n->g(), depth); }
            else if (symbol_name == "list") { result = eval_list(n->g(), depth); }
            else if (symbol_name == "<") { result = lt(eval_list(n->g(), depth), depth); }
            else if (symbol_name == ">") { result = gt(eval_list(n->g(), depth), depth); }
            else { result = apply(n, depth); }
        } else {
            result = apply(n, depth);
        }
    }
    else if (n->type() == FUNC) { result = n; }
    else {
        cout << "!!!! EVAL ERRR !!!!" << endl;
    }

    // cout << string(4 * depth, ' ') << "EVAL FNSH " << result->to_s() << "" << endl;

    return result;
}

Briq *Interpreter::apply(Briq *n, const unsigned int depth) {
    Briq *result = 0;
    Briq *f = eval(n->l(), depth + 1);

    if (!f) {
        cout << "!!!! APLY ERRR !!!! " << n->l()->tree() << " _IS_ NULL" << endl;
    }

    if (f->sbtp3() == ATOM) { result = atom(n->g(), depth); }
    else if (f->sbtp3() == EQL_) { result = equal(n->g(), depth); }
    else if (f->sbtp3() == CAR_) { result = car(n->g(), depth); }
    else if (f->sbtp3() == CDR_) { result = cdr(n->g(), depth); }
    else if (f->sbtp3() == CONS) { result = cons(n->g(), depth); }
    else if (n->kind() == CELL) {
        if (f->type() == RULE && f->name() == "LMBD") {
            result = exec_func(f->l(), n->g(), depth);
        }
    } else {
        cout << "!!!! APLY ERRR !!!!" << endl;
    }

    return result;
}

Briq *Interpreter::eval_root(Briq *args, const unsigned int depth) {
    Briq *b = args;
    while (b) {
        List *l = plate->make_list();
        l->add_child(eval(b->l(), depth + 1));
        root->add_child(l);
        b = b->g();
    }
    return root;
}

Briq *Interpreter::eval_sval(Briq *sval, const unsigned int depth) {
    Briq *result = 0;

    if (sval->type() == NONE) {
        result = N;
    } else if (sval->type() == TVAL) {
        result = T;
    } else if (sval->type() == FVAL) {
        result = F;
    } else if (sval->type() == UI64) {
        result = plate->make_ui64(sval->lval());
    } else {
        result = 0; //new Sval((SvalType)n->type);
    }
    // plate->vnsh_briq(n);

    return result;
}

Briq *Interpreter::eval_vctr(Briq *vctr, const unsigned int depth) {
    return vctr;
}

Briq *Interpreter::resolve_symbol(Briq *smbl, const unsigned int depth) {
    string symbol_name = smbl->name();
    return scope_stack.top()->resolve(symbol_name);
}

Briq *Interpreter::setl(Briq *args, const unsigned int depth) {
    Briq *arg1 = eval(args->l(), depth + 1);
    Briq *arg2 = eval(args->g()->l(), depth + 1);
    arg1->set_lptr(arg2);
    return arg1;
}

Briq *Interpreter::setg(Briq *args, const unsigned int depth) {
    Briq *arg1 = eval(args->l(), depth + 1);
    Briq *arg2 = eval(args->g()->l(), depth + 1);
    arg1->set_gptr(arg2);
    return arg1;
}

Briq *Interpreter::save(Briq *args, const unsigned int depth) {
    Briq *save_target = eval(args->l(), depth + 1);
    Briq *bucket_text = eval(args->g()->l(), depth + 1);

    return plate->save_briq(save_target, bucket_text->name());
}

Briq *Interpreter::load(Briq *args, const unsigned int depth) {
    Briq *briq_id = eval(args->l(), depth + 1);
    Briq *bucket_text = eval(args->g()->l(), depth + 1);

    return plate->load_briq(briq_id->lval(), bucket_text->name(), 0);
}

Briq *Interpreter::save_recursive(Briq *args, const unsigned int depth) {
    Briq *save_target = eval(args->l(), depth + 1);
    Briq *bucket_text = eval(args->g()->l(), depth + 1);

    return plate->save_briq_recursive(save_target, bucket_text->name());
}

Briq *Interpreter::import(Briq *args, const unsigned int depth) {
    Briq *result = 0;
    Briq *bucket_text = eval(args->l(), depth + 1);

    Briq *ent = plate->load_briq(0, bucket_text->name(), 0);
    Briq *statement = ent->l();
    result = statement;
    while (statement) {
        result = eval(statement->l(), depth + 1);
        statement = statement->g();
    }
    return result;
}

Briq *Interpreter::to_s(Briq *args, const unsigned int depth) {
    Briq *target = eval(args->l(), depth + 1);
    return plate->make_text(target->to_s());
}

Briq *Interpreter::print(Briq *args, const unsigned int depth) {
    Briq *target = eval(args->l(), depth + 1);
    cout << target->name();
    return plate->make_rule("UNDF");
}

Briq *Interpreter::println(Briq *args, const unsigned int depth) {
    Briq *target = eval(args->l(), depth + 1);
    cout << target->name() << endl;
    return plate->make_rule("UNDF");
}

Briq *Interpreter::str(Briq *args, const unsigned int depth) {
    stringstream ss;
    Briq *arg_list = args;
    while (arg_list) {
        Briq *arg = eval(arg_list->l(), depth + 1);
        ss << arg->name();
        arg_list = arg_list->g();
    }
    return plate->make_text(ss.str());
}

Briq *Interpreter::ln(Briq *args, const unsigned int depth) {
    return plate->make_text("\n");
}

Briq *Interpreter::index(Briq *args, const unsigned int depth) {
    Briq *arg1 = eval(args->l(), depth + 1);
    return plate->make_ui64(arg1->get_index());
}

Briq *Interpreter::clear_bucket(Briq *args, const unsigned int depth) {
    Briq *bucket_text = eval(args->l(), depth + 1);
    plate->clear_bucket(bucket_text->name());
    return bucket_text; // plate->load_briq(0, bucket_text->name(), 0);
}

Briq *Interpreter::quote(Briq *args, const unsigned int depth) {
    return args->l();
}

Briq *Interpreter::define(Briq *args, const unsigned int depth) {
    Briq *sym = args->l();
    Briq *cnt = eval(args->g()->l(), depth + 1);
    scope_stack.top()->define(sym->name(), cnt);
    return sym;
}

Briq *Interpreter::condition(Briq *args, const unsigned int depth) {
    Briq *result = 0;
    Briq *p = eval(args->l(), depth + 1);
    if (p->type() != NONE && p->type() != FVAL && !(p->type() == CELL && !p->l())) {
        result = eval(args->g()->l(), depth + 1);
    } else {
        if (args->g()->g()) {
            result = eval(args->g()->g()->l(), depth + 1);
        } else {
            result = plate->make_rule("UNDF");
        }
    }
    return result;
}

Briq *Interpreter::lambda(Briq *args, const unsigned int depth) {
    Briq *result = 0;
    Rule *r = plate->make_rule("LMBD");
    if (args->g()) {
        Briq *arguments = args->l();
        List *argList = plate->make_list();
        argList->add_child(arguments);
        r->add_child(argList);

        Rule *sequence = plate->make_rule("SEQ_");
        Briq *statement = args->g();
        while (statement) {
            List *l = plate->make_list();
            l->add_child(statement->l());
            sequence->add_child(l);
            statement = statement->g();
        }
        r->add_child(sequence);
        result = r;
    } else {
        cout << "!!!! INVL ARGS LMBD !!!!" << endl;
    }
    return result;
}

Briq *Interpreter::sequence(Briq *seq_list, const unsigned int depth) {
    Briq *result = 0;
    Briq *statement = seq_list->l();
    while (statement) {
        if (statement->g()) {
            eval(statement->l(), depth + 1);
        } else {
            result = eval(statement->l(), depth + 1);
        }
        statement = statement->g();
    }
    return result;
}

Briq *Interpreter::atom(Briq *args, const unsigned int depth) {
    Briq *result = 0;
    if (eval(args->l(), depth + 1)->kind() != CELL) {
        result = T;
    } else {
        result = F;
    }
    return result;
}

Briq *Interpreter::equal(Briq *args, const unsigned int depth) {
    Briq *result = 0;

    Briq *arg1 = eval(args->l(), depth + 1);
    Briq *arg2 = eval(args->g()->l(), depth + 1);

    if (arg1->type() == FVAL || arg1->type() == TVAL) {
        if (arg1 == arg2) {
            result = T;
        } else {
            result = F;
        }
    } else if (arg1->type() == TXT_) {
        if (arg1->name() == arg2->name()) {
            result = T;
        } else {
            result = F;
        }
    } else if (arg1->type() == SMBL) {
        if (arg1->name() == arg2->name()) {
            result = T;
        } else {
            result = F;
        }
    } else if (arg1->type() == UI64) {
        if (arg1->lval() == arg2->lval()) {
            result = T;
        } else {
            result = F;
        }
    } else {
        result = F;
    }

    return result;
}

Briq *Interpreter::car(Briq *args, const unsigned int depth) {
    Briq *result = 0;

    Briq *arg1 = eval(args->l(), depth + 1);
    if (arg1->l()) {
        result = arg1->l();
        if (!result) {
            result = N;
        }
    } else {
        result = N;
    }

    return result;
}

Briq *Interpreter::cdr(Briq *args, const unsigned int depth) {
    Briq *result = 0;

    Briq *arg1 = eval(args->l(), depth + 1);
    if (arg1->l()) {
        result = arg1->g();
        if (!result) {
            result = N;
        }
    } else {
        result = N;
    }

    return result;
}

Briq *Interpreter::cons(Briq *args, const unsigned int depth) {
    List *l = plate->make_list();

    if (args) {
        Briq *arg1 = eval(args->l(), depth + 1);
        l->set_lptr(arg1);

        if (args->g()) {
            Briq *arg2 = eval(args->g()->l(), depth + 1);
            l->set_gptr(arg2);
        }
    }

    return l;
}

Briq *Interpreter::eval_list(Briq *args, const unsigned int depth) {
    List *first = plate->make_list();
    Briq *l = first;
    Briq *arg_list = args;
    while (arg_list) {
        l->set_lptr(eval(arg_list->l(), depth + 1));
        l->set_gptr(plate->make_list());
        l = l->g();
        arg_list = arg_list->g();
    }
    return first;
}

Briq *Interpreter::gt(Briq *args, const unsigned int depth) {
    Briq *result = N;
    Briq *arg1 = args->l();
    Briq *arg2 = args->g()->l();
    if (arg1->type() == TXT_) {
        result = bool_to_TF(arg1->vstr() > arg2->vstr());
    }
    return result;
}

Briq *Interpreter::lt(Briq *args, const unsigned int depth) {
    Briq *result = N;
    Briq *arg1 = args->l();
    Briq *arg2 = args->g()->l();
    if (arg1->type() == TXT_) {
        result = bool_to_TF(arg1->vstr() < arg2->vstr());
    }
    return result;
}

Briq *Interpreter::exec_func(Briq *lmbd, Briq *args, const unsigned int depth) {
    Briq *result = 0;
    Briq *params = lmbd->l();
    Briq *lambda_body = lmbd->g();

    scope_stack.push(new Scope("FuncScope", scope_stack.top()));

    while (params) {
        Briq *param_smbl = params->l();
        scope_stack.top()->define(param_smbl->name(), eval(args->l(), depth + 1));
        params = params->g();
        args = args->g();
    }
    result = eval(lambda_body, depth + 1);

    Scope *top = scope_stack.top();
    delete top;
    scope_stack.pop();

    return result;
}

Briq *Interpreter::bool_to_TF(bool b) {
    Briq *r;
    if (b) {
        r = T;
    } else {
        r = F;
    }
    return r;
}
