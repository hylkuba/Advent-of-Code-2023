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
    if (words.find(currWord) != words.end()) {
        int digit = words[currWord];
        if (digitsFound == 0) {
            num1 = digit;
            digitsFound++;
        } else if (digitsFound > 0) {
            num2 = digit;
            digitsFound++;
        }
    }
}

int valueOnLine(std::string line) {
    int num1, num2;
    int digitsFound = 0;

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

    std::string currWord;

    for(size_t i = 0; i < line.size(); i++) {
        if (isdigit(line[i])) {
            if (digitsFound == 0) {
                num1 = line[i] - '0';
                digitsFound++;
            } else if (digitsFound > 0) {
                num2 = line[i] - '0';
                digitsFound++;
            }
        } else if (isalpha(line[i])) {
            // ONE
            if(line[i] == 'o') {
                if(i + 2 < line.size() && line[i+1] == 'n' && line[i+2] == 'e') {
                    currWord = "one";
                    checkIfWordInMap(words, currWord, digitsFound, num1, num2);
                }
                i+=2;
                continue;
            }

            if(line[i] == 't') {
                // TWO
                if(i + 2 < line.size() && line[i+1] == 'w' && line[i+2] == 'o') {
                    currWord = "two";
                    checkIfWordInMap(words, currWord, digitsFound, num1, num2);
                    i+=2;
                    continue;
                }
                // THREE
                if(i + 4 < line.size() && line[i+1] == 'h' && line[i+2] == 'r' && line[i+3] == 'e' && line[i+4] == 'e') {
                    currWord = "three";
                    checkIfWordInMap(words, currWord, digitsFound, num1, num2);
                    i+=4;
                    continue;
                }
            }

            if (line[i] == 'f') {
                // FOUR
                if(i + 3 < line.size() && line[i+1] == 'o' && line[i+2] == 'u' && line[i+3] == 'r') {
                    currWord = "four";
                    checkIfWordInMap(words, currWord, digitsFound, num1, num2);
                    i+=3;
                    continue;
                }
                // FIVE
                if(i + 3 < line.size() && line[i+1] == 'i' && line[i+2] == 'v' && line[i+3] == 'e') {
                    currWord = "five";
                    checkIfWordInMap(words, currWord, digitsFound, num1, num2);
                    i+=3;
                    continue;
                }
            }

            if(line[i] == 's') {
                // SIX
                if(i + 2 < line.size() && line[i+1] == 'i' && line[i+2] == 'x') {
                    currWord = "six";
                    checkIfWordInMap(words, currWord, digitsFound, num1, num2);
                    i+=2;
                    continue;
                }
                // SEVEN
                if(i + 4 < line.size() && line[i+1] == 'e' && line[i+2] == 'v' && line[i+3] == 'e' && line[i+4] == 'n') {
                    currWord = "seven";
                    checkIfWordInMap(words, currWord, digitsFound, num1, num2);
                    i+=4;
                    continue;
                }
            }
            
            // EIGHT
            if(line[i] == 'e') {
                if(i + 4 < line.size() && line[i+1] == 'i' && line[i+2] == 'g' && line[i+3] == 'h' && line[i+4] == 't') {
                    currWord = "eight";
                    checkIfWordInMap(words, currWord, digitsFound, num1, num2);
                    i+=4;
                    continue;
                }
            }

            // NINE
            if(line[i] == 'n') {
                if(i + 3 < line.size() && line[i+1] == 'i' && line[i+2] == 'n' && line[i+3] == 'e') {
                    currWord = "nine";
                    checkIfWordInMap(words, currWord, digitsFound, num1, num2);
                    i+=3;
                    continue;
                }
            }
        }
    }

    if(digitsFound == 1) {
        num2 = num1;
    }

    //std::cout << "num1: " << num1 << " num2: " << num2 << std::endl;
    return num1*10 + num2;
}


int main(void) { 
    std::vector<std::string> lines;
    
    readLines(lines);

    int sum = 0;
    int count = 1;
    int val = 0;
    for(const auto &line : lines) {
        val = valueOnLine(line);
        sum += val;
        std::cout << "count: " << count++ << " Val: " << val << std::endl;

        //if(count == 5) break; 
    }

    std::cout << "Total value is: " << sum << std::endl;

    return 0;
}