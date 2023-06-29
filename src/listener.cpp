#include "listener.h"
#include <iostream>

void VCDListener::enterDeclaration_command(antlrvcdp::VCDParser::Declaration_commandContext* ctx){
    std::cout << ctx->Text()->getText() << std::endl;
}