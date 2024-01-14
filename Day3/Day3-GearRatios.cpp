/**
 * @author Jakub Hýl <hylkuba@gmail.com>
 * @date 3.12.2023
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>

#define FILE "Day3-input.txt"

/**
 * @brief Struct for use of storing information of position of specific numbers in text
 * 
 */
struct TPos{
    int yPos;
    int xStartingPos;
    int xEndingPos;

    bool operator<(const TPos& other) const {
        if (yPos != other.yPos) {
            return yPos < other.yPos;
        }
        if (xStartingPos != other.xStartingPos) {
            return xStartingPos < other.xStartingPos;
        }
        return xEndingPos < other.xEndingPos;
    }

    friend std::ostream& operator<<(std::ostream& os, const TPos& position) {
        os << "yPos: " << position.yPos << ", "
        << "xStartingPos: " << position.xStartingPos << ", "
        << "xEndingPos: " << position.xEndingPos;
        return os;
    }
};

/**
 * @brief Stores all necessary information of number to a map
 * 
 * @param numbers 
 * @param yPos 
 * @param xStartingPos 
 * @param xEndingPos 
 * @param value 
 */
void storeToMap(std::map<TPos, int> &numbers, int yPos, int xStartingPos, int xEndingPos, int value) {
    TPos pos{yPos, xStartingPos, xEndingPos};
    numbers[pos] = value;
}

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

/**
 * @brief Prints all numbers that follow convention of having adjacent symbol
 * 
 * @param numbers 
 */
void printPartNumbers(const std::map<TPos, int> &numbers) {
    std::cout << "Part numbers are: " << std::endl;
    for (const auto& pair : numbers) {
        const TPos& key = pair.first;
        int value = pair.second;
        std::cout << "Key: (" << key.yPos << ", " << key.xStartingPos << ", " << key.xEndingPos << "), Value: " << value << std::endl;
    }
}

/**
 * @brief Prints all asterixes with their positions
 * 
 * @param asterixPos 
 */
void printAsterixPos(const std::set<std::pair<int, int>> &asterixPos) {
    std::cout << "Asterix positions:" << std::endl;
    for(const auto &it : asterixPos) {
        std::cout << "(" << it.first << ", " << it.second << ")" << std::endl;
    }
}

/**
 * @brief Checks symbol existance on adjacent positions
 * 
 * @param prevLine 
 * @param nextLine 
 * @param startIndex 
 * @param endIndex 
 * @return true 
 * @return false 
 */
bool checkSymbolExistance(std::string prevLine, std::string nextLine, int startIndex, int endIndex) {
    if(prevLine != "") {
        for (int i = startIndex; i <= endIndex; i++) {
            if(!isdigit(prevLine[i]) && prevLine[i] != '.') {
                return true;
            }
        }
    }

    if(nextLine != "") {
        for (int i = startIndex; i <= endIndex; i++) {
            if(!isdigit(nextLine[i]) && nextLine[i] != '.') {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Sums all part numbers that have symbols on adjacent positions
 * 
 * @param lines 
 * @param numbers 
 * @param asterixPos 
 * @return int 
 */
int sumPartNumbers(std::vector<std::string> lines, std::map<TPos, int> &numbers, std::set<std::pair<int, int>> &asterixPos) {
    std::string prevLine = "", nextLine = "";
    int startIndex = 0, lineLength = lines[0].length(), sum = 0, vectorSize = lines.size(), currTmpLen = 0;

    std::string tmp;

    for(int lineCount = 0; lineCount < vectorSize; ++lineCount) {
        tmp = "";
        currTmpLen = 0;

        for (int i = 0; i < lineLength; i++) {
            if(!isdigit(lines[lineCount][i]) && currTmpLen == 0) {
                if(lines[lineCount][i] == '*') {
                    asterixPos.insert(std::make_pair(i, lineCount));
                }
                continue;
            } else if(isdigit(lines[lineCount][i])) {
                tmp += lines[lineCount][i];
                currTmpLen++;
            } else if(!isdigit(lines[lineCount][i]) && currTmpLen != 0) {
                if(lines[lineCount][i] == '*') {
                    asterixPos.insert(std::make_pair(i, lineCount));
                }
                
                if(i - currTmpLen - 1 >= 0) {
                    startIndex = i - currTmpLen - 1;
                } else {
                    startIndex = 0;
                }
                // Check existance of the symbol
                if((i - currTmpLen - 1 >= 0 && lines[lineCount][i - currTmpLen - 1] != '.') || (lines[lineCount][i] != '.')) {
                    int value = std::stoi(tmp);
                    sum += value;
                    storeToMap(numbers, lineCount, startIndex, i, value);
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

                    if(checkSymbolExistance(prevLine, nextLine, startIndex, i)) {
                        int value = std::stoi(tmp);
                        sum += value;
                        storeToMap(numbers, lineCount, startIndex, i, value);
                    }
                }
                tmp = "";
                currTmpLen = 0;
            }
        }

        if(tmp != "") {
            if(lineLength - currTmpLen - 1 >= 0) {
                startIndex = lineLength - currTmpLen - 1;
            } else {
                startIndex = 0;
            }

            if(lineLength - currTmpLen - 1 >= 0 && lines[lineCount][lineLength - currTmpLen - 1] != '.') {
                int value = std::stoi(tmp);
                sum += value;
                storeToMap(numbers, lineCount, startIndex, lineLength - 1, value);
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

                if(checkSymbolExistance(prevLine, nextLine, startIndex, lineLength - 1)) {
                    int value = std::stoi(tmp);
                    sum += value;
                    storeToMap(numbers, lineCount, startIndex, lineLength - 1, value);
                }
            }
        }
    }
    return sum;
}

/**
 * @brief Sums all gears. Finds 2 numbers that share asterix on adjacent position
 * 
 * @param asterixPos 
 * @param numbers 
 * @return int 
 */
int sumGearRatio(std::set<std::pair<int, int>> asterixPos, std::map<TPos, int> numbers) {
    int sum = 0;

    for (const auto &number1 : numbers) {
        const TPos& key1 = number1.first;
        int val1 = number1.second;
        for(const auto &asterix : asterixPos) {
            if(abs(asterix.second - key1.yPos) <= 1) {
                if(asterix.first >= key1.xStartingPos && asterix.first <= key1.xEndingPos) {
                    for(const auto &number2 : numbers) {
                        const TPos& key2 = number2.first;
                        // Skip same number
                        if(key2.yPos == key1.yPos && key2.xStartingPos == key1.xStartingPos && key2.xEndingPos == key1.xEndingPos) {
                            continue;
                        }

                        int val2 = number2.second;

                        if(abs(asterix.second - key2.yPos) <= 1) {
                            if(asterix.first >= key2.xStartingPos && asterix.first <= key2.xEndingPos) {
                                sum += val1 * val2;
                            }
                        }
                    }
                }
            }
        }
    }

    return sum / 2;
}

int main(void) {
    std::vector<std::string> lines;
    readFile(lines);

    // X and Y axis
    std::set<std::pair<int, int>> asterixPos;

    std::map<TPos, int> numbers;

    std::cout << "Sum of part Numbers is: " << sumPartNumbers(lines, numbers, asterixPos) << std::endl;

    std::cout << "Sum of gear ratios is: " << sumGearRatio(asterixPos, numbers) << std::endl;

    //printPartNumbers(numbers);

    //printAsterixPos(asterixPos);

    return 0;
}