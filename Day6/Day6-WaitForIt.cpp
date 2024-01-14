/**
 * @author Jakub Hýl <hylkuba@gmail.com>
 * @date 6.12.2023
*/

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

void printMap(const std::map<size_t, std::pair<size_t, size_t>> &myMap) {
    for(const auto &element : myMap) {
        std::cout << "Key: " << element.first << " => (time: " <<  element.second.first <<
        " | distance: " << element.second.second << ")" << std::endl;
    }
}

void storeData(const std::vector<std::string> &lines, std::map<size_t, std::pair<size_t, size_t>> &record, size_t &totalTime, size_t &totalDistance) {
    for (const auto &line : lines) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;
        size_t count = 0;
        if(word == "Time:") {
            std::string number = "";
            while(iss >> word) {
                record[count] = std::make_pair(stoul(word), 0); 
                count++;
                number += word;
            }
            totalTime = stoull(number);
        } else if(word == "Distance:") {
            std::string number = "";
            while(iss >> word) {
                record[count] = std::make_pair(record[count].first, stoul(word)); 
                count++;
                number += word;
            }
            totalDistance = stoull(number);
        }
    }
}

size_t calculate(size_t time, size_t recordToBeat) {
    size_t result = 0;

    // i = button hold time
    for (size_t i = 1; i < time; i++) {
        if(i * (time - i) > recordToBeat) {
            result++;
        }
    }

    return result;
}

size_t beatRecord(const std::map<size_t, std::pair<size_t, size_t>> &record) {
    size_t sum = 1;
    for(const auto &element : record) {
        sum *= calculate(element.second.first, element.second. second);
    }
    return sum;
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::map<size_t, std::pair<size_t, size_t>> record;

    size_t totalTime, totalDistance;
    storeData(lines, record, totalTime, totalDistance);

    std::cout << "Number of ways to beat Record: " << beatRecord(record) << std::endl;
    std::cout << "Number of ways to beat Record of complete time: " << calculate(totalTime, totalDistance) << std::endl;
}