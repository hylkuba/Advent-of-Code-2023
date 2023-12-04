#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <iomanip>

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
            std::cout << std::setw(2) << value << " ";
        }
        std::cout << std::endl;
    }
}

void storeData(std::vector<std::string> lines, std::map<int, std::set<int>> &winningNumbers,
    std::map<int, std::set<int>> &chosenNumbers) {

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
        if(tmp != "") {
            chosenNumbers[key].insert(stoi(tmp));
        }
    }
}

int sumNumbers(std::map<int, int> &copies, const std::map<int, std::set<int>> &winningNumbers,
    const std::map<int, std::set<int>> &chosenNumbers) {

    int sum = 0;
    for(const auto &card : winningNumbers) {
        int key = card.first;
        auto it = chosenNumbers.find(key);
        int count = 0;
        for (const auto &number : card.second) {
            if(it->second.find(number) != it->second.end()) {
                if(count == 0) {
                    count = 1;
                } else {
                    count *= 2;
                }
            }
        }
        if(count > 0) {
            sum += count;
                int m = 0;
                /*do {
                    for (int i = 1; i <= count; i++) {
                        copies[i + key] += 1;
                    }
                    m++;
                } while(m < copies[key]);*/
        }
    }

    return sum;
}

int sumCopies(const std::map<int, int> &copies) {
    int sum = 0;
    for (const auto& pair : copies) {
        sum += pair.second;
    }
    return sum;
}

void initializeCopies(std::map<int, int> &copies, int count) {
    for (int i = 1; i <= count; i++) {
        copies[i] = 0;
    }
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::map<int, std::set<int>> winningNumbers;
    std::map<int, std::set<int>> chosenNumbers;
    std::map<int, int> copies;

    storeData(lines, winningNumbers, chosenNumbers);
    initializeCopies(copies, winningNumbers.size());

    std::cout << "The sum is: " << sumNumbers(copies, winningNumbers, chosenNumbers) << std::endl;
    std::cout << "The sum of copies is " << sumCopies(copies) << std::endl;

    /*std::cout << "Printing winning numbers:" << std::endl;
    printMap(winningNumbers);
    std::cout << "Printing chosen numbers:" << std::endl;
    printMap(chosenNumbers);*/

    return 0;
}