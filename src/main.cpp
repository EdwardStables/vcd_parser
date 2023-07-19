#include "main.h"
#include <iostream>
#include "antlr4-runtime.h"
#include "vcdParser.h"
#include "vcdLexer.h"
#include "listener.h"
#include <fstream>
#include <filesystem>
#include "store.h"

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
    std::string content = read_file("../samples/spec_example.vcd");
    a4::ANTLRInputStream* input_stream = new a4::ANTLRInputStream(content);
    antlrvcdp::vcdLexer* lexer = new antlrvcdp::vcdLexer(input_stream);
    a4::CommonTokenStream* tokenstream = new a4::CommonTokenStream(lexer);
    tokenstream->fill();

    antlrvcdp::vcdParser* parser = new antlrvcdp::vcdParser(tokenstream);

    Store* store = new Store();
    vcdListener* listener = new vcdListener(store);

    a4::tree::ParseTreeWalker walker;
    walker.walk(listener, parser->value_change_dump_definitions());

    std::cout << store->get_top()->format_heirarchy() << std::endl;
    return 0;
}