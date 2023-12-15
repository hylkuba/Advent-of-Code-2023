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

void printSymmetricalPyramid(const std::map<int, std::vector<TInfo>> &myMap) {
    size_t maxVectorSize = 0;
    for (const auto &pair : myMap) {
        maxVectorSize = std::max(maxVectorSize, pair.second.size());
    }
    maxVectorSize *= 8;

    for (const auto &pair : myMap) {
        size_t currSize = pair.second.size() * 8;
        size_t leadingSpaces = (maxVectorSize - currSize) / 2;
        for (size_t i = 0; i < leadingSpaces; ++i) {
            std::cout << " ";
        }

        for (const auto &info : pair.second) {
            std::cout << std::setw(8) << info;
        }
        std::cout << std::endl;
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

std::pair<int, int> predict(std::map<int, std::vector<TInfo>> &differenceHistory, int lastNumber) {
    int back = 0, frontResult = 0;
    size_t mapSize = differenceHistory.size();

    while(mapSize != 0) {
        size_t vectorSize = differenceHistory[mapSize].size();
        back += differenceHistory[mapSize][vectorSize - 1].diff;

        frontResult = differenceHistory[mapSize][0].diff - frontResult; 
        //std::cout << result << "(" << mapSize << ") + ";
        mapSize--;
    }

    return std::make_pair(frontResult, back + lastNumber);
}

std::pair<int, int> forecastedNumber(std::vector<int> &numbers) {
    // Key is line, value is vector of TInfos where each element contains information of two previous values and their difference
    std::map<int, std::vector<TInfo>> differenceHistory;

    size_t count = 0, lineCount = 1;
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
    
    while(!checkZeroes(differenceHistory[lineCount])) {
        lineCount++;
        count = 0;
        for(const auto &number : differenceHistory[lineCount - 1]) {
            if(count == 0) {
                prevNumber = number.diff;
                count++;
                continue;
            }
            differenceHistory[lineCount].push_back({prevNumber, number.diff, number.diff - prevNumber});
            prevNumber = number.diff;
        }
    }
    //printSymmetricalPyramid(differenceHistory);

    return predict(differenceHistory, numbers[numbers.size() - 1]);
}

std::pair<int, int> calculate(std::vector<std::string> &lines) {
    int sumBack = 0, sumFront = 0;
    int firstNumber = 0;
    for(const auto &line : lines) {
        std::istringstream iss(line);

        std::vector<int> numbers;
        std::string word;
        int count = 0;

        while(iss >> word) {
            if(count == 0) {
                firstNumber = stoi(word);
                count++;
            }
            numbers.push_back(stoi(word));
        }
        sumBack += forecastedNumber(numbers).second;
        sumFront += firstNumber - forecastedNumber(numbers).first;
    }
    return std::make_pair(sumFront, sumBack);
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::cout << "Sum of back is: " << calculate(lines).second << std::endl;
    std::cout << "Sum of front is: " << calculate(lines).first << std::endl;

    return 0;
}