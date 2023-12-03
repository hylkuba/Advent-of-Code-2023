#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>

#define FILE "Day3-input.txt"

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

void printPartNumbers(const std::set<int> &partnumbers) {
    int cnt = 0;
    for(const auto &it : partnumbers) {
        std::cout << "Part number no: " << cnt++ << " is: " << it << std::endl;
    }
}

bool checkSymbolExistance(std::string prevLine, std::string nextLine, int startIndex, int endIndex) {
    //std::cout << "startIndex: " << startIndex << " EndIndex: " << endIndex << std::endl; 
    
    if(prevLine != "") {
        //std::cout << "prevLine: " << prevLine << std::endl;
        for (int i = startIndex; i <= endIndex; i++) {
            //std::cout << "actual Line: " << prevLine[i];
            //std::cout << std::endl;
            if(!isdigit(prevLine[i]) && prevLine[i] != '.') {
                return true;
            }
        }
    }

    if(nextLine != "") {
        //std::cout << "nextLine: " << nextLine << std::endl;
        for (int i = startIndex; i <= endIndex; i++) {
            //std::cout << "actual Line: " << nextLine[i];
            //std::cout << std::endl;
            if(!isdigit(nextLine[i]) && nextLine[i] != '.') {
                return true;
            }
        }
    }
    return false;
}

int getPartNumbers(std::set<int> &partnumbers, std::vector<std::string> lines) {
    std::string prevLine, nextLine;
    int startIndex, lineLength = lines[0].length(), sum = 0, vectorSize = lines.size();

    std::string tmp;

    for(int lineCount = 0; lineCount < vectorSize; ++lineCount) {
        tmp = "";

        std::cout << "Cnt: " << lineCount << std::endl;

        for (int i = 0; i < lineLength; i++) {

            //std::cout << "Index: " << i << " Tmp value: " << tmp << " line[i]: " << lines[lineCount][i] << std::endl;

            if(!isdigit(lines[lineCount][i]) && tmp.length() == 0) {
                continue;;
            } else if(isdigit(lines[lineCount][i])) {
                tmp += lines[lineCount][i];
            } else if(!isdigit(lines[lineCount][i]) && tmp.length() != 0) {
                // Check existance of the symbol
                if((i - tmp.length() - 1 >= 0 && lines[lineCount][i - tmp.length() - 1] != '.') || (lines[lineCount][i] != '.')) {
                    //partnumbers.insert(std::stoi(tmp));
                    sum += std::stoi(tmp);
                    std::cout << "added: " << std::stoi(tmp) << std::endl;
                } else {
                    // Assign prevLine, nextLine, startIndex, endIndex only if symbol not found on current line for efficiency
                    if(lineCount - 1 >= 0) {
                        prevLine = lines[lineCount - 1];
                    } else {
                        prevLine = "";
                    }

                    if(lineCount + 1 < vectorSize) {
                        nextLine = lines[lineCount + 1];
                    } else {
                        nextLine = "";
                    }

                    if(i - tmp.length() - 1 >= 0) {
                        startIndex = i - tmp.length() - 1;
                    } else {
                        startIndex = -1;
                    }

                    if(checkSymbolExistance(prevLine, nextLine, startIndex, i)) {
                        //partnumbers.insert(std::stoi(tmp));
                        sum += std::stoi(tmp);
                        std::cout << "added: " << std::stoi(tmp) << std::endl;
                    }
                }
                tmp = "";
            }
        }

        // TODO: If tmp has still value at the end of line
        if(tmp != "") {
            if(lineLength - tmp.length() - 1 >= 0 && lines[lineCount][lineLength - tmp.length() - 1] != '.') {
                partnumbers.insert(std::stoi(tmp));
                std::cout << "added: " << std::stoi(tmp) << std::endl;
            } else {
                if(lineCount - 1 >= 0) {
                    prevLine = lines[lineCount - 1];
                } else {
                    prevLine = "";
                }
                if(lineCount + 1 < vectorSize) {
                    nextLine = lines[lineCount + 1];
                } else {
                    nextLine = "";
                }

                if(lineLength - tmp.length() - 1 >= 0) {
                    startIndex = lineLength - tmp.length() - 1;
                } else {
                    startIndex = -1;
                }
                if(checkSymbolExistance(prevLine, nextLine, startIndex, lineLength - 1)) {
                    //partnumbers.insert(std::stoi(tmp));
                    sum += std::stoi(tmp);
                    std::cout << "added: " << std::stoi(tmp) << std::endl;
                }
            }
        }
    }
    return sum;
}

int sumPartNumbers(const std::set<int> &partnumbers) {
    int sum = 0;

    for(const auto &it : partnumbers) {
        sum += it;
    }

    return sum;
}

int main(void) {
    std::vector<std::string> lines;
    readFile(lines);

    std::set<int> partNumbers;

    //getPartNumbers(partNumbers, lines);

    //printPartNumbers(partNumbers);

    std::cout << "Sum of part Numbers is: " << /*sumPartNumbers(partNumbers)*/ getPartNumbers(partNumbers, lines) << std::endl;

    return 0;
}