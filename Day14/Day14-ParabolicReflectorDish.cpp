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

#define SPINCOUNT 1000000000

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
    std::map<std::pair<size_t, size_t>, char> &platform,
    std::vector<std::pair<size_t, size_t>> &rocks) {

    size_t y = 1, width = 0;
    for(const auto &line : lines) {
        if(y == 1) {
            width = line.length();
        }

        for (size_t x = 0; x < line.length(); x++) {
            std::pair<size_t, size_t> currPos = std::make_pair(x + 1, y);

            platform[currPos] = line[x];
            if(line[x] == 'O') {
                rocks.push_back(currPos);
            }
        }
        y++;
    }
    return std::make_pair(width, y - 1);
}

/**
 * @brief Sorts vector from TOP to BOTTOM, LEFT to RIGHT
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool sortNorth(const std::pair<size_t, size_t>& lhs, const std::pair<size_t, size_t>& rhs) {
    if (lhs.second != rhs.second) {
        return lhs.second < rhs.second;
    }
    return lhs.first < rhs.first;
}

/**
 * @brief Sorts vector from LEFT to RIGHT, TOP to BOTTOM
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool sortWest(const std::pair<size_t, size_t>& lhs, const std::pair<size_t, size_t>& rhs) {
    if (lhs.first != rhs.first) {
        return lhs.first < rhs.first;
    }

    return lhs.second < rhs.second;
}

/**
 * @brief Sorts vector from TOP to BOTTOM, LEFT to RIGHT
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool sortSouth(const std::pair<size_t, size_t>& lhs, const std::pair<size_t, size_t>& rhs) {
    if (lhs.second != rhs.second) {
        return  rhs.second < lhs.second;
    }
    return lhs.first < rhs.first;
}

/**
 * @brief Sorts vector from LEFT to RIGHT, TOP to BOTTOM
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool sortEast(const std::pair<size_t, size_t>& lhs, const std::pair<size_t, size_t>& rhs) {
    if (lhs.first != rhs.first) {
        return rhs.first < lhs.first;
    }

    return lhs.second < rhs.second;
}

void loadRocks(std::map<std::pair<size_t, size_t>, char> &platform,
    std::vector<std::pair<size_t, size_t>> &rocks,
    std::pair<size_t, size_t> dimensions) {
    
    for (size_t y = 1; y <= dimensions.second; y++) {
        for (size_t x = 1; x <= dimensions.first; x++) {
            std::pair<size_t, size_t> currPos = std::make_pair(x, y);
            if(platform[currPos] == 'O') {
                rocks.push_back(currPos);
            }
        }
    }
}

/**
 * @brief Moves rock up, as far as it can go
 * 
 * @param platform 
 * @param currPos 
 */
std::pair<size_t, size_t> moveNorth(std::map<std::pair<size_t, size_t>, char> &platform,
    std::vector<std::pair<size_t, size_t>> &rocks,
    std::pair<size_t, size_t> startingPos) {

    std::pair<size_t, size_t> oldPos = startingPos, currPos; 
    
    for (size_t y = startingPos.second - 1; y > 0; y--){
        currPos = std::make_pair(startingPos.first, y);
        if(platform[currPos] == '.') {
            platform[currPos] = 'O';
            platform[oldPos] = '.';
        } else {
            break;
        }
        oldPos = currPos;
    }

    return oldPos;
}

/**
 * @brief Moves rock left, as far as they can go
 * 
 * @param platform 
 * @param currPos 
 */
std::pair<size_t, size_t> moveWest(std::map<std::pair<size_t, size_t>, char> &platform,
    std::vector<std::pair<size_t, size_t>> &rocks,
    std::pair<size_t, size_t> startingPos) {

    std::pair<size_t, size_t> oldPos = startingPos, currPos; 
    
    for (size_t x = startingPos.first - 1; x > 0; x--){
        currPos = std::make_pair(x, startingPos.second);
        if(platform[currPos] == '.') {
            platform[currPos] = 'O';
            platform[oldPos] = '.';
        } else {
            break;
        }
        oldPos = currPos;
    }

    return oldPos;
}

