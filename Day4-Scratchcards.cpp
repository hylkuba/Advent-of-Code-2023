#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>

#define FILE "Day4-input.txt"

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

void printMap(const std::map<int, std::set<int>>& myMap) {
    for (const auto& pair : myMap) {
        std::cout << "Key: " << pair.first << ", Values: ";
        for (const auto& value : pair.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

void storeData(std::vector<std::string> lines, std::map<int, std::set<int>> &winningNumbers, std::map<int, std::set<int>> &chosenNumbers) {
    for (size_t i = 0; i < lines.size(); i++) {
        // skip first 5 characters (Card )
        std::string tmp = "";
        int key = 0;

        bool winning = true;
        for (size_t m = 5; m < lines[i].length(); m++) {
            if(isdigit(lines[i][m])) {
                tmp += lines[i][m];
            } else if(lines[i][m] == ':') {
                key = stoi(tmp);
                m++;
                tmp = "";
            } else if(lines[i][m] == ' ') {
                if(tmp == "") {
                    continue;
                }

                if(winning) {
                    winningNumbers[key].insert(stoi(tmp));
                } else {
                    chosenNumbers[key].insert(stoi(tmp));
                }
                tmp = "";
            } else if(lines[i][m] == '|') {
                winning = false;
                m++;
            }
        }
    }
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::map<int, std::set<int>> winningNumbers;
    std::map<int, std::set<int>> chosenNumbers;

    storeData(lines, winningNumbers, chosenNumbers);

    std::cout << "Printing winning numbers:" << std::endl;
    printMap(winningNumbers);
    std::cout << "Printing chosen numbers:" << std::endl;
    printMap(chosenNumbers);

    return 0;
}