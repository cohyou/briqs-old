#include <iostream>
#include <fstream>
#include "bucket.h"

using namespace std;

Bucket::Bucket(string n, Baseplate *plate)
    : bucket_name(n), baseplate(plate) {}
Bucket::~Bucket() {}

string const Bucket::name() {
    return bucket_name;
}

Briq *Bucket::save_briq(Briq *b) {
    b->set_bucket(this->name());

    if (b->kind() == CELL) {
        if (b->ltyp() == PNTR && b->lptr()) {
            b->set_lidx(b->lptr()->get_index());
        }

        if (b->gtyp() == PNTR && b->gptr()) {
            b->set_gidx(b->gptr()->get_index());
        }
    }

    if (b->has_valid_index()) {
        fstream fio(get_file_path().c_str(), ios::in | ios::out | ios::binary);
        fio.seekp(b->get_index() * b->data_size(), ios_base::beg);
        fio.write(as_bytes(b->data()[0]), b->data_size());
    } else {
        b->set_index(incr_max_id());
        ofstream fout(get_file_path().c_str(), ios::binary | ios::app);
        fout.write(as_bytes(b->data()[0]), b->data_size());
    }

    b->save_other_briqs();

    return b;
}

Briq *Bucket::save_briq_recursive(Briq *b) {
    b->set_bucket(this->name());

    if (b->kind() == CELL) {
        if (b->ltyp() == PNTR && b->lptr()) {
            save_briq_recursive(b->lptr());
            b->set_lidx(b->lptr()->get_index());
        } else if (b->ltyp() == INDX && b->lval() != 0) {
            Briq *new_b = load_briq(b->lval(), b->depth() + 1);
            save_briq_recursive(new_b);
            b->set_lidx(new_b->get_index());
        }

        if (b->gtyp() == PNTR && b->gptr()) {
            save_briq_recursive(b->gptr());
            b->set_gidx(b->gptr()->get_index());
        } else if (b->gtyp() == INDX && b->gval() != 0) {
            Briq *new_b = load_briq(b->gval(), b->depth() + 1);
            save_briq_recursive(new_b);
            b->set_gidx(new_b->get_index());
        }
    }

    return save_briq(b);
}

Briq *Bucket::load_briq(const briq_index index, const int depth) {
    string indentation(depth * 2, ' ');

    Briq *briq_work = load_data(index, depth);
    Briq *briq = cast_data(briq_work->data(), depth);

    if (briq) {
        for (size_t i = 0; i < briq->data_size(); ++i) {
            briq->data()[i] = briq_work->data()[i];
        }

        briq->set_bucket(this->name());
        briq->set_index(index);
        briq->load_other_briqs();
        briq->set_depth(depth);
    }
    delete briq_work;

    return briq;
}

void Bucket::clear() {
    ifstream fin(get_file_path().c_str());
    if (fin.is_open()) remove(get_file_path().c_str());
    fin.close();

    prepare();
}

void Bucket::prepare() {
    List b(baseplate);
    b.set_lidx(0);
    b.set_gidx(0);
    ifstream fin(get_file_path().c_str());
    if (!fin.is_open()) {
        ofstream fout(get_file_path().c_str(), ios::binary | ios::app);
        fout.write(as_bytes(b.data()[0]), b.data_size());
    }
}

briq_index Bucket::get_max_id() {
    briq_index mxid;

    fstream fio(get_file_path().c_str(), ios::in | ios::out | ios::binary);
    fio.seekp(24, ios_base::beg);
    fio.read(as_bytes(mxid), sizeof(mxid));

    return mxid;
}

briq_index Bucket::incr_max_id() {
    briq_index newmxid = get_max_id() + 1;

    fstream fio(get_file_path().c_str(), ios::in | ios::out | ios::binary);
    fio.seekp(24, ios_base::beg);
    fio.write(as_bytes(newmxid), sizeof(newmxid));

    return newmxid;
}

Briq *Bucket::load_data(const briq_index index, const int depth) {
    Briq *b = new Briq((Kind)0, baseplate);

    ifstream fin_first(get_file_path().c_str());
    if (!fin_first.is_open()) {
        prepare();
    }
    fin_first.close();

    ifstream fin(get_file_path().c_str(), ios::binary);
    fin.seekg(index * b->data_size(), ios_base::beg);
    fin.read(as_bytes(b->data()[0]), b->data_size());

    return b;
}

Briq *Bucket::cast_data(byte *data, const int depth) {
    Briq *b = 0;

    switch (data[0]) {
        case SVAL:
        switch (data[1]) {
            case FVAL: b = new Fval(baseplate); break;
            case TVAL: b = new Tval(baseplate); break;
            case UI64: b = new Ui64(0, baseplate); break;
            default:
            break;
        }
        break;

        case VCTR:
        if (data[1] == TXT_) {
            b = new Text("", baseplate);
        } else if (data[1] == TXT2) {
            b = new Text(baseplate);
        } else if (data[1] == BIN_) {
        }
        break;

        case CELL:
        switch (data[1]) {
            case LIST: b = new List(baseplate); break;
            default:
            b = new Cell((CellType)0, baseplate);
            break;
        }
        break;

        default:
        break;
    }

    return b;
}

string Bucket::get_file_path() {
    return "buckets/" + name() + ".bc";
}
