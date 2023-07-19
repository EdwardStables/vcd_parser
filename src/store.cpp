#include "store.h"
#include <iostream>

Scope::Scope(std::string scope_str){
    std::cout << scope_str << std::endl;
}

void Store::down_scope(Scope* scope){

}

void Store::up_scope() {

}

void Store::add_var(Var*) {

}