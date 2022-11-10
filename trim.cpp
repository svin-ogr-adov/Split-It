#include "trim.h"

#include <iostream>
#include <string>
#include <regex>

std::string ltrim(const std::string &s) {
    return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}

std::string rtrim(const std::string &s) {
    return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
}

std::string trim(const std::string &s) {
    return ltrim(rtrim(s));
}
