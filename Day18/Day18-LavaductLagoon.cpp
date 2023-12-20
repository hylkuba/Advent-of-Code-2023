#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

#define FILE "Day18-input.txt"

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

struct TDig{
    std::pair<int, int> move;
    std::string color;
};

void printDigs(const std::vector<TDig> &digs) {
    for(const auto &dig : digs) {
        if(dig.move.first > 0) {
            std::cout << "R " << dig.move.first;
        } else if(dig.move.first < 0) {
            std::cout << "L " << dig.move.first;
        } else if(dig.move.second > 0) {
            std::cout << "U " << dig.move.second;
        } else if(dig.move.second < 0) {
            std::cout << "D " << dig.move.second;
        }
        std::cout << " color: " << dig.color << std::endl;
    }
}

void storeLines(
        std::vector<std::string> &lines,
        std::vector<TDig> &digs) {
    
    for(const auto &line : lines) {
        char firstChar;
        int number;

        std::istringstream iss(line);
        iss >> firstChar >> number;

        std::string restOfString = line.substr(line.find('(', 0));

        size_t lastSpace = restOfString.find_last_of(' ');
        if (lastSpace != std::string::npos) {
            restOfString = restOfString.substr(0, lastSpace) + restOfString.substr(lastSpace + 1, restOfString.length() - lastSpace - 2);
        }

        TDig curr;
        curr.color = restOfString;
        switch (firstChar)
        {
        case 'R':
            curr.move = std::make_pair(number, 0);
            break;
        case 'L':
            curr.move = std::make_pair(-number, 0);
            break;
        case 'U':
            curr.move = std::make_pair(0, number);
            break;
        case 'D':
            curr.move = std::make_pair(0, -number);
            break;
        }
        digs.push_back(curr);
    }
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::vector<TDig> digs;

    storeLines(lines, digs);

    printDigs(digs);

    std::pair<int, int> dimensions;

    return 0;
}