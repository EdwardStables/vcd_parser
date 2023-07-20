#pragma once

#include <vector>
#include <string>

std::vector<std::string> split_inner(std::string s, std::string header, int expected);
std::string extract_inner(std::string full_text, std::string header);
