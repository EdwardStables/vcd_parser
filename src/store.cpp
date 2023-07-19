#include "store.h"
#include <iostream>

Scope::Scope(std::string scope_str){
    size_t space = scope_str.find(' ');

    std::string stype = scope_str.substr(0, space);
    std::string sname = scope_str.substr(space+1, scope_str.size()-space);

    space = sname.find(' ');
    sname = sname.substr(0, space);

    if (space <= sname.size()){
        std::cerr << "Extra data found after name in scope '" << sname << "'. Ignoring.\n";
    }

    name = sname;

    if (stype == "begin") {
        type = Type::BEGIN;
    } else
    if (stype == "fork") {
        type = Type::FORK;
    } else
    if (stype == "function") {
        type = Type::FUNCTION;
    } else
    if (stype == "module") {
        type = Type::MODULE;
    } else
    if (stype == "task") {
        type = Type::TASK;
    } else {
        std::cerr << "Unknown scope type '" << stype << ". Defaulting to module.\n";
        type = Type::MODULE;
    }
}

void Store::down_scope(Scope* scope){
    if (top_scope == nullptr){
        top_scope = scope;
        current_scope = scope;
        return;
    }

    if (current_scope->child_scopes.count(scope->name)){
        std::cerr << "Repeated scope name " << scope->name << " at same level. Unrecoverable.";
        exit(1);
    }

    current_scope->child_scopes[scope->name] = scope;
    current_scope = scope;
}

void Store::up_scope() {
    if (current_scope->parent_scope){
        current_scope = current_scope->parent_scope;
        return;
    }
    
    if (current_scope == top_scope){ 
        std::cerr << "Unexpected additional $endscope. Ignoring.";
    }
}

void Store::add_var(Var*) {

}

std::string Scope::format_heirarchy(int indent) {
    std::string sindent;
    for (int i = 0; i < indent; i++){
        sindent += " ";
    }

    std::string s = sindent + name + '\n'; 
    for (auto& c : child_scopes){
        s += c.second->format_heirarchy(indent + 2);
    }

    return s;
}

Scope* Store::get_top() { 
    return top_scope;
}