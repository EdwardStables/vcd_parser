#include "listener.h"
#include <iostream>

void enterValue_change_dump_definitions(antlrvcdp::VCDParser::Value_change_dump_definitionsContext* ctx){
    std::cout << "top" << std::endl;
}

void VCDListener::enterDeclaration_command(antlrvcdp::VCDParser::Declaration_commandContext* ctx){
    std::cout << ctx->Text()->getText() << std::endl;
}