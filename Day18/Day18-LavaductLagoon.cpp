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

void printMap(
        std::map<std::pair<int, int>, bool> &mapOfDigs,
        std::pair<std::pair<int, int>, std::pair<int, int>> dimensions) {

    for (int y = dimensions.second.first; y <= dimensions.second.second; y++) {
        for (int x = dimensions.first.first; x <= dimensions.first.second; x++) {
            if(mapOfDigs[std::make_pair(x, y)]) {
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
}

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

void changeLimit(int curr, int &max, int &min) {
    if(curr > max) {
        max = curr;
    } else if(curr < min) {
        min = curr;
    }
}

std::pair<std::pair<int, int>, std::pair<int, int>> digInMap(
        std::vector<TDig> &digs,
        std::map<std::pair<int, int>, bool> &mapOfDigs) {
    
    std::pair<int, int> currPos = std::make_pair(0, 0);
    int maxX = 0, maxY = 0, minX = 0, minY = 0;

    for(const auto &dig : digs) {
        int currX = currPos.first + dig.move.first;
        int currY = currPos.second + dig.move.second;

        // Dig on X or Y axis
        if(currX > currPos.first) {        
            for (int i = currPos.first + 1; i <= currX; i++) {
                mapOfDigs[std::make_pair(i, currY)] = true;
            }
            changeLimit(currX, maxX, minX);
        } else if(currX < currPos.first) {
            for (int i = currPos.first - 1; i >= currX; i--) {
                mapOfDigs[std::make_pair(i, currY)] = true;
            }
            changeLimit(currX, maxX, minX);
        } else if(currY > currPos.second) {
            for (int i = currPos.second + 1; i <= currY; i++) {
                mapOfDigs[std::make_pair(currX, i)] = true;
            }
            changeLimit(currY, maxY, minY);
        } else if(currY < currPos.second) {
            for (int i = currPos.second - 1; i >= currY; i--) {
                mapOfDigs[std::make_pair(currX, i)] = true;
            }
            changeLimit(currY, maxY, minY);
        }
        currPos.first = currX;
        currPos.second = currY;
    }

    return std::make_pair(std::make_pair(minX, maxX), std::make_pair(minY, maxY));
}

size_t cubicMeters(
        const std::map<std::pair<int, int>, bool> &mapOfDigs,
        std::pair<std::pair<int, int>, std::pair<int, int>> dimensions) {
    
    size_t sum = 0;

    return sum;
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::vector<TDig> digs;

    storeLines(lines, digs);

    //printDigs(digs);

    std::map<std::pair<int, int>, bool> mapOfDigs;

    std::pair<std::pair<int, int>, std::pair<int, int>> dimensions = digInMap(digs, mapOfDigs);

    printMap(mapOfDigs, dimensions);

    std::cout << "Cubic meters of lava to hold: " << cubicMeters(mapOfDigs, dimensions) << std::endl;

    return 0;
}