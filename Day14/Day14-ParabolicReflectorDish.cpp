#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <iomanip>

#define FILE "Day14-input.txt"

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

/**
 * @brief Prints map of platform
 * 
 * @param platform 
 * @param dimensions 
 */
void printMap(std::map<std::pair<size_t, size_t>, char> &platform
    ,std::pair<size_t, size_t> dimensions) {
    
    for (size_t y = 1; y <= dimensions.second; y++) {
        std::cout << std::setw(3)<< y << " : ";
        for (size_t x = 1; x <= dimensions.first; x++) {
            std::cout << platform[std::make_pair(x, y)];
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Stores lines read from file to appropriate platform map
 * 
 * @param lines 
 * @param platform 
 * @return std::pair<size_t, size_t>
 */
std::pair<size_t, size_t> storePlatform(const std::vector<std::string> &lines,
    std::map<std::pair<size_t, size_t>, char> &platform) {

    size_t y = 1, width = 0;
    for(const auto &line : lines) {
        if(y == 1) {
            width = line.length();
        }

        for (size_t x = 0; x < line.length(); x++) {
            platform[std::make_pair(x + 1, y)] = line[x];
        }
        y++;
    }
    return std::make_pair(width, y - 1);
}

/**
 * @brief Moves rock up, as far as it can go
 * 
 * @param platform 
 * @param currPos 
 */
void moveNorth(std::map<std::pair<size_t, size_t>, char> &platform,
    std::pair<size_t, size_t> currPos) {
    
    std::pair<size_t, size_t> oldPos = currPos; 
    for (size_t y = currPos.second - 1; y > 0; y--){
        currPos = std::make_pair(currPos.first, y);
        if(platform[currPos] == '.') {
            platform[currPos] = 'O';
            platform[oldPos] = '.';
        } else {
            break;
        }
        oldPos = currPos;
    }
}

/**
 * @brief Loops through platform, if current position is moveable rock,
 * calls moveNorth() that moves that rock as far as it can vertically go
 * 
 * @param platform 
 * @param dimensions 
 */
void tiltToNorth(std::map<std::pair<size_t, size_t>, char> &platform,
    std::pair<size_t, size_t> dimensions) {
    
    for (size_t y = 1; y <= dimensions.second; y++) {
        for (size_t x = 1; x <= dimensions.first; x++) {
            std::pair<size_t, size_t> currPos = std::make_pair(x, y);
            if(platform[currPos] == 'O') {
                moveNorth(platform, currPos);
            }
        }
    }
}

/**
 * @brief Summarizes total load of platform after all rocks were moved
 * 
 * @param platform 
 * @param dimensions 
 * @return size_t 
 */
size_t sumTotalLoad(std::map<std::pair<size_t, size_t>, char> &platform,
    std::pair<size_t, size_t> dimensions) {

    size_t sum = 0;

    for (size_t y = 1; y <= dimensions.second; y++) {
        for (size_t x = 1; x <= dimensions.first; x++) {
            if(platform[std::make_pair(x, y)] == 'O') {
                sum += 101 - y;
            }
        }
    }

    return sum;
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    /**
     * @brief Key is pair (x, y) and value is char
     * 
     */
    std::map<std::pair<size_t, size_t>, char> platform;

    std::pair<size_t, size_t> dimensions = storePlatform(lines, platform);

    //printMap(platform, dimensions);

    tiltToNorth(platform, dimensions);

    //printMap(platform, dimensions);

    std::cout << "The total load is: " << sumTotalLoad(platform, dimensions) << std::endl;
    
    return 0;
}