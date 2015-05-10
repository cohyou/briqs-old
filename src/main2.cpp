#include "query.h"

using namespace briqs;

void rep(const char* path) {
    Baseplate* plate = new Baseplate();
    std::fstream fs(path);
    std::stringstream ss;
    ss << fs.rdbuf();
    std::cout << ss.str() << std::endl;
    // ss << "ab cd";

    /*
    Lexer lexer(ss, plate);
    Tokn* tokn;
    tokn = lexer.next_token();
    while (tokn->type() != EOF_) {
        std::cout << "|" << tokn->to_s();
        // std::cout << tokn->type() << std::endl;
        tokn = lexer.next_token();
    }
    */

    Parser parser(ss);
    parser.start();
    parser.print();

    /*
    Interpreter interpreter(ss);
    cout << interpreter.eval() << endl;
    */
}

Briq* wowow(Briq* briq) { std::cout << "wowow wowow" << std::endl; return nullptr; }

int main() {
    [](){ std::cout << "Hello, world!" << std::endl; }();

    std::function<Briq*(Briq*)> f = wowow;

    f(nullptr);

    /*
    Baseplate* plate = new Baseplate();
    auto c = plate->make<Cell<None, None>>(none, none);
    auto b = car(c);
    // auto d = car(b); // error

    auto t = plate->make<Text>("wowow");
    auto n = plate->make<None>();

    std::cout << t->to_s() << std::endl;

    std::string bucket_name = "bool_save_test";
    plate->save(tval, bucket_name);
    plate->save(fval, bucket_name);

    auto load_0 = plate->load<Bool>(0, bucket_name);
    std::cout << load_0->to_s() << std::endl;
    std::cout << load_0->bval() << std::endl;
    auto load_1 = plate->load<Bool>(1, bucket_name);
    std::cout << load_1->to_s() << std::endl;
    std::cout << load_1->bval() << std::endl;
    */

    std::string path = "stiq/__.iq";
    rep(path.c_str());
}
