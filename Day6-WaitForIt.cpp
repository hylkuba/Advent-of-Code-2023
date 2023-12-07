#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <iomanip>
#include <limits>

#define FILE "Day6-input.txt"

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

void printMap(const std::map<int, std::pair<int, int>> &myMap) {
    for(const auto &element : myMap) {
        std::cout << "Key: " << element.first << " => (time: " <<  element.second.first <<
        " | distance: " << element.second.second << ")" << std::endl;
    }
}

void storeData(const std::vector<std::string> &lines, std::map<int, std::pair<int, int>> &record) {
    for (const auto &line : lines) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;
        int count = 0;
        if(word == "Time:") {
            while(iss >> word) {
                record[count] = std::make_pair(stoi(word), 0); 
                count++;
            }
        } else if(word == "Distance:") {
            while(iss >> word) {
                record[count] = std::make_pair(record[count].first, stoi(word)); 
                count++;
            }
        }
    }
}

int calculate(int time, int recordToBeat) {
    int result = 0;

    // i = button hold time
    for (int i = 1; i < time; i++) {
        if(i * (time - i) > recordToBeat) {
            result++;
        }
    }

    return result;
}

int beatRecord(const std::map<int, std::pair<int, int>> &record) {
    int sum = 1;
    for(const auto &element : record) {
        sum *= calculate(element.second.first, element.second. second);
    }
    return sum;
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::map<int, std::pair<int, int>> record;

    storeData(lines, record);

    //printMap(record);

    std::cout << "Number of ways to beat Record: " << beatRecord(record) << std::endl;
}