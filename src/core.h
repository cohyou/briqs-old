/**
 @brief namespace briqs
 TODO: naming functions
 TODO: where namespace should be located, classes, functions, constants...
 TODO: functions have three types:
    1. internal(arguments isnt top of list, and may not return briq)
    2. Stiq's primitive function(argument may be top of list - "Briq* args", and must return briq)
    3. user-defined Stiq function(it is, indeed, merely evaluatable list)
 TODO: should make table of class, Stiq primitive.
 */

#ifndef CORE_H
#define CORE_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stack>
#include <type_traits>

namespace briqs {
    typedef unsigned long briq_index;
    enum Kind { INVL, CODE, SVAL, VCTR, CELL, };

    // TODO: wanna change name of ____
    enum Type { ____, SMBL, };
    typedef unsigned char byte;

    class Briq {
    public:
        virtual bool is_atom() const = 0;
        virtual Type type() const
            { return ____; }
        virtual bool is_self_evaluating() const
            { return true; }
        virtual std::string to_s() const = 0;
        virtual void tree() const { std::cout << to_s() << std::endl; };

        virtual void set_lptr(Briq *briq) {};
        virtual void set_gptr(Briq *briq) {};
        virtual Briq *l()
            { return nullptr; }
        virtual Briq *g()
            { return nullptr; }
        virtual ~Briq();
    };

    class None : public Briq {
        bool is_atom() const
            { return true; }
    public:
        None() {};
        std::string to_s() const
            { return "None"; }
    };

    static None* none = new None();

    class Bool : public Briq {
        bool bval_;
        bool is_atom() const
            { return true; }
    public:
        Bool(bool b) : bval_(b) {};
        bool bval();
        std::string to_s() const
            { return "Bool"; }
    };

    static Bool* fval = new Bool(false);
    static Bool* tval = new Bool(true);

    class Ui64 : public Briq {
        unsigned long ul_;
        bool is_atom() const
            { return true; }
        std::string to_s() const
            { return "Ui64"; }
        public:
            Ui64(unsigned long ul) : ul_(ul) {};
    };

    class Text : public Briq {
        std::string bval;
        bool is_atom() const
            { return true; }
    public:
        Text(std::string s) : bval(s) {};
        std::string to_s() const
            { return "Text"; }
    };

    // class Cell;
    /**
     @brief means SiGniFieR.
    */
    /*
    class Sgfr {
    public:
        virtual Cell* get_cell() = 0;
        virtual Briq* get_sval() = 0;
        virtual ~Sgfr();
    };
    */
    // template <class T>
    /*
    class Pntr : public Sgfr {
        // T* pval;
        union {
            Cell* pcel;
            Briq* pval;
        };
    public:
        Pntr(Cell *cell) : pcel(cell) {};
        Pntr(Briq *briq) : pval(briq) {};
        virtual Cell* get_cell()
            { return pcel; }
        Briq* get_sval()
            { return pval; }
        // T* operator->() const { return pval; }
        virtual ~Pntr()
            { delete pval; } // Briqの解放はBaseplateに任せる
    };
    */

    class Cell : public Briq {
        Briq* lptr;
        Briq* gptr;
        bool is_atom() const
            { return false; }
        std::string to_s() const
            { return "Cell"; }
    public:
        Cell() : lptr(nullptr), gptr(nullptr) {};
        bool is_self_evaluating() const
            { return false; }
        void set_lptr(Briq *briq)
            { lptr = briq; }
        void set_gptr(Briq *briq)
            { gptr = briq; }
        Briq* l() const
            { return lptr; }
        Briq* g() const
            { return gptr; }

        void tree() const;
    };

    const Bool* btob(bool b);
    bool is_atom(const Briq* b);
    void msg_exit(std::string msg);

} // namespace briqs

#endif // CORE_H
