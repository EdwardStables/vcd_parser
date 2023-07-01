#include "main.h"
#include <iostream>
#include "antlr4-runtime.h"
#include "VCDParser.h"
#include "VCDLexer.h"
#include "listener.h"
#include <fstream>
#include <filesystem>

namespace a4 = antlr4;

std::string read_file(const std::filesystem::path path){
    std::ifstream inf{path};

    if (!inf){
        std::cerr << "Could not open file " << path << " for reading." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::stringstream buffer;
    buffer << inf.rdbuf();

    return buffer.str();
}

int main() {
    std::cout << "Read" << std::endl;
    std::string content = read_file("samples/wikipedia_example.vcd");
    a4::ANTLRInputStream* input_stream = new a4::ANTLRInputStream(content);
    std::cout << "Lex" << std::endl;
    antlrvcdp::VCDLexer* lexer = new antlrvcdp::VCDLexer(input_stream);
    a4::CommonTokenStream* tokenstream = new a4::CommonTokenStream(lexer);
    tokenstream->fill();
    std::cout << "Parse" << std::endl;
    antlrvcdp::VCDParser* parser = new antlrvcdp::VCDParser(tokenstream);

    std::cout << "Walk" << std::endl;
    a4::tree::ParseTreeWalker walker;
    VCDListener listener;
    walker.walk(&listener, parser->value_change_dump_definitions());
    
    std::cout << "Done" << std::endl;

    return 0;
}