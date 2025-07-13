#include "argument_parser.h"


std::map<std::string, std::string> Argument_Parser::parse_list(int argument_count, char *arguments[]) {
    std::map<std::string, std::string> my_map;

    if (argument_count == 2 && !argument_identifiers.contains(std::string(arguments[1])))
        my_map["file"] = std::string(arguments[1]);

    else {
        for (int i = 1; i < argument_count; i += 1) {
            std::string curr_argument = std::string(arguments[i]);

            if (argument_identifiers.contains(curr_argument)) {
                std::string name = argument_identifiers[curr_argument];
                std::string value = "";

                if (argument_requires_value[name]) {
                    value = arguments[i + 1];
                    i += 1; // TODO: need to change the outer loop to an iterator to make this safer!
                }
                my_map[name] = value;

                if (argument_overrides_other[name])
                    break;

            } else std::cerr << "Unknown argument: " << curr_argument << " skipping over.\n";
        }
    }
    return my_map;
}

void Argument_Parser::add_argument(std::string name, std::vector<std::string> identifiers, bool value_required, bool is_override) {

    for (std::string& identifier: identifiers) {
        argument_identifiers[identifier] = name;
    }
    argument_requires_value[name] = value_required;
    argument_overrides_other[name] = is_override;
}