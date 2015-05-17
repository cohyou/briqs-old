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
    enum Type { ____, NONE, SMBL, QUOT, PRIM, };
    typedef unsigned char byte;

    class Stiq;
    class Briq {
    public:
        virtual bool is_atom() const = 0;
        virtual Kind kind() const
            { return INVL; }
        virtual Type type() const
            { return ____; }
        virtual bool is_self_evaluating() const
            { return true; }
        virtual Briq* operator()(Stiq* stiq, Briq* b)
            { return nullptr; }
        virtual std::string info() const = 0;
        virtual std::string to_s() const { return info(); };
        virtual std::string tree() const { return to_s(); };

        virtual void set_lptr(Briq *briq) { /*std::cout << "Briq::set_lptr" << std::endl;*/ };
        virtual void set_gptr(Briq *briq) { /*std::cout << "Briq::set_gptr" << std::endl;*/ };
        virtual Briq *l() const
            { return nullptr; }
        virtual Briq *g() const
            { return nullptr; }
        virtual ~Briq();
    };

    class None : public Briq {
        bool is_atom() const
            { return true; }
    public:
        None() {};
        Type type() const
            { return NONE; }
        std::string info() const override
            { return "NONE"; }
        std::string to_s() const override
            { return "N"; }
    };

    static None* none = new None();

    class Bool : public Briq {
        bool bval_;
        bool is_atom() const
            { return true; }
    public:
        Bool(bool b) : bval_(b) {};
        bool bval();
        std::string info() const override
            { return bval_ ? "TVAL" : "FVAL"; }
        std::string to_s() const override
            { return bval_ ? "T" : "F"; }
    };

    static Bool* fval = new Bool(false);
    static Bool* tval = new Bool(true);

    class Ui64 : public Briq {
        unsigned long ul_;
        bool is_atom() const
            { return true; }
        std::string info() const
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
        std::string info() const
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
        std::string info() const
            { return "CELL"; }
    public:
        Cell() : lptr(nullptr), gptr(nullptr) {};
        bool is_atom() const
            { return false; }
        bool is_self_evaluating() const
            { return false; }
        void set_lptr(Briq *briq)
            { /*std::cout << "Cell::set_lptr" << std::endl;*/ lptr = briq; }
        void set_gptr(Briq *briq)
            { /*std::cout << "Cell::set_gptr" << std::endl;*/ gptr = briq; }
        Briq* l() const
            { return lptr; }
        Briq* g() const
            { return gptr; }

        std::string tree();
    };

    const Bool* btob(bool b);
    bool is_atom(const Briq* b);
    void msg_exit(std::string msg);

} // namespace briqs

#endif // CORE_H
