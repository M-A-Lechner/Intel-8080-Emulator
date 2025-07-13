#pragma once

#include <iostream>
#include <map>
#include <variant>
#include <string>
#include <vector>


struct Argument_Parser {
    std::map<std::string, std::string> argument_identifiers;
    std::map<std::string, bool> argument_requires_value;
    std::map<std::string, bool> argument_overrides_other;
    

    void add_argument(std::string name, std::vector<std::string> identifiers, bool value_required, bool is_override);

    std::map<std::string, std::string> parse_list(int argument_count, char *arguments[]);
};