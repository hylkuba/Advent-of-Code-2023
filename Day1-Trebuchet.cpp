#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

#define URL "Day1-input.txt"

void readLines(std::vector<std::string> &lines) {
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

void checkIfWordInMap(std::map<std::string, int> &words, std::string currWord, int &digitsFound, int &num1, int &num2) {
    int digit = words[currWord];
    if (digitsFound == 0) {
        num1 = digit;
        digitsFound++;
    } else if (digitsFound > 0) {
        num2 = digit;
        digitsFound++;
    }
}

std::pair<int, int> valueOnLine(std::string line) {
    int num1digit, num2digit;
    int num1total, num2total;
    int digitsFound = 0;
    int digitsFoundTotal = 0;

    std::pair<int, int> result;

    std::map<std::string, int> words = {
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4},
        {"five", 5},
        {"six", 6},
        {"seven", 7},
        {"eight", 8},
        {"nine", 9}
    };

    std::set<std::string> digitWords = {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };


    std::string currWord = "";
    size_t i = 0;

    while(i < line.size()) {
        if(isdigit(line[i])) {
            if (digitsFoundTotal == 0) {
                num1total = line[i] - '0';
                digitsFoundTotal++;
            } else if (digitsFoundTotal > 0) {
                num2total = line[i] - '0';
                digitsFoundTotal++;
            }

            if (digitsFound == 0) {
                num1digit = line[i] - '0';
                digitsFound++;
            } else if (digitsFound > 0) {
                num2digit = line[i] - '0';
                digitsFound++;
            }
            
            currWord = "";
            i++;
            continue;
        }
        
        currWord += line[i];
        for(const std::string word : digitWords) {
            if(currWord.find(word) != std::string::npos) {
                checkIfWordInMap(words, word, digitsFoundTotal, num1total, num2total);
                currWord = "";
                i -= word.size();
                i++;
                break;
            }
        }
        i++;
    }

    if(digitsFoundTotal == 1) {
        num2total = num1total;
    }

    if(digitsFound == 1) {
        num2digit = num1digit;
    }

    result.first = num1digit * 10 + num2digit;
    result.second = num1total * 10 + num2total;

    return result;
}

int main(void) { 
    std::vector<std::string> lines;
    
    readLines(lines);

    int sumDigits = 0, sumTotal = 0;
    int count = 1;

    for(const auto &line : lines) {
        std::pair<int, int> val = valueOnLine(line);
        sumDigits += val.first;
        sumTotal += val.second;
        //std::cout << "count: " << count++ << " Val of Digits: " << val.first << " Val of all: "<< val.second << std::endl;
    }

    std::cout << "Total value of digits only is: " << sumDigits << std::endl;
    std::cout << "Total value of all is: " << sumTotal << std::endl;

    return 0;
}