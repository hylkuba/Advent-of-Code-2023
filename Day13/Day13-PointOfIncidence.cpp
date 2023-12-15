#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <iterator>

#define FILE "Day13-input.txt"

/**
 * @brief Reads file defined as FILE
 * 
 * @param lines vector of strings where all lines of file will be stored
 */
void readFile(std::vector<std::string> &lines) {
    std::ifstream inputFile(FILE);

    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            lines.push_back(line);
        }
        inputFile.close();
    } else {
        std::cerr << "Unable to open the file." << std::endl;
    }
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    return 0;
}