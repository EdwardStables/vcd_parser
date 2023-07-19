#include "listener.h"
#include <iostream>
#include <assert.h>

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
}

void vcdListener::enterScope(antlrvcdp::vcdParser::ScopeContext* ctx) {
    std::string str = ctx->getText();
    assert(str.substr(0, 7) == "$scope ");
    assert(str.substr(str.size()-4, 4) == "$end");

    Scope* s = new Scope(str.substr(7, str.size()-7-5));
    store->down_scope(s);
}

void vcdListener::enterUpScope(antlrvcdp::vcdParser::UpScopeContext* ctx) {
    store->up_scope();
}
