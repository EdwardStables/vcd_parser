#include "listener.h"
#include <iostream>

void vcdListener::enterValue_change_dump_definitions(antlrvcdp::vcdParser::Value_change_dump_definitionsContext* ctx){
}

void vcdListener::enterDeclaration_command(antlrvcdp::vcdParser::Declaration_commandContext* ctx){
    std::cout << "command" << std::endl;
    std::cout << ctx->getText() << std::endl;
}

void vcdListener::enterSimulation_command(antlrvcdp::vcdParser::Simulation_commandContext* ctx){
    std::cout << "simulation" << std::endl;
    std::cout << ctx->getText() << std::endl;
}

void vcdListener::enterSimulation_directive(antlrvcdp::vcdParser::Simulation_directiveContext* ctx){
    std::cout << "directive" << std::endl;
    std::cout << ctx->simulation_keyword()->getText() << std::endl;
    for (auto v : ctx->value_change()){
        std::cout << v->getText() << std::endl;
    }
}