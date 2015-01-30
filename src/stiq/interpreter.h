#include "abstractor.h"
#include "scope.h"

class Interpreter {
public:
    Interpreter(const stringstream& ss);
    ~Interpreter();
    string eval();
    unsigned int briq_count();
private:
    Briq *root;
    Stiqplate *plate;
    Abstractor *abstractor;
    Scope *global_scope;
    stack<Scope *> scope_stack;
private:
    None *N;
    Tval *T;
    Fval *F;
private:
    Briq *eval(Briq *c, const unsigned int depth);
    Briq *apply(Briq *c, const unsigned int depth);
private:
    Briq *eval_root(Briq *args, const unsigned int depth);
    Briq *eval_sval(Briq *sval, const unsigned int depth);
    Briq *eval_vctr(Briq *vctr, const unsigned int depth);
    Briq *resolve_symbol(Briq *smbl, const unsigned int depth);

    Briq *setl(Briq *args, const unsigned int depth);
    Briq *setg(Briq *args, const unsigned int depth);
    Briq *save(Briq *args, const unsigned int depth);
    Briq *load(Briq *args, const unsigned int depth);
    Briq *save_recursive(Briq *args, const unsigned int depth);
    Briq *import(Briq *args, const unsigned int depth);
    Briq *to_s(Briq *args, const unsigned int depth);
    Briq *print(Briq *args, const unsigned int depth);
    Briq *println(Briq *args, const unsigned int depth);
    Briq *str(Briq *args, const unsigned int depth);
    Briq *ln(Briq *args, const unsigned int depth);
    Briq *index(Briq *args, const unsigned int depth);
    Briq *clear_bucket(Briq *args, const unsigned int depth);

    Briq *quote(Briq *args, const unsigned int depth);
    Briq *define(Briq *args, const unsigned int depth);
    Briq *condition(Briq *args, const unsigned int depth);
    Briq *lambda(Briq *args, const unsigned int depth);
    Briq *sequence(Briq *args, const unsigned int depth);

    Briq *atom(Briq *args, const unsigned int depth);
    Briq *equal(Briq *args, const unsigned int depth);
    Briq *car(Briq *args, const unsigned int depth);
    Briq *cdr(Briq *args, const unsigned int depth);
    Briq *cons(Briq *args, const unsigned int depth);

    Briq *exec_func(Briq *lmbd, Briq *args, const unsigned int depth);
};
