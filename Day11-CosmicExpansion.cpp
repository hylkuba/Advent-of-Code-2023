#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <iomanip>

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

void expandColumn(std::map<std::pair<size_t, size_t>, bool> &universe, size_t currX, size_t uniX, size_t uniY) {
    for (size_t x = uniX; x > currX; x--) {
        for (size_t y = 1; y <= uniY; y++) {
            universe[std::make_pair(x, y)] = universe[std::make_pair(x - 1, y)];
        }
    }
}

void expandRow(std::map<std::pair<size_t, size_t>, bool> &universe, size_t currY, size_t uniX, size_t uniY) {
    for (size_t y = uniY; y > currY; y--) {
        for (size_t x = 1; x <= uniX; x++) {
            universe[std::make_pair(x, y)] = universe[std::make_pair(x, y - 1)];
        }
    }
}

void expandUniverse(std::map<std::pair<size_t, size_t>, bool> &universe, size_t &uniX, size_t &uniY) {
    // *Check rows first
    for (size_t y = 1; y <= uniY; y++) {
        bool galaxyFound = false;
        for (size_t x = 1; x <= uniX; x++) {
            if(universe[std::make_pair(x, y)]) {
                galaxyFound = true;
                break;
            }
        }

        if(!galaxyFound) {
            //std::cout << "Expaned on line: " << y << std::endl;
            uniY++;
            expandRow(universe, y, uniX, uniY);
            y++;
        }
    }

    // *Check columns
    for (size_t x = 1; x <= uniX; x++) {
        bool galaxyFound = false;
        for (size_t y = 1; y <= uniY; y++) {
            if(universe[std::make_pair(x, y)]) {
                galaxyFound = true;
                break;
            }
        }

        if(!galaxyFound) {
            //std::cout << "Expaned on column: " << x << std::endl;
            uniX++;
            expandColumn(universe, x, uniX, uniY);
            x++;
        }
    }
}

void getGalaxyPos(std::map<std::pair<size_t, size_t>, bool> &universe, std::set<std::pair<size_t, size_t>> &galaxyPos,
    size_t uniX, size_t uniY) {

    for (size_t y = 1; y <= uniY; y++) {
        for (size_t x = 1; x <= uniX; x++) {
            if(universe[std::make_pair(x, y)]) {
                galaxyPos.insert(std::make_pair(x, y));
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

size_t sumOfDistances(std::set<std::pair<size_t, size_t>> &galaxyPos, size_t uniX, size_t uniY) {
    std::set<std::pair<size_t, size_t>> galaxiesToUse = galaxyPos;
    size_t sum = 0;

    for(const auto &galaxy : galaxyPos) {
        galaxiesToUse.erase(galaxy);

        for(const auto &other : galaxiesToUse) {
            sum += distanceBetween(galaxy, other);
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

    expandUniverse(universe, uniX, uniY);

    std::set<std::pair<size_t, size_t>> galaxyPos;
    getGalaxyPos(universe, galaxyPos, uniX, uniY);

    std::cout << "Pairs needed: " << pairsNeeded(galaxyPos.size()) << std::endl;

    std::cout << "Sum of distances between galaxies is: " << sumOfDistances(galaxyPos, uniX, uniY) << std::endl;

    return 0;
}