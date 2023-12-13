#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <iomanip>
#include <cstdint>

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

size_t pairsNeeded(size_t amountOfGalaxies) {
    size_t result = 0;

    for (size_t i = amountOfGalaxies - 1; i > 0; i--) {
        result += i;
    }

    return result;
}

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

size_t distanceBetween(std::pair<size_t, size_t> startGalaxy, std::pair<size_t, size_t> endGalaxy) {
    size_t startX = startGalaxy.first;
    size_t startY = startGalaxy.second;
    size_t endX = endGalaxy.first;
    size_t endY = endGalaxy.second;

    size_t diffX = startX > endX ? startX - endX : endX - startX;
    size_t diffY = startY > endY ? startY - endY : endY - startY;

    return diffX + diffY;
}

uint64_t sumOfDistances(std::vector<std::pair<size_t, size_t>> &galaxyPos) {
    std::vector<std::pair<size_t, size_t>> galaxiesToUse = galaxyPos;
    uint64_t sum = 0;

    for (auto it = galaxyPos.rbegin(); it != galaxyPos.rend(); ++it) {
        galaxiesToUse.erase(std::next(it).base());
        for(const auto &other : galaxiesToUse) {
            sum += distanceBetween(*it, other);
        }
        //std::cout << "sum: " << sum << std::endl;
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
    biggerGalaxyPos = galaxyPos;

    expandUniverse(universe, galaxyPos, uniX, uniY, 1);
    expandUniverse(universe, biggerGalaxyPos, uniX, uniY, 1000000);

    std::cout << "size of bigger: " << biggerGalaxyPos.size() << std::endl;

    //std::cout << "Sum of distances between galaxies is: " << sumOfDistances(galaxyPos) << std::endl;
    std::cout << "Sum of distances between galaxies in BIGGER universe is: " << sumOfDistances(biggerGalaxyPos) << std::endl;

    return 0;
}