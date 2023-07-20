#pragma once

#include <vector>
#include <string>
#include <filesystem>

std::string read_file(const std::filesystem::path path);
std::vector<std::string> split_inner(std::string s, std::string header, int expected);
std::string extract_inner(std::string full_text, std::string header);
