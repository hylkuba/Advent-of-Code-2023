#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <iomanip>
#include <cmath>

#define FILE "Day11-input.txt"

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
 * @brief Prints whole universe to terminal
 * 
 * @param universe 
 * @param x 
 * @param y 
 */
void printUniverse(std::map<std::pair<size_t, size_t>, bool> &universe, size_t x, size_t y) {
    for (size_t i = 1; i <= y; i++) {
        std::cout << std::setw(3) << i << " : ";
        for (size_t m = 1; m <= x; m++) {
            if(universe[std::make_pair(m, i)]) {
                std::cout << "#";
            } else {
                std::cout << ".";
            }       
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Prints all galaxy positions in format (x, y)
 * 
 * @param galaxyPos 
 */
void printGalaxyPos(std::vector<std::pair<size_t, size_t>> &galaxyPos) {
    for(const auto &galaxy : galaxyPos) {
        std::cout << "(" << galaxy.first << ", " << galaxy.second << ")" << std::endl;
    }
}

/**
 * @brief Expands universe in both horizontal and vertical axis
 * 
 * @param universe 
 * @param galaxyPos 
 * @param uniX 
 * @param uniY 
 * @param expansion Size of how many times universe should be expanded
 */
void expandUniverse(std::map<std::pair<size_t, size_t>, bool> &universe, std::vector<std::pair<size_t, size_t>> &galaxyPos,
    size_t uniX, size_t uniY, size_t expansion) {

    // *Check rows first
    size_t moveY = 0;
    for (size_t y = 1; y <= uniY; y++) {
        bool galaxyFound = false;
        for (size_t x = 1; x <= uniX; x++) {
            if(universe[std::make_pair(x, y)]) {
                galaxyFound = true;
                break;
            }
        }

        if(!galaxyFound) {
            for (auto it = galaxyPos.rbegin(); it != galaxyPos.rend(); ++it) {
                if(it->second > y + moveY) {
                    size_t tmpX = it->first;
                    size_t tmpY = it->second;
                    galaxyPos.erase(std::next(it).base());
                    galaxyPos.push_back(std::make_pair(tmpX, tmpY + expansion));
                }
            }
            moveY += expansion;
        }
    }

    // *Check columns
    size_t moveX = 0;
    for (size_t x = 1; x <= uniX; x++) {
        bool galaxyFound = false;
        for (size_t y = 1; y <= uniY; y++) {
            if(universe[std::make_pair(x, y)]) {
                galaxyFound = true;
                break;
            }
        }

        if(!galaxyFound) {
            for (auto it = galaxyPos.rbegin(); it != galaxyPos.rend(); ++it) {
                if(it->first > x + moveX) {
                    size_t tmpX = it->first;
                    size_t tmpY = it->second;
                    galaxyPos.erase(std::next(it).base());
                    galaxyPos.push_back(std::make_pair(tmpX + expansion, tmpY));
                }
            }
            moveX += expansion;
        }
    }
}

/**
 * @brief Uses map of universe to store galaxy positions into a vector
 * 
 * @param universe 
 * @param galaxyPos 
 * @param uniX Width of universe
 * @param uniY Height of universe
 */
void getGalaxyPos(std::map<std::pair<size_t, size_t>, bool> &universe, std::vector<std::pair<size_t, size_t>> &galaxyPos,
    size_t uniX, size_t uniY) {

    for (size_t y = 1; y <= uniY; y++) {
        for (size_t x = 1; x <= uniX; x++) {
            if(universe[std::make_pair(x, y)]) {
                galaxyPos.push_back(std::make_pair(x, y));
            }
        }
    }
}

/**
 * @brief Stores lines loaded from file into a map of universe that holds information where galaxies are located
 * 
 * @param lines Vector of loaded lines from input file
 * @param universe Map where key is a position (x, y) and value is either TRUE: galaxy or FALSE: space
 * @return std::pair<size_t, size_t> Size of universe (x, y)
 */
std::pair<size_t, size_t> storeToMap(std::vector<std::string> &lines, std::map<std::pair<size_t, size_t>, bool> &universe) {
    size_t lineCount = 1;
    size_t uniX = 0;
    for(const auto &line : lines) {
        for (size_t i = 0; i < line.length(); i++) {
            if(line[i] == '#') {
                universe[std::make_pair(i + 1, lineCount)] = true;
            } else {
                universe[std::make_pair(i + 1, lineCount)] = false;
            }
        }
        if(lineCount == 1) {
            uniX = line.length();
        }

        lineCount++;
    }
    return std::make_pair(uniX, lineCount - 1);
}

/**
 * @brief Calculates distance between two positions (as a number of squares to cross to get to final location)
 * 
 * @param startGalaxy 
 * @param endGalaxy 
 * @return size_t Distance
 */
size_t distanceBetween(std::pair<size_t, size_t> startGalaxy, std::pair<size_t, size_t> endGalaxy) {
    size_t startX = startGalaxy.first;
    size_t startY = startGalaxy.second;
    size_t endX = endGalaxy.first;
    size_t endY = endGalaxy.second;

    size_t diffX = startX > endX ? startX - endX : endX - startX;
    size_t diffY = startY > endY ? startY - endY : endY - startY;

    return diffX + diffY;
}

/**
 * @brief Calculates the sum of distances between all pairs of galaxies excluded of duplicitiess
 * 
 * @param galaxyPos Vector containing positions of existing galaxies
 * @return size_t Sum of distances
 */
size_t sumOfDistances(std::vector<std::pair<size_t, size_t>> &galaxyPos) {
    size_t sum = 0;

    for (size_t i = 0; i < galaxyPos.size(); ++i) {
        for (size_t j = i + 1; j < galaxyPos.size(); ++j) {
            sum += distanceBetween(galaxyPos[i], galaxyPos[j]);
            /*std::cout << "Distance between: " << galaxyPos[i].first << ", " << galaxyPos[i].second
                      << " and " << galaxyPos[j].first << ", " << galaxyPos[j].second
                      << " = " << distanceBetween(galaxyPos[i], galaxyPos[j]) << std::endl;*/
        }
    }

    return sum;
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    // True if galaxy exists on index
    std::map<std::pair<size_t, size_t>, bool> universe;

    std::pair<size_t, size_t> universeSize= storeToMap(lines, universe);
    size_t uniX = universeSize.first, uniY = universeSize.second;

    std::vector<std::pair<size_t, size_t>> galaxyPos, biggerGalaxyPos;
    getGalaxyPos(universe, galaxyPos, uniX, uniY);
    getGalaxyPos(universe, biggerGalaxyPos, uniX, uniY);

    expandUniverse(universe, galaxyPos, uniX, uniY, 1);
    expandUniverse(universe, biggerGalaxyPos, uniX, uniY, 999999);

    std::cout << "Sum of distances between galaxies is: " << sumOfDistances(galaxyPos) << std::endl;
    std::cout << "Sum of distances between galaxies in BIGGER universe is: " << sumOfDistances(biggerGalaxyPos) << std::endl;

    return 0;
}