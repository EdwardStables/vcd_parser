#include "store.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>

std::vector<std::string> split_inner(std::string s, std::string header, int expected) {
    //https://stackoverflow.com/a/5607650
    std::stringstream ss(s);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin, end);

    if (vstrings.size() < expected){
        std::cerr << header + " declaration is missing expected content." << std::endl;
        exit(1);
    }

    if (vstrings.size() > expected){
        std::cerr << header + " declaration has unexpected content. Ignoring extra." << std::endl;
    }

    return vstrings;
}

Var::Var(std::string var_str){
    std::vector<std::string> inner = split_inner(var_str, "$var", 4);

    std::string vtype = inner[0];
    std::string identifier = inner[3];

    size = std::stoi(inner[1]);
    identifier_code = inner[2];

    if (vtype == "integer"){
        type = Type::INTEGER;
    } else
    if (vtype == "parameter") {
        type = Type::PARAMETER;
    } else
    if (vtype == "real") {
        type = Type::REAL;
    } else
    if (vtype == "reg") {
        type = Type::REG;
    } else
    if (vtype == "wire") {
        type = Type::WIRE;
    } else {
        std::cerr << "Use of unknown variable type '" << vtype << "'. Setting to wire.\n";
        type = Type::WIRE;
    }

    size_t bracket_start = identifier.find('[');
    if (bracket_start > identifier.size()){
        this->identifier = identifier;
        msb = 0;
        lsb = 0;
        return;
    } 

    identifier_indexed = true;

    this->identifier = identifier.substr(0, bracket_start);

    std::string bracket_inner = identifier.substr(bracket_start+1);
    int bracket_end = bracket_inner.find(']');

    if (bracket_end > bracket_inner.size()){
        std::cerr << "Identifier " +identifier + " did not meet expected format\n";
        exit(1);
    }

    bracket_inner = bracket_inner.substr(0, bracket_end);

    int bracket_inner_colon = bracket_inner.find(':');
    if (bracket_inner_colon > bracket_inner.size()){// [x]
        lsb = msb = std::stoi(bracket_inner);
        return;
    }

    msb = std::stoi(bracket_inner.substr(0, bracket_inner_colon));
    lsb = std::stoi(bracket_inner.substr(bracket_inner_colon+1));
}

Scope::Scope(std::string scope_str){

    std::vector<std::string> inner = split_inner(scope_str, "$scope", 2);

    std::string stype = inner[0];
    name = inner[1];

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
    if (top_scope && !current_scope){
        std::cerr << "Multiple top level scopes present. Not supported." << std::endl;
        exit(1);
    }

    if (!top_scope){
        top_scope = scope;
        current_scope = scope;
        return;
    }

    if (current_scope->child_scopes.count(scope->name)){
        std::cerr << "Repeated scope name " << scope->name << " at same level. Unrecoverable.";
        exit(1);
    }

    scope->parent_scope = current_scope;
    current_scope->child_scopes[scope->name] = scope;
    current_scope = scope;
}

void Store::up_scope() {
    if (!current_scope){ 
        std::cerr << "Unexpected additional $endscope. Ignoring.\n";
    } else {
        current_scope = current_scope->parent_scope;
    }
}

void Store::add_var(Var* v) {
    if (identifier_code_to_var.count(v->identifier_code)){
        std::cerr << "Repeated identifier code " << v->identifier_code << ". Unrecoverable.";
        exit(1);
    }

    identifier_code_to_var[v->identifier_code] = v;
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