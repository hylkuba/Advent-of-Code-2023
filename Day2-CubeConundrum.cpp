#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>

#define URL "Day2-input.txt"
#define REDMAX  12
#define GREENMAX 13
#define BLUEMAX  14

/**
 * @brief Reads file defined as URL
 * 
 * @param lines vector of strings where all lines of file will be stored
 */
void readFile(std::vector<std::string> &lines) {
    std::ifstream inputFile(URL);
    
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
 * @brief Divides lines into appropriate format and stores that information into map
 * 
 * @param games 
 * @param lines 
 */
void store2map(std::map<int, std::set<std::pair<int, std::string>>> &games, const std::vector<std::string> lines) {
    std::string tmp = "";
    int currKey;
    for(const auto& line : lines) {
        // Start on 5th index, every line starts with "Game int", get game ID
        size_t i;
        for (i = 5; i < line.length(); i++) {
            if(line[i] == ':') {
                //std::cout << "Casting: " << tmp << std::endl;
                currKey = std::stoi(tmp);
                break;
            }
            tmp += line[i];
        }

        tmp = "";

        // Get pairs
        i += 2;
        int value;
        for (; i < line.length(); i++) {
            // If line = space, store value
            if(line[i] == ' ') {
                //std::cout << "Casting: " << tmp << std::endl;
                value = std::stoi(tmp);
                tmp = "";
                continue;
            }
            // If line = ';' save for the current key the pair of value and color into the set
            if(line[i] == ';' || line[i] == ',') {
                games[currKey].insert(std::make_pair(value, tmp));
                tmp = "";
                i++;
                continue;
            }
            tmp += line[i];
        }
        games[currKey].insert(std::make_pair(value, tmp));
        tmp = "";
    }
}

/**
 * @brief Prints values in map
 * 
 * @param games 
 */
void printMapValues(const std::map<int, std::set<std::pair<int, std::string>>>& games) {
    for (const auto& entry : games) {
        int key = entry.first;
        const std::set<std::pair<int, std::string>>& valueSet = entry.second;

        std::cout << "Key: " << key << ", Values: ";
        for (const auto& pairValue : valueSet) {
            int intValue = pairValue.first;
            const std::string& stringValue = pairValue.second;
            std::cout << "(" << intValue << ", " << stringValue << ") ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Checks if set of information for current game follows the rules of maximum values
 * 
 * @param data 
 * @return true If follows criteria
 * @return false If doesn't follow criteria
 */
bool checkPossibility(std::set<std::pair<int, std::string>> data) {
    for (const auto& pairValue : data) {
        int value = pairValue.first;
        const std::string& color = pairValue.second;

        if(color == "red" && value > REDMAX) {
            return false;
        }else if(color == "green" && value > GREENMAX) {
            return false;
        }else if(color == "blue" && value > BLUEMAX) {
            return false;
        }
    }
    return true;
}

int sumIDs(const std::map<int, std::set<std::pair<int, std::string>>> games) {
    int sum = 0;

    for (auto it = games.begin(); it != games.end(); ++it) {
        if(checkPossibility(it->second)) {
            sum += it->first;
        }
    }

    return sum;
}

int main(void) {
    std::vector<std::string> lines;
    readFile(lines);

    /*
        !KEY = game ID
        *VALUE = pair(number of cubes, color of cubes) 
    */
    std::map<int, std::set<std::pair<int, std::string>>> games;

    store2map(games, lines);

    //printMapValues(games);

    std::cout << "Sum of IDs that follow criteria is: " << sumIDs(games) << std::endl;

    return 0;
}