#include "parser.h"
#include <iostream>
#include <assert.h>
#include "util.h"
#include "antlr4-runtime.h"
#include "vcdParser.h"
#include "vcdLexer.h"
#include "parser.h"

namespace a4 = antlr4;

Store* build_store(std::string file) {
    std::string content = read_file(file);
    a4::ANTLRInputStream* input_stream = new a4::ANTLRInputStream(content);
    antlrvcdp::vcdLexer* lexer = new antlrvcdp::vcdLexer(input_stream);
    a4::CommonTokenStream* tokenstream = new a4::CommonTokenStream(lexer);
    tokenstream->fill();

    antlrvcdp::vcdParser* parser = new antlrvcdp::vcdParser(tokenstream);

    Store* store = new Store();
    vcdListener* listener = new vcdListener(store);

    a4::tree::ParseTreeWalker walker;
    walker.walk(listener, parser->value_change_dump_definitions());
    return store;
}

vcdListener::vcdListener(Store* store)
    : store(store)
{

}

void vcdListener::enterScalar_change(antlrvcdp::vcdParser::Scalar_changeContext* ctx) {
    store->scalar_binary_change(ctx->getText());
}

void vcdListener::enterVector_change(antlrvcdp::vcdParser::Vector_changeContext* ctx) {
    std::string t = ctx->getText();
    if (t[0] == 'b' || t[0] == 'B')
        store->vector_binary_change(t);
    else if (t[0] == 'r' || t[0] == 'R')
        store->vector_real_change(t);
}

void vcdListener::enterVar(antlrvcdp::vcdParser::VarContext* ctx) {
    store->add_var(new Var(extract_inner(ctx->getText(), "$var")));
}

void vcdListener::enterScope(antlrvcdp::vcdParser::ScopeContext* ctx) {
    store->down_scope(new Scope(extract_inner(ctx->getText(), "$scope")));
}

void vcdListener::enterUpScope(antlrvcdp::vcdParser::UpScopeContext* ctx) {
    store->up_scope();
}

void vcdListener::enterTime_update(antlrvcdp::vcdParser::Time_updateContext* ctx) {
    auto s = ctx->getText();
    store->set_time(std::stoi(s.substr(1)));
}