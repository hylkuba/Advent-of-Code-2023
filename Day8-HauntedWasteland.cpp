#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <iomanip>

#define FILE "Day8-input.txt"

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

void printMap(const std::map<std::string, std::pair<std::string, std::string>> &myMap) {
    for(const auto &pair : myMap) {
        std::cout << pair.first << " = (" << pair.second.first << " ," << pair.second.second << ")" << std::endl;
    }
}

void storeData(std::vector<std::string> &lines, std::vector<char> &directions,
    std::map<std::string, std::pair<std::string, std::string>> &network) {

    int lineCnt = 0;
    for(const auto &line : lines) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;

        if(lineCnt == 0) {
            for (size_t i = 0; i < word.length(); i++) {
                directions.push_back(word[i]);
            }
            lineCnt++;
            continue;
        } else if(lineCnt == 1) {
            lineCnt++;
            continue;
        }

        std::string key = word, pair1, pair2;
        // '='
        iss >> word;
        // '(key'
        iss >> word;
        if (0 != std::string::npos && 4 != std::string::npos) {
            pair1 = word.substr(1, 3);
        }
        iss >> word;
        if (0 != std::string::npos && 3 != std::string::npos) {
            pair2 = word.substr(0, 2);
        }
        network[key] = std::make_pair(pair1, pair2);
    }
}

int main(void) {
    std::vector<std::string> lines;

    std::vector<char> directions;
    /**
     * @brief key is a string, value is pair of (left, right)
     * 
     */
    std::map<std::string, std::pair<std::string, std::string>> network;
    
    readFile(lines);

    storeData(lines, directions, network);

    /*std::cout << "Printing network map:" << std::endl;
    printMap(network);
    
    std::cout << "Printing vector:" << std::endl;
    for(const auto &c : directions) {
        std::cout << c << ", ";
    }*/

    return 0;
}