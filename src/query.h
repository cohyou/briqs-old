#include "facade.h"

namespace briqs {
    enum ToknType { TOKN = 0x30, LBCT, RBCT, SGQT, DBQT, EOF_, };
    // std::string get_state_name(State s);

    void log(std::string s);
    void print(Briq *briq);

    class Tokn : public Briq {
        ToknType type_;
        std::string name_;
        bool is_atom() const
            { return true; }
    public:
        Tokn(ToknType t, std::string n)
            : type_(t), name_(n) {};
        std::string info() const
            { return name_; }
        ToknType type()
            { return type_; };
        // std::string tree();
    };

    class Smbl : public Briq {
    public:
        Smbl(std::string n)
            : name_(n) {};
        Type type() const
            { return SMBL; }
        bool is_self_evaluating() const override
            { return false; }
    private:
        std::string name_;
        bool is_atom() const
            { return true; }
        std::string info() const
            { return "SMBL[" + name_ + "]"; }
        std::string to_s() const
            { return name_; }
    };

    class Stiq;
    class Prim : public Briq {
    public:
        Prim(std::function<Briq*(Stiq*, Briq*)> f) : p(f), type_(PRIM) {};
        Prim(std::function<Briq*(Stiq*, Briq*)> f, Type t) : p(f), type_(t) {};
        Briq* operator()(Stiq* stiq, Briq* briq) override
            { return p(stiq, briq); }
        Type type() const
            { return type_; }
    private:
        Type type_;
        std::function<Briq*(Stiq*, Briq*)> p;
    private:
        bool is_atom() const
            { return true; }
        std::string info() const
            { return "PRIM"; }
    };

    class Func : public Briq {
    };

    class Scope {
        std::string name;
        Scope *enclosing_scope;
    protected:
        std::map<std::string, Briq*> symbol_table;
    public:
        Scope();
        Scope(std::string name, Scope *parent);
        std::string get_scope_name();
        Scope *get_enclosing_scope();
        void define(std::string sym, Briq *content);
        Briq *resolve(std::string name);
    };

    class Primitives : public Scope {
    public:
        Primitives();
    };

    Briq *eval(Stiq* stiq, Briq* briq);
    // Briq* resolve_symbol(Stiq* stiq, Briq* smbl);
    Briq* list_of_values(Stiq* stiq, Briq* old_list);

    Briq* apply(Briq* proc, Briq* args);

    class Stiq {
    public:
        Stiq(const std::stringstream& ss, Baseplate *p);
        ~Stiq();
    private:
        // common
        Baseplate *plate;
        std::stringstream input;

        // Lexer
        enum State {
            NORMAL,
            SYMBOL,
            STRING,
            STREND,
            COMMNT,
        };

        bool finished;
        char current_char;
        State current_state;

        ToknType tokn_type;
        std::string tokn_name;

        // Parser
        Briq* root;
        Briq* current_node;
        Tokn* current_token;
        std::stack<Briq*> node_stack;
        Briq* result;

        // Evaluator
        std::stack<Scope*> scope_stack;
    public:
        Briq* parse();
        Briq* evaluate();
        void print(Briq* briq);
        template <class T>
        Cell* make_list_item(T* briq);
    private:
        // Lexer
        Tokn *next_token();
        void consume_char();
        void scan(State s, char c);
        void finish(ToknType t, std::string n, State next = NORMAL);


        // Parser
        void consume_token();
        void match(int x);

        void before();
        void after();

        void element();
        void elements();
        void list();
        void quote();
        void text();

        void add();

        bool is_digit(std::string n);
        template <class T>
        Cell* add_child(T* briq);

        // Evaluator

        Briq* list_of_values(Briq* l);
    public:
        Briq* eval(Briq *briq);
        Briq* apply(Briq* proc, Briq* args);
        Briq* resolve_symbol(Briq* smbl);
    public:
        int depth;
    };

    /*
    Briq* car(const Briq* b);

    template<class L, class G>
    L* car(const Cell<L, G>* c)
    {
        return c->l();
    }
    */

} // namespace briqs