/**
 * @brief Moves rock down, as far as it can go
 * 
 * @param platform 
 * @param currPos 
 */
std::pair<size_t, size_t> moveSouth(std::map<std::pair<size_t, size_t>, char> &platform,
    std::vector<std::pair<size_t, size_t>> &rocks,
    std::pair<size_t, size_t> startingPos,
    size_t height) {

    std::pair<size_t, size_t> oldPos = startingPos, currPos; 
    
    for (size_t y = startingPos.second + 1; y <= height; y++){
        currPos = std::make_pair(startingPos.first, y);
        if(platform[currPos] == '.') {
            platform[currPos] = 'O';
            platform[oldPos] = '.';
        } else {
            break;
        }
        oldPos = currPos;
    }

    return oldPos;
}

/**
 * @brief Moves rock right, as far as it can go
 * 
 * @param platform 
 * @param currPos 
 */
std::pair<size_t, size_t> moveEast(std::map<std::pair<size_t, size_t>, char> &platform,
    std::vector<std::pair<size_t, size_t>> &rocks,
    std::pair<size_t, size_t> startingPos,
    size_t width) {

    std::pair<size_t, size_t> oldPos = startingPos, currPos; 
    
    for (size_t x = startingPos.first + 1; x <= width; x++){
        currPos = std::make_pair(x, startingPos.second);
        if(platform[currPos] == '.') {
            platform[currPos] = 'O';
            platform[oldPos] = '.';
        } else {
            break;
        }
        oldPos = currPos;
    }

    return oldPos;
}

/**
 * @brief Loops through platform, if current position is moveable rock,
 * calls moveNorth() that moves that rock as far as it can vertically go
 * 
 * @param platform 
 * @param dimensions 
 */
void tilt(std::map<std::pair<size_t, size_t>, char> &platform,
    std::pair<size_t, size_t> dimensions,
    std::vector<std::pair<size_t, size_t>> &rocks,
    size_t dir) {
    
    std::vector<std::pair<size_t, size_t>> newRocks;
    for(auto rock : rocks) {
        switch(dir) {
            case 1:
                newRocks.push_back(moveNorth(platform, rocks, rock));
                break;
            case 2:
                newRocks.push_back(moveWest(platform, rocks, rock));
                break;
            case 3:
                newRocks.push_back(moveSouth(platform, rocks, rock, dimensions.second));
                break;
            case 4:
                newRocks.push_back(moveEast(platform, rocks, rock, dimensions.first));
                break;
        }
    }
    rocks = newRocks;
}

void spinCycle(std::map<std::pair<size_t, size_t>, char> &platform,
    std::pair<size_t, size_t> dimensions,
    std::vector<std::pair<size_t, size_t>> &rocks) {

    for (size_t i = 0; i < SPINCOUNT; i++) {
        std::sort(rocks.begin(), rocks.end(), sortNorth);
        tilt(platform, dimensions, rocks, 1);

        std::sort(rocks.begin(), rocks.end(), sortWest);
        tilt(platform, dimensions, rocks, 2);
        
        std::sort(rocks.begin(), rocks.end(), sortSouth);
        tilt(platform, dimensions, rocks, 3);
        
        std::sort(rocks.begin(), rocks.end(), sortEast);
        tilt(platform, dimensions, rocks, 4);
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
    std::map<std::pair<size_t, size_t>, char> platform, spinPlatform;
    std::vector<std::pair<size_t, size_t>> moveablePlatformRocks, moveableSpinPlatformRocks;

    std::pair<size_t, size_t> dimensions = storePlatform(lines, platform, moveablePlatformRocks);

    spinPlatform = platform;

    //printMap(platform, dimensions);

    tilt(platform, dimensions, moveablePlatformRocks, 1);

    //printMap(platform, dimensions);

    std::cout << "The total load is: " << sumTotalLoad(platform, dimensions) << std::endl;

    spinCycle(spinPlatform, dimensions, moveableSpinPlatformRocks);
    std::cout << "The total load is: " << sumTotalLoad(spinPlatform, dimensions) << std::endl;
    
    return 0;
}