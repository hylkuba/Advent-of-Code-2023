#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iterator>

#define FILE "Day12-input.txt"

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

void printRows(const std::vector<std::pair<std::string, std::vector<int>>> &rows) {
    for(const auto &row : rows) {
        std::cout << row.first << " ";
        for(const auto &num : row.second) {
            std::cout << num << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int countArrangements(const std::string &row, const std::vector<int> &damagedSprings) {
    int sum = 0;

    return sum;
}

void storeLines(std::vector<std::pair<std::string, std::vector<int>>> &rows,
    const std::vector<std::string> &lines) {
    
    std::string word = "", partString = "";
    std::vector<int> numVector;
    bool num = false;
    
    for(const auto &line : lines) {
        for (size_t i = 0; i < line.length(); i++) {
            if(line[i] == ' ') {
                num = true;
                partString = word;
                word = "";
                continue;
            }

            if(!num) {
                word += line[i];
            } else {
                if(line[i] == ',') {
                    numVector.push_back(stoi(word));
                    word = "";
                } else {
                    word += line[i];
                }
            }
        }
        numVector.push_back(stoi(word));
        rows.push_back(std::make_pair(partString, numVector));
        numVector.clear();
        word = "";
    }
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::vector<std::pair<std::string, std::vector<int>>> rows;
    storeLines(rows, lines);

    int totalArrangements = 0;
    //printRows(rows);

    for (const auto &row : rows) {
        totalArrangements += countArrangements(row.first, row.second);
    }

    std::cout << "Total arrangements: " << totalArrangements << std::endl;

    return 0;
}
