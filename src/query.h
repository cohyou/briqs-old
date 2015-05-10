#include "facade.h"

namespace briqs {
    enum ToknType { TOKN = 0x30, LBCT, RBCT, SGQT, DBQT, EOF_, };
    // std::string get_state_name(State s);

    class Tokn : public Briq {
        ToknType type_;
        std::string name_;
        bool is_atom() const
            { return true; }
    public:
        Tokn(ToknType t, std::string n)
            : type_(t), name_(n) {};
        std::string to_s() const
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
    private:
        std::string name_;
        bool is_atom() const
            { return true; }
        std::string to_s() const
            { return name_; }
    };

    class Prim : public Briq {
    };

    class Func : public Briq {
    };

    class Lexer {
    public:
        Lexer(const std::stringstream& ss, Baseplate *p);
        Tokn *next_token();
        Baseplate *plate();
        ~Lexer();
    private:
        enum State {
            NORMAL,
            SYMBOL,
            STRING,
            STREND,
            COMMNT,
        };

        Baseplate *plate_;
        std::stringstream input;

        bool finished;
        char current_char;
        State current_state;

        ToknType tokn_type;
        std::string tokn_name;
    private:
        void consume();
        void scan(State s, char c);
        void finish(ToknType t, std::string n, State next = NORMAL);
    };

    class Parser {
    public:
        Parser(const std::stringstream& ss);
        void start();
        void print();
        ~Parser();
    private:
        Lexer* lexer;
        Cell* root;
        Cell* current_node;
        Tokn* current_token;
        std::stack<Cell *> node_stack;
    private:
        void consume();
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
        /*
        Stiqplate *plate();
        unsigned int briq_count();
    private:
        int depth;
        string indent();
        */
    private:
        template <class T>
        Cell* make_list_item(T* briq);
        template <class T>
        Cell* add_child(T* briq);
    };

    class Scope {
        std::string name;
        Scope *enclosingScope;
        std::map<std::string, Briq *> symbolTable;
    public:
        Scope();
        Scope(std::string name, Scope *parent);
        std::string getScopeName();
        Scope *getEnclosingScope();
        void define(std::string sym, Briq *content);
        Briq *resolve(std::string name);
    };

    class Stiq {
        Briq *eval(Briq *briq);
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
