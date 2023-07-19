#pragma once
#include <unordered_map>
#include <string>
#include <tuple>
#include <vector>

//TODO: probably refactor to a template?
struct BinaryStore {
    std::vector<bool> b_vector;
    std::vector<bool> x_vector;
    std::vector<bool> z_vector;
    int size;

    BinaryStore(int size, std::string bit_string);
    std::string as_string();
    char char_at(int ind);
    bool b_at(int ind);
    bool x_at(int ind);
    bool z_at(int ind);

private:
    //b, x, z
    std::tuple<bool,bool,bool> at(int ind);
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
    Var(std::string);
};

struct Store {
    Scope* top_scope=nullptr;
    Scope* current_scope=nullptr;
    std::unordered_map<std::string,Var*> identifier_code_to_var;

    Store() = default;
    void down_scope(Scope* scope);
    void up_scope();
    void add_var(Var*);
    Scope* get_top();
    void scalar_binary_change(std::string);
    void vector_binary_change(std::string val);
    void vector_real_change(std::string val);
};