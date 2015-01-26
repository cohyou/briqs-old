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
    Briq *set_pidx(Briq *args, const unsigned int depth);
    Briq *str(Briq *args, const unsigned int depth);
    Briq *ln(Briq *args, const unsigned int depth);
    Briq *index(Briq *args, const unsigned int depth);
    Briq *clear_bucket(Briq *args, const unsigned int depth);

    Briq *define(Briq *args, const unsigned int depth);
    Briq *condition(Briq *args, const unsigned int depth);
    Briq *lambda(Briq *args, const unsigned int depth);
    Briq *sequence(Briq *args, const unsigned int depth);

    Briq *atom(Briq *args, const unsigned int depth);
    Briq *equal(Briq *args, const unsigned int depth);
    Briq *car(Briq *args, const unsigned int depth);
    Briq *cdr(Briq *args, const unsigned int depth);
    Briq *cons(Briq *args, const unsigned int depth);
};
