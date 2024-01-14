/**
 * @author Jakub Hýl <hylkuba@gmail.com>
 * @date 12.12.2023
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <iterator>

#define FILE "Day12-input.txt"

#define MULTIPLY 5

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
 * @brief Prints all rows
 * 
 * @param rows 
 */
void printRows(const std::vector<std::pair<std::string, std::vector<size_t>>> &rows) {
    for(const auto &row : rows) {
        std::cout << row.first << " ";
        for(const auto &num : row.second) {
            std::cout << num << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Checks if parsed string (combination) follows rules by numbers
 * 
 * @param combination 
 * @param numbers 
 * @return true 
 * @return false 
 */
bool checkValidCombination(const std::string &combination, const std::vector<size_t> &numbers) {
    size_t tmpLen = 0, index = 0, maxIndex = numbers.size();

    for (size_t i = 0; i < combination.length(); i++) {
        if(index == maxIndex) {
            if(combination[i] == '#') {
                return false;
            }
            continue;
        }
        
        if(combination[i] == '#') {
            tmpLen++;
        } else if (tmpLen > 0 && combination[i] == '.') {
            if(tmpLen == numbers[index]) {
                tmpLen = 0;
                index++;
            } else {
                return false;
            }
        }
    }

    if(index != maxIndex && tmpLen > 0) {
        if(tmpLen != numbers[index]) {
            return false;
        }
        index++;
    }

    return index == maxIndex;
}

/**
 * @brief Given a string, generates all possible permutations where '?' is converted to either '.' or '#'
 * 
 * @param input String containing '?'
 * @param index Index of current character 
 * @param current Current character
 * @param sum Sum of all possible combinations
 * @param numbers Vector of numbers
 */
void generateCombinations(const std::string& input, size_t index, std::string& current, size_t &sum, 
    const std::vector<size_t> &numbers) {
    
    if (index == input.size()) {
        sum += checkValidCombination(current, numbers) ? 1 : 0;
        return;
    }

    if (input[index] == '?') {
        current[index] = '.';
        generateCombinations(input, index + 1, current, sum, numbers);
        current[index] = '#';
        generateCombinations(input, index + 1, current, sum, numbers);
        current[index] = '?'; // backtracking to handle multiple question marks
    } else {
        generateCombinations(input, index + 1, current, sum, numbers);
    }
}

/**
 * @brief Sums all possible combinations that follow rules
 * 
 * @param row current string containing '?'
 * @param numbers Vector of amount of parts that should be continous 
 * @return size_t Sum of all possible combinations that follow rules
 */
size_t countArrangements(std::string row, const std::vector<size_t> &numbers) {
    size_t sum = 0;

    generateCombinations(row, 0, row, sum, numbers);

    return sum;
}

/**
 * @brief Iterates through vector containing lines read from file and stores them to vector of pairs
 *        following the assignment
 * 
 * @param rows Vector of pairs. 1st string containing '?', 2nd vector of numbers
 * @param lines Vector of lines read from file
 */
void storeLines(std::vector<std::pair<std::string, std::vector<size_t>>> &rows,
    const std::vector<std::string> &lines) {
    
    std::string word = "", partString = "";
    std::vector<size_t> numVector;
    bool num = false;
    
    for(const auto &line : lines) {
        for (size_t i = 0; i < line.length(); i++) {
            if(line[i] == ' ') {
                num = true;
                partString = word;
                word = "";
                continue;
            }

            if(!num) {
                word += line[i];
            } else {
                if(line[i] == ',') {
                    numVector.push_back(stoll(word));
                    word = "";
                } else {
                    word += line[i];
                }
            }
        }
        numVector.push_back(stoll(word));
        rows.push_back(std::make_pair(partString, numVector));
        numVector.clear();
        word = "";
    }
}

/**
 * @brief Multiplies vector size by the value defined in MULTIPLY
 * 
 * @param rows 
 * @param multiplied 
 */
void multipleRows(std::vector<std::pair<std::string, std::vector<size_t>>> &rows,
    std::vector<std::pair<std::string, std::vector<size_t>>> &multiplied) {

    for (const auto &row : rows) {
        std::string word = row.first;
        std::vector<size_t> numbers = row.second;

        for (size_t i = 0; i < MULTIPLY - 1; i++) {
            word += "?";
            word += row.first;
            for (const auto &num : row.second) {
                numbers.push_back(num);
            }
        }
        multiplied.push_back(std::make_pair(word, numbers));
    }
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::vector<std::pair<std::string, std::vector<size_t>>> rows;
    storeLines(rows, lines);

    size_t totalArrangements = 0;

    for (const auto &row : rows) {
        totalArrangements += countArrangements(row.first, row.second);
    }
    std::cout << "Total arrangements: " << totalArrangements << std::endl;

    std::vector<std::pair<std::string, std::vector<size_t>>> multipliedRows;
    
    multipleRows(rows, multipliedRows);

    size_t totalArrangementsMultiplied = 0;
    for (const auto &row : multipliedRows) {
        totalArrangementsMultiplied += countArrangements(row.first, row.second);
    }
    std::cout << "Total arrangements of multipliedRows: " << totalArrangementsMultiplied << std::endl;

    return 0;
}
