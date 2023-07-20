#include "store.h"
#include <iostream>
#include <vector>
#include <math.h>
#include "util.h"
#include <map>
#include <iterator>
#include <assert.h>

BitVector::BitVector(uint16_t size, std::string bit_string) : size(size) {
    for (int i = 0; i < size; i++){
        bits.push_back(Bit::X);
    }

    set(bit_string);
}

BitVector::BitVector(uint16_t size, Bit value) : size(size) {
    for (int i = 0; i < size; i++){
        bits.push_back(value);
    }
}

void BitVector::set(std::string bit_string){
    while (bit_string.size() < size){
        bit_string = '0' + bit_string;
    }

    for (int i = 0; i < size; i++){
        char c = bit_string[size-1-i];
        if (c == 'x' || c == 'X')
            bits[i] = Bit::X;
        else if (c == 'z' || c == 'Z')
            bits[i] = Bit::Z;
        else if (c == '0')
            bits[i] = Bit::_0;
        else if (c == '1')
            bits[i] = Bit::_1;
    }
}

BitVector::BitVector(uint16_t size, uint64_t number) : size(size) {
    for (int i = 0; i < size; i++){
        bits.push_back(Bit::X);
    }

    set(number);
}

void BitVector::set(uint64_t number) {
    uint64_t max = pow(2, size)-1;
    if (number > max){
        number = max;
    }

    unsigned long long mask = 1;
    for (int i = 0; i < size; i++){
        if (mask & number){
            bits[i] = Bit::_1;
        } else {
            bits[i] = Bit::_0;
        }

        mask <<= 1;
    }
}

void BitVector::set_bit(size_t ind, BitVector::Bit value) {
    if (ind >= size) return;

    bits[ind] = value;
}


std::string BitVector::as_string() const {
    std::string out;
    for (int i=size-1; i >= 0; i--){
        out += char_at(i);
    }
    return out;
}

char BitVector::char_at(int ind) const {
    switch(bits[ind]){
        case Bit::Z: return 'z';
        case Bit::_0: return '0';
        case Bit::_1: return '1';
        //case Bit::X
        default: return 'x';
    }
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

void Var::add_value(uint64_t time, BitVector* value) {
    values.insert({time, value});
}

BitVector* Var::value_at(uint64_t time){
    auto geq = values.lower_bound(time);

    //time is beyond the final value
    if (geq == values.end()){
        return values.rbegin()->second;//return the final value
    }

    //implies that the first value in the map is not at time zero
    if(geq == values.begin()){
        BitVector* xv = new BitVector(size, BitVector::Bit::X);
        add_value(0, xv);
        return xv;
    }

    //lower bound gives the value after, we want the value before
    return std::prev(geq)->second;
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

    Var* var = identifier_code_to_var[id];
    int size = var->size;
    BitVector* b = new BitVector(size, val.substr(0,1));

    var->add_value(current_time, b);
}

void Store::vector_binary_change(std::string val){
    auto vec_id = split_inner(val, "Binary Vector", 2);
    std::string vec = vec_id[0];
    std::string id = vec_id[1];

    if (!identifier_code_to_var.count(id)){
        std::cerr << "Identifier '" << id << "' is unknown. Skipping";
        return;
    }

    Var* var = identifier_code_to_var[id];
    int size = var->size;
    BitVector* b = new BitVector(size, vec.substr(1));
    var->add_value(current_time, b);
}

void Store::vector_real_change(std::string val) {
    std::cout << "real not implemented" << std::endl;
}

BitVector* Store::value_at(std::string identifier_code, uint64_t time) {
    if (!identifier_code_to_var.count(identifier_code)) return nullptr;

    return identifier_code_to_var[identifier_code]->value_at(time);
}