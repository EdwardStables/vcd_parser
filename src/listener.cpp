#include "listener.h"
#include <iostream>

void VCDListener::enterValue_change_dump_definitions(antlrvcdp::VCDParser::Value_change_dump_definitionsContext* ctx){
    std::cout << ctx->getText() << std::endl;
}

void VCDListener::enterDeclaration_command(antlrvcdp::VCDParser::Declaration_commandContext* ctx){
    std::cout << ctx->Text()->getText() << std::endl;
}