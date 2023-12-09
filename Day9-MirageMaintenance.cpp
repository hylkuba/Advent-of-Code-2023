#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <iomanip>

#define FILE "Day9-input.txt"

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

struct TInfo {
    int prev1;
    int prev2;
    int diff;

    friend std::ostream& operator<<(std::ostream& os,
        const TInfo& other) {
        
        os << other.diff;
        return os;
    }
};

void printMap(const std::map<int, std::vector<TInfo>> &myMap) {
    for(const auto &pair : myMap) {
        std::cout << "Line: " << pair.first << std::endl;
        for(const auto &info : pair.second) {
            std::cout << info << ", ";
        }
        std::cout << ")" << std::endl;
    }
}

bool checkZeroes(const std::vector<TInfo> &myVector) {
    for(const auto &element : myVector) {
        if(element.diff != 0) {
            return false;
        }
    }
    return true;
}

int forecastedNumber(std::vector<int> &numbers) {
    // Key is line, value is vector of TInfos where each element contains information of two previous values and their difference
    std::map<int, std::vector<TInfo>> differenceHistory;

    size_t count = 0, lineCount = 1;
    int prediction = 0;
    int prevNumber;
    for(const auto &number : numbers) {
        if(count == 0) {
            prevNumber = number;
            count++;
            continue;
        }
        differenceHistory[lineCount].push_back({prevNumber, number, number - prevNumber});
        prevNumber = number;
    }
    printMap(differenceHistory);


    return prediction;
}

int calculate(std::vector<std::string> &lines) {

    int sum = 0;
    for(const auto &line : lines) {
        std::istringstream iss(line);

        std::vector<int> numbers;
        std::string word;
        while(iss >> word) {
            //std::cout << word << std::endl;
            numbers.push_back(stoi(word));
        }
        sum += forecastedNumber(numbers);
    }
    return sum;
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::cout << "Sum of historeis is: " << calculate(lines) << std::endl;

    return 0;
}