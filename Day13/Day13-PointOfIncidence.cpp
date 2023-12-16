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

void printPatterns(const std::vector<std::vector<std::vector<char>>> &patterns) {
    for(const auto &pattern : patterns) {
        for(const auto &row : pattern) {
            for(const auto &column : row) {
                std::cout << column;
            }
            std::cout << std::endl;
        }
        std::cout << "-----------------------------------------------" << std::endl;
    }
}

void storeLines(std::vector<std::string> &lines, std::vector<std::vector<std::vector<char>>> &patterns) {
    std::vector<std::vector<char>> currPattern;
    for(const auto &line : lines) {
        if(line == "") {
            patterns.push_back(currPattern);
            currPattern.clear();
            continue;
        }
        std::vector<char> currRow;
        for (size_t i = 0; i < line.length(); i++) {
            currRow.push_back(line[i]);   
        }
        currPattern.push_back(currRow);
    }
    patterns.push_back(currPattern);
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    /* First vector iterates through all patterns, 2nd vector iterates through rows,
        3rd vector iterates through columns
    */
    std::vector<std::vector<std::vector<char>>> patterns;

    storeLines(lines, patterns);

    //printPatterns(patterns);

    return 0;
}