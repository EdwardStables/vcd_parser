#include "listener.h"
#include <iostream>
#include <assert.h>

std::string extract_inner(std::string full_text, std::string header){
    int hsize = header.size();

    assert(full_text.substr(0, hsize) == header);
    assert(full_text.substr(full_text.size()-4, 4) == "$end");

    return full_text.substr(hsize + 1, full_text.size()-hsize-1-5);
}

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
void vcdListener::enterVar(antlrvcdp::vcdParser::VarContext* ctx) {
    store->add_var(new Var(extract_inner(ctx->getText(), "$var")));
}

void vcdListener::enterScalar_change(antlrvcdp::vcdParser::Scalar_changeContext* ctx) {
}

void vcdListener::enterScope(antlrvcdp::vcdParser::ScopeContext* ctx) {
    store->down_scope(new Scope(extract_inner(ctx->getText(), "$scope")));
}

void vcdListener::enterUpScope(antlrvcdp::vcdParser::UpScopeContext* ctx) {
    store->up_scope();
}
