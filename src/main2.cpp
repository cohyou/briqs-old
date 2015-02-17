#include <string>
#include <iostream>

namespace briqs {

class Briq {
public:
    virtual bool is_atom() const = 0;
    virtual std::string to_string() const = 0;
};

class None : public Briq {
    bool is_atom() const
        { return true; }
public:
    std::string to_string() const
        { return "None"; }
};

class Bool : public Briq {
    bool bval;
    bool is_atom() const
        { return true; }
    std::string to_string() const
        { return "Bool"; }
public:
    Bool(bool b) : bval(b) {};
};

class Text : public Briq {
    std::string bval;
    bool is_atom() const
        { return true; }
    std::string to_string() const
        { return "Text"; }
public:
    Text(std::string s) : bval(s) {};
};

template <class L, class G>
class Cell : public Briq {
    L* lptr;
    G* gptr;
    bool is_atom() const
        { return false; }
    std::string to_string() const
        { return "Cell"; }
public:
    Cell(L* l, G* g) : lptr(l), gptr(g) {};
    L* l() const
        { return lptr; }
    G* g() const
        { return gptr; }
};

static const None* N = new None();
static const Bool* F = new Bool(false);
static const Bool* T = new Bool(true);

const Bool* btob(bool b)
    { return b ? T : F; }

bool is_atom(const Briq* b)
    { return b->is_atom(); }

class Baseplate {
    template <class L, class G>
    Cell<L, G>* make_cell(const L* l, const G* g)
        { return new Cell<L, G>(); }
};

namespace prim {


// special forms

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

const Bool* atom(const Briq* b)
    { return btob(is_atom(b)); }

template<class L, class G>
Cell<L, G>* cons(L* l, G* g)
{
    // TODO: should use Baseplate's make_cell
    return new Cell<L, G>(l, g);
}

template<class L, class G>
L* car(const Cell<L, G>* c)
{
    return c->l();
}

Briq* car(const Briq* b)
{
    std::cerr << "required list for car, but it's not list." << std::endl;
    exit(1);
    return nullptr; // never reached
}

template<class L, class G>
G* cdr(const Cell<L, G>* c)
{
    return c->g();
}

Briq* cdr(const Briq* b)
{
    std::cerr << "required list for cdr, but it's not list." << std::endl;
    exit(1);
    return nullptr; // never reached
}

const Bool* eq(const Briq* b1, const Briq* b2)
{
    // TODO: should make function resulting not 'Bool*' but 'bool'?
    // TODO: should re-think equality of briq
    return btob(b1 == b2);
}

} // namespace prim

} // namespace briqs

int main() {
    // TODO: should use facade for access primitives
    auto c = briqs::prim::cons(briqs::N, briqs::N);
    auto b = briqs::prim::car(c);
    auto d = briqs::prim::car(b);
    std::cout << b->to_string() << std::endl;
}
