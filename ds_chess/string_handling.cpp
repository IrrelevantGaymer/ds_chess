//
// Created by river on 5/13/24.
//

#include "string_handling.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

std::vector<std::string> StringHandling::split(std::string input, char delimiter) {
    std::vector <std::string> output = std::vector<std::string>();

    std::string string = std::move(input);
    size_t index = string.find(delimiter);
    while (index != -1) {
        output.push_back(string.substr(0, index));
        string = string.substr(index + 1);
        index = string.find(delimiter);
    }
    if (!string.empty()) {
        output.push_back(string);
    }
    return output;
}