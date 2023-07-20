#include <iostream>
#include "parser.h"
#include "store.h"

int main() {
    Store* store = build_store("../samples/spec_example.vcd");
    std::cout << store->get_top()->format_heirarchy() << std::endl;
    for (auto [s, v] : store->identifier_code_to_var){
        std::cout << s << " ";
        std::cout << v->identifier_code << " ";
        std::cout << v->identifier << " ";
        std::cout << v->size << " ";
        if (v->identifier_indexed){
            std::cout << v->msb << " ";
            std::cout << v->lsb << " ";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < 2500; i+=10){
        std::cout << i << " " << store->value_at("*$", i)->as_string() << "\n";
    }

    return 0;
}