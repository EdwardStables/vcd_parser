#pragma once
#include <unordered_map>
#include <string>
#include <tuple>
#include <vector>
#include <map>

struct BitVector{
    enum class Bit {
        Z, X, _1, _0
    };

private:
    const uint16_t size;
    std::vector<Bit> bits;

public:
    BitVector(uint16_t size, std::string bit_string);
    BitVector(uint16_t size, uint64_t number);
    BitVector(uint16_t size, Bit value);
    std::string as_bit_string() const;
    std::string as_hex_string() const;
    char char_at(int ind) const;
    char hex_char_at(int ind) const;
    uint16_t width() const { return size; }

    bool is_fully_x() const;

    void set(std::string bit_string);
    void set(uint64_t number);
    void set_bit(size_t ind, Bit value);

    const Bit& operator[](size_t i) const;
    const bool operator==(const BitVector& rhs) const;
};

struct Var;

struct Scope {
    enum class Type {
        BEGIN,
        FORK,
        FUNCTION,
        MODULE,
        TASK
    };
    Type type; 
    std::string name;
    Scope* parent_scope = nullptr;
    std::unordered_map<std::string,Scope*> child_scopes;
    std::unordered_map<std::string,Var*> identifier_to_var;
    Scope(std::string);
    std::string format_heirarchy(int indent=0);
};

struct Var {
    enum class Type {
        EVENT, INTEGER, PARAMETER, REAL,
        REALTIME, REG, SUPPLY0, SUPPLY1,
        TIME, TRI, TRIAND, TRIOR,
        TRIREG, TRI0, TRI1, WAND,
        WIRE, WOR
    };

    Scope* scope;
    Type type;
    std::string identifier_code;
    std::string identifier;
    bool identifier_indexed = false;
    int size; //actual width
    int msb; //variable label msb
    int lsb; //variable label lsb

    std::map<uint64_t,BitVector*> values;

    Var(std::string);
    void add_value(uint64_t time, BitVector* value);
    BitVector* value_at(uint64_t time);
    uint64_t change_before_time(uint64_t time);
    uint64_t change_after_time(uint64_t time);
    void extend_to_zero();
};

struct Store {
    Scope* top_scope=nullptr;
    Scope* current_scope=nullptr;
    std::unordered_map<std::string,Var*> identifier_code_to_var;
    uint64_t current_time = 0;

    Store() = default;
    void down_scope(Scope* scope);
    void up_scope();
    void add_var(Var*);
    Scope* get_top();  
    void set_time(uint64_t time) {current_time = time;}
    void scalar_binary_change(std::string);
    void vector_binary_change(std::string val);
    void vector_real_change(std::string val);

    BitVector* value_at(std::string identifier_code, uint64_t time);
    void extend_all_to_zero();
    int64_t get_max_time();
};

Store* build_store(std::string file);
