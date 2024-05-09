#include <string>
#include <vector>

std::vector<std::string> split(std::string input, char delimiter) {
    std::vector <std::string> output = std::vector<std::string>();

    size_t start = 0;
    size_t index = input.find(delimiter);
    if (index != -1) {
        output.push_back(input.substr(start, index - start));
        start += index;
    }
    return output;
}