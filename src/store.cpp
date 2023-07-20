#include "store.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>

BinaryStore::BinaryStore(int _size, std::string bit_string) : size(_size) {
    while (bit_string.size() < size){
        bit_string = '0' + bit_string;
    }

    for (int i = size-1; i >= 0; i--){
        char c = bit_string[i];
        if (c == 'x' || c == 'X'){
            b_vector.push_back(false);
            x_vector.push_back(true);
            z_vector.push_back(false);
        } else
        if (c == 'z' || c == 'Z'){
            b_vector.push_back(false);
            x_vector.push_back(false);
            z_vector.push_back(true);
        } else
        if (c == '0'){
            b_vector.push_back(false);
            x_vector.push_back(false);
            z_vector.push_back(false);
        } else
        if (c == '1'){
            b_vector.push_back(true);
            x_vector.push_back(false);
            z_vector.push_back(false);
        }
    }
}


std::string BinaryStore::as_string() {
    std::string out;
    for (int i=size-1; i >= 0; i--){
        out += char_at(i);
    }
    return out;
}

char BinaryStore::char_at(int ind) {
    auto [b, x, z] = at(ind);
    if (x) return 'x';
    if (z) return 'z';
    if (b) return '1';
    return '0';
}

bool BinaryStore::b_at(int ind) {
    auto [b, x, z] = at(ind);
    if (x || z) return false;
    return b;
}

bool BinaryStore::x_at(int ind) {
    return std::get<1>(at(ind));
}

bool BinaryStore::z_at(int ind) {
    return std::get<2>(at(ind));
}

std::tuple<bool,bool,bool> BinaryStore::at(int ind) {
    return {b_vector[ind], x_vector[ind], z_vector[ind]};
}

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
    if (vtype == "reg" || vtype == "trireg") {
        type = Type::REG;
    } else
    if (
        vtype == "wire" || vtype == "wand" || vtype == "wor" ||
        vtype == "tri" || vtype == "triand" || vtype == "trior" ||
        vtype == "tri0" || vtype == "tri1"
    ) {
        type = Type::WIRE;
    } else
    if (
        vtype == "event" || vtype == "realtime" ||
        vtype == "supply0" || vtype == "supply1" ||
        vtype == "time"
    ) {
        std::cerr << "Use of known but unsupported variable type '" << vtype << "'. Setting to wire.\n";
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

void Store::scalar_binary_change(std::string val){
    std::string id = val.substr(1);

    if (!identifier_code_to_var.count(id)){
        std::cerr << "Identifier '" << id << "' is unknown. Skipping";
        return;
    }

    int size = identifier_code_to_var[id]->size;
    BinaryStore b(size, val.substr(0,1));

    std::cout << val.substr(1) << " is " << b.as_string() << std::endl;
}

void Store::vector_binary_change(std::string val){
    auto vec_id = split_inner(val, "Binary Vector", 2);
    std::string vec = vec_id[0];
    std::string id = vec_id[1];

    if (!identifier_code_to_var.count(id)){
        std::cerr << "Identifier '" << id << "' is unknown. Skipping";
        return;
    }

    int size = identifier_code_to_var[id]->size;

    BinaryStore b(size, vec.substr(1));
    std::cout << id << " is " << b.as_string() << std::endl;
}

void Store::vector_real_change(std::string val) {
    std::cout << "real not implemented" << std::endl;
}