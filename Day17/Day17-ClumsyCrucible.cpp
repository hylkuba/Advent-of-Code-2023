#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>

#define FILE "Day17-input.txt"

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

void printBlocks(
        std::map<std::pair<int, int>, int> &blocks,
        std::pair<int, int> dimensions) {
    
    for (int y = 1; y <= dimensions.second; y++) {
        for (int x = 1; x < dimensions.first; x++) {
            std::cout << blocks[std::make_pair(x, y)];
        }
        std::cout << std::endl;
    }
}

std::pair<int, int> storeBlocks(
        std::vector<std::string> &lines,
        std::map<std::pair<int, int>, int> &blocks) {
    
    int width = 0, y = 1;
    for(const auto &line : lines) {
        if(y == 1) {
            width = line.length();
        }

        for (size_t x = 1; x <= line.length(); x++) {
            blocks[std::make_pair(x, y)] = line[x] - 48;
        }
        y++;
    }

    return std::make_pair(width, y - 1);
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::map<std::pair<int, int>, int> blocks;

    std::pair<int, int> dimensions = storeBlocks(lines, blocks);

    printBlocks(blocks, dimensions);

    return 0;
}