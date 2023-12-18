#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <iterator>

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

void storePlatform(const std::vector<std::string> &lines,
    std::map<std::pair<size_t, size_t>, int> &platform) {

    size_t y = 1;
    for(const auto &line : lines) {

        for (size_t x = 0; x < line.length(); x++) {
            int assign = 0;
            if(line[x] == 'O') {
                assign = -1;
            } else if(line[x] == '.') {
                assign = 0;
            } else if(line[x] == '#') {
                assign = 1;
            }
            platform[std::make_pair(x + 1, y)] = assign;
        }
        y++;
    }
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    /**
     * @brief Key is pair (x, y) and value is -1 for rounded rock (O), 0 for empty space (.) and 1 for cube shaped rocks (#)
     * 
     */
    std::map<std::pair<size_t, size_t>, int> platform;

    storePlatform(lines, platform);
    
    return 0;
}