#include <iostream>
#include <cassert>
#include <iomanip>
#include <sstream>
#include "briq.h"
#include "bucket.h"
#include "baseplate.h"

using namespace std;

Briq::Briq(Kind k, Baseplate *plate)
    : baseplate(plate), target_bucket_name(), briq_depth(0) {
    assert(baseplate);
    denoters.clear();
    for (size_t i = 0; i < sizeof(d.data); ++i) {
        d.data[i] = 0;
    }
    d.kind = k;
}

Briq::~Briq() {}

size_t Briq::data_size() {
    return sizeof(d.data);
}

byte *Briq::data() {
    return d.data;
}

Kind Briq::kind() {
    return (Kind)d.kind;
}

byte Briq::type() {
    return d.type;
}

byte Briq::sbtp3() {
    return d.sbtp3;
}

unsigned long Briq::lval() {
    return d.lval;
}

unsigned long Briq::gval() {
    return d.gval;
}

Briq *Briq::lptr() {
    return d.lptr;
}

Briq *Briq::gptr() {
    return d.gptr;
}

byte *Briq::vector_data() {
    return d.vctrdata;
}

void Briq::set_bucket(string bucket_name) {
    target_bucket_name = bucket_name;
}

string Briq::get_bucket() {
    return target_bucket_name;
}

bool Briq::has_valid_index() {
    return (exists_in(get_bucket()));
}

void Briq::set_index(briq_index index) {
    set_index_of(get_bucket(), index);
}

briq_index Briq::get_index() {
    return get_index_of(get_bucket());
}

bool Briq::exists_in(string bucket_name) {
    return (denoters.count(bucket_name) > 0);
}

void Briq::set_index_of(string bucket_name, briq_index index) {
    denoters[bucket_name] = index;
}

briq_index Briq::get_index_of(string bucket_name) {
    return denoters[bucket_name];
}

void Briq::mark() {
    d.sbtp6 = 1;
}

bool Briq::is_marked() {
    return (d.sbtp6 == 1);
}

void Briq::set_depth(unsigned int dpth) {
    briq_depth = dpth;
}

unsigned int Briq::depth() {
    return briq_depth;
}

string Briq::indent() {
    string spaces(briq_depth * 2, ' ');
    return spaces;
}

string Briq::get_kind_name() {
    static const string kindNames[] = { "INVL", "CODE", "SVAL", "VCTR", "CELL", };
    if (d.kind >= 5) return "????";
    return kindNames[d.kind];
}

string Briq::info() {
    stringstream ss;

    cout << hex << uppercase << setfill('0');
    cout << hex << setw(8) << get_index();
    cout << " ";
    cout << "[" << setw(4) << left << get_kind_name() << "]";

    return ss.str();
}

string Briq::tree() {
    return indent() + "BRIQ[" + get_kind_name() + "]";
}

string Briq::to_s() {
    return "BRIQ<" + get_kind_name() + ">";
}

string Briq::to_s_impl() {
    return "";
}

string Briq::name() {
    return "";
}

void Briq::save_other_briqs() {}

void Briq::load_other_briqs() {}

void Briq::mark_other_briqs() {}

byte Briq::ltyp() {
    return 0;
}

byte Briq::gtyp() {
    return 0;
}

Briq *Briq::l() {
    return 0;
}

Briq *Briq::g() {
    return 0;
}

void Briq::set_lptr(Briq* ptr) {}

void Briq::set_gptr(Briq* ptr) {}

void Briq::set_lidx(briq_index idx) {}

void Briq::set_gidx(briq_index idx) {}

void Briq::add_child(Briq *b) {}
