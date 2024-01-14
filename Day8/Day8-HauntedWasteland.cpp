/**
 * @author Jakub Hýl <hylkuba@gmail.com>
 * @date 8.12.2023
*/

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
            pair2 = word.substr(0, 3);
        }
        network[key] = std::make_pair(pair1, pair2);
    }
}

void getKeyByEnding(const std::map<std::string, std::pair<std::string, std::string>> &network,
    std::set<std::string> &starting, std::set<std::string> &ending) {

    for(const auto &pair : network) {
        std::string key = pair.first;
        if(key.substr(2, 1) == "A") {
            starting.insert(key);
        } else if(key.substr(2, 1) == "Z") {
            ending.insert(key);
        }
    }
}

size_t numOfStepsGhosts(const std::vector<char> &directions,
    std::map<std::string, std::pair<std::string, std::string>> &network) {
    
    size_t sum = 0;
    size_t currentIndex = 0;
    size_t directionsSize = directions.size();

    std::set<std::string> currKeys;
    std::set<std::string> ending;

    getKeyByEnding(network, currKeys, ending);

    while(true) {

        bool allEndingWithZ = true;
        for(const auto &word : currKeys) {
            if(ending.find(word) == ending.end()) {
                allEndingWithZ = false;
                break;
            }
        }
        if(allEndingWithZ) {
            break;
        }

        std::set<std::string> prevKeys = currKeys;
        currKeys.clear();

        char currentChar = directions[currentIndex];

        for(auto &word : prevKeys) {
            if(currentChar == 'L') {
                currKeys.insert(network[word].first);
            } else {
                currKeys.insert(network[word].second);
            }
        }

        currentIndex = (currentIndex + 1) % directionsSize;
        sum++;
    }

    return sum;
}

size_t numOfSteps(const std::vector<char> &directions,
    std::map<std::string, std::pair<std::string, std::string>> &network) {
    
    size_t sum = 0;
    size_t currentIndex = 0;
    size_t directionsSize = directions.size();

    std::string key = "AAA";

    while(true) {
        char currentChar = directions[currentIndex];

        if(key == "ZZZ") {
            break;
        }

        if(currentChar == 'L') {
            key = network[key].first;
        } else {
            key = network[key].second;
        }

        currentIndex = (currentIndex + 1) % directionsSize;
        sum++;
    }

    return sum;
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

    std::cout << "Steps required to reach ZZZ: " << numOfSteps(directions, network) << std::endl;
    std::cout << "Steps required by Ghosts: " << numOfStepsGhosts(directions, network) << std::endl;

    return 0;
}