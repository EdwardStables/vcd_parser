#include "listener.h"
#include <iostream>
#include <assert.h>
#include "util.h"

vcdListener::vcdListener(Store* store)
    : store(store)
{

}


//void vcdListener::enterValue_change_dump_definitions(antlrvcdp::vcdParser::Value_change_dump_definitionsContext* ctx){
//}
//
//void vcdListener::enterDeclaration_command(antlrvcdp::vcdParser::Declaration_commandContext* ctx){
//}
//
//void vcdListener::enterSimulation_command(antlrvcdp::vcdParser::Simulation_commandContext* ctx){
//}
//
//void vcdListener::enterSimulation_directive(antlrvcdp::vcdParser::Simulation_directiveContext* ctx){
//}
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