#include "main.h"
#include <iostream>
#include "antlr4-runtime.h"
#include "VCDParser.h"
#include "VCDLexer.h"
#include "listener.h"

namespace a4 = antlr4;

int main() {
    a4::ANTLRInputStream* input_stream = new a4::ANTLRInputStream("");
    antlrvcdp::VCDLexer* lexer = new antlrvcdp::VCDLexer(input_stream);
    a4::CommonTokenStream* tokenstream = new a4::CommonTokenStream(lexer);
    tokenstream->fill();
    antlrvcdp::VCDParser* parser = new antlrvcdp::VCDParser(tokenstream);

    a4::tree::ParseTreeWalker walker;
    VCDListener listener;
    walker.walk(&listener, parser->value_change_dump_definitions());
    

}