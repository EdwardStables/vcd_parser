#include "util.h"
#include <iterator>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <fstream>

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

std::vector<std::string> split_inner(std::string s, std::string header, int expected) {
    //https://stackoverflow.com/a/5607650
    std::stringstream ss(s);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin, end);

    if (vstrings.size() < expected){
        std::cerr << header + " declaration is missing expected content." << std::endl;
        exit(1);
    }

    if (vstrings.size() > expected){
        std::cerr << header + " declaration has unexpected content. Ignoring extra." << std::endl;
    }

    return vstrings;
}

std::string extract_inner(std::string full_text, std::string header){
    int hsize = header.size();

    assert(full_text.substr(0, hsize) == header);
    assert(full_text.substr(full_text.size()-4, 4) == "$end");

    return full_text.substr(hsize + 1, full_text.size()-hsize-1-5);
}
