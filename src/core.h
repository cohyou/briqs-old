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
#include <set>
#include <stack>
#include <algorithm>
#include <type_traits>
#include <memory>

namespace briqs {
    typedef unsigned long briq_index;
    enum Kind { INVL, CODE, SVAL, VCTR, CELL, };

    // TODO: wanna change name of ____
    enum Type { ____, NONE, TEXT, SMBL, SPFM, PRIM, FUNC, };
    typedef unsigned char byte;

    class Briq;
    /**
     @brief means SiGniFieR.
    */
    class Sgfr {
    public:
        virtual Briq* get() { return nullptr; };
        virtual std::string type() { return ""; };
        bool operator==(Sgfr another_sgfr) {
            return false;
        }
        bool operator<(const Sgfr& another_sgfr) const {
            return false;
        }
        virtual std::string bucket_name() { return ""; };
        virtual briq_index get_index() { return ULONG_MAX; };
        virtual void set_index(briq_index idx) {};
    };

    class Stiq;
    class Briq {
    protected:
        std::string target_bucket_name;
        std::map<std::string, briq_index> denoter_infos;
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
        virtual std::string name() const { return to_s(); };
        virtual unsigned long ul() const { return ULONG_MAX; };

        virtual void set_lptr(Briq *briq) {};
        virtual void set_gptr(Briq *briq) {};
        /*
        void set_sgfr(Sgfr* sgfr)
            { signifiers.insert(sgfr); }
        */
        void set_target_bucket(std::string bucket_name);
        virtual Briq *l()
            { return nullptr; }
        virtual Briq *g()
            { return nullptr; }
        virtual ~Briq();
        virtual byte* cast_to_data() { return nullptr; };
        virtual briq_index get_index();
        virtual void set_index(briq_index idx);
        virtual std::string vstr() const { return ""; };
        bool has_valid_index();
    private:
        bool exists_in(std::string bucket_name);
        briq_index get_index_of(std::string bucket_name);
        void set_index_of(std::string bucket_name, briq_index index);
    };

    class None : public Briq {
        bool is_atom() const override
            { return true; }
    public:
        None() {};
        Type type() const override
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
        byte* cast_to_data() override;
    };

    static Bool* fval = new Bool(false);
    static Bool* tval = new Bool(true);

    class Ui64 : public Briq {
        unsigned long ul_;
        bool is_atom() const override
            { return true; }
        std::string info() const override
            { return "Ui64"; }
        public:
            Ui64(unsigned long ul) : ul_(ul) {};
        unsigned long ul() const override
            { return ul_; }
        byte* cast_to_data() override;
    };

    class Text : public Briq {
        std::string bval;
        bool is_atom() const override
            { return true; }
    public:
        Text(std::string s) : bval(s) {};
        Type type() const override
            { return TEXT; }
        std::string info() const override
            { return "Text\"" + bval + "\""; }
        std::string to_s() const override
            { return "\"" + bval + "\""; }
        std::string name() const override
            { return bval; };
        std::string vstr() const override
            { return bval; };
    };

    class Pntr : public Sgfr {
    public:
        Pntr(Briq *briq) : pval(briq) {};
        Briq* get() override
            { std::cout << "pntr" << std::endl; return pval; }
        std::string type() override
            { return "PNTR"; }
    private:
        Briq* pval;
    };

    class Cell : public Briq {
        Sgfr* lsgr;
        Sgfr* gsgr;
        std::string info() const
            { return "CELL"; }
    public:
        Cell() : lsgr(nullptr), gsgr(nullptr) {};
        bool is_atom() const override
            { return false; }
        bool is_self_evaluating() const override
            { return false; }
        void set_lptr(Briq *briq) override
            { std::cout << "set_lptr" << std::endl;
              lsgr = new Pntr(briq); /* lsgr = std::make_shared<Pntr>(briq); */ }
        void set_gptr(Briq *briq) override
            { gsgr = new Pntr(briq); /* std::make_shared<Pntr>(briq); */ }
        Briq* l() override {
            Briq* briq = lsgr->get();

            if (lsgr->type() == "DNTR") {
                set_lptr(briq);
                briq->set_target_bucket(lsgr->bucket_name());
                briq->set_index(lsgr->get_index());
            }

            return briq;
        }
        Briq* g() override {
            Briq* briq = gsgr->get();
            if (gsgr->type() == "DNTR") {
                set_gptr(briq);
                briq->set_target_bucket(gsgr->bucket_name());
                briq->set_index(gsgr->get_index());
            }
            return briq;
        }

        std::string tree() const override;
        byte* cast_to_data() override;
        ~Cell() {
            delete lsgr;
            delete gsgr;
        }
    };

    Bool* btob(bool b);
    bool is_atom(Briq* b);
    void msg_exit(std::string msg);

} // namespace briqs

#endif // CORE_H
