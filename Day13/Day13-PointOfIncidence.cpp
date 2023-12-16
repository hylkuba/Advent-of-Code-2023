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

bool checkHorizontalTillEnd(const std::vector<std::vector<char>> &pattern,
    int start, int end, int width) {
    
    int currL = start;
    int currR = start + 1;
    do {
        ///std::cout << "CurrL: " << currL << " currR: " << currR << std::endl;
        for(int i = 0; i < width; i++) {
            if(pattern[currL][i] != pattern[currR][i]) {
                return false;
            }
        }
        currL--;
        currR++;
    } while(currL >= 0 && currR <= end);

    return true;
}

size_t horizontalMirror(const std::vector<std::vector<char>> &pattern) {
    size_t result = 0;
    int width = pattern[0].size(), height = pattern.size();

    for (int i = 0; i < height - 1; i++) {
        //std::cout << "Line: " << i << std::endl;
        if(checkHorizontalTillEnd(pattern, i, height - 1, width)) {
            result = i + 1;
        }
    }

    return 100 * result;
}

bool checkVerticalTillEnd(const std::vector<std::vector<char>> &pattern,
    int start, int end, int height) {
    
    int currL = start;
    int currR = start + 1;
    do {
        for(int i = 0; i < height; i++) {
            if(pattern[i][currL] != pattern[i][currR]) {
                return false;
            }
        }
        currL--;
        currR++;
    } while(currL >= 0 && currR <= end);

    return true;
}

size_t verticalMirror(const std::vector<std::vector<char>> &pattern) {
    size_t result = 0;
    int width = pattern[0].size(), height = pattern.size();

    for (int i = 0; i < width - 1; i++) {
        if(checkVerticalTillEnd(pattern, i, width - 1, height)) {
            result = i + 1;
        }
    }
    
    return result;
}

size_t mirror(const std::vector<std::vector<char>> &pattern) {
    size_t result = verticalMirror(pattern);
    
    if(result > 0) return result;

    return horizontalMirror(pattern);
}

size_t summarize(const std::vector<std::vector<std::vector<char>>> &patterns) {
    size_t sum = 0;

    for(const auto &pattern : patterns) {
        sum += mirror(pattern);
    }

    return sum;
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);    

    /* First vector iterates through all patterns, 2nd vector iterates through rows,
        3rd vector iterates through columns
    */
    std::vector<std::vector<std::vector<char>>> patterns;

    storeLines(lines, patterns);

    std::cout << "Summary is: " << summarize(patterns) << std::endl;

    //printPatterns(patterns);

    return 0;
}