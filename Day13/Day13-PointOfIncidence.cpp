/**
 * @author Jakub Hýl <hylkuba@gmail.com>
 * @date 13.12.2023
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <iterator>

#define FILE "Day13-input.txt"

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
 * @brief Prints pattern in original format
 * 
 * @param patterns 
 */
void printPatterns(const std::vector<std::vector<std::vector<char>>> &patterns) {
    for(const auto &pattern : patterns) {
        for(const auto &row : pattern) {
            for(const auto &column : row) {
                std::cout << column;
            }
            std::cout << std::endl;
        }
        std::cout << "-----------------------------------------------" << std::endl;
    }
}

/**
 * @brief Takes vector of lines from file and stores them appropriately to vector of patterns
 * 
 * @param lines 
 * @param patterns 
 */
void storeLines(std::vector<std::string> &lines, std::vector<std::vector<std::vector<char>>> &patterns) {
    std::vector<std::vector<char>> currPattern;
    for(const auto &line : lines) {
        if(line == "") {
            patterns.push_back(currPattern);
            currPattern.clear();
            continue;
        }
        std::vector<char> currRow;
        for (size_t i = 0; i < line.length(); i++) {
            currRow.push_back(line[i]);   
        }
        currPattern.push_back(currRow);
    }
    patterns.push_back(currPattern);
}

/**
 * @brief Generates combinations of all possible scenarios in which exactly one smudge happens for parsed pattern
 * 
 * @param pattern 
 * @return std::vector<std::vector<std::vector<char>>> Vector of all generated possibilites
 */
std::vector<std::vector<std::vector<char>>> generateCombinations(const std::vector<std::vector<char>> &pattern) {
    std::vector<std::vector<std::vector<char>>> generated;

    for (size_t i = 0; i < pattern.size(); ++i) {
        for (size_t j = 0; j < pattern[i].size(); ++j) {
            // Create a copy of the original pattern
            std::vector<std::vector<char>> newPattern = pattern;

            // Change exactly one character at a time
            newPattern[i][j] = (newPattern[i][j] == '.') ? '#' : '.';

            // Add the modified pattern to the result vector
            generated.push_back(newPattern);
        }
    }

    return generated;
}

/**
 * @brief Checks all lines horizontaly if they are reflected
 * 
 * @param pattern 
 * @param start 
 * @param end 
 * @param width 
 * @return true 
 * @return false 
 */
bool checkHorizontalTillEnd(const std::vector<std::vector<char>> &pattern,
    int start, int end, int width) {
    
    int currL = start;
    int currR = start + 1;
    do {
        for(int i = 0; i < width; i++) {
            if(pattern[currL][i] != pattern[currR][i]) {
                return false;
            }
        }
        currL--;
        currR++;
    } while(currL >= 0 && currR <= end);

    return true;
}

/**
 * @brief Loops through all horizontal axis and tries to find if there exists a horizontal reflection
 * 
 * @param pattern 
 * @return size_t 
 */
size_t horizontalMirror(const std::vector<std::vector<char>> &pattern) {
    size_t result = 0;
    int width = pattern[0].size(), height = pattern.size();

    for (int i = 0; i < height - 1; i++) {
        if(checkHorizontalTillEnd(pattern, i, height - 1, width)) {
            result = i + 1;
            break;
        }
    }

    return 100 * result;
}

/**
 * @brief Checks all lines vertically if they are reflected
 * 
 * @param pattern 
 * @param start 
 * @param end 
 * @param height 
 * @return true 
 * @return false 
 */
bool checkVerticalTillEnd(const std::vector<std::vector<char>> &pattern,
    int start, int end, int height) {
    
    int currL = start;
    int currR = start + 1;
    do {
        for(int i = 0; i < height; i++) {
            if(pattern[i][currL] != pattern[i][currR]) {
                return false;
            }
        }
        currL--;
        currR++;
    } while(currL >= 0 && currR <= end);

    return true;
}

/**
 * @brief Loops through all vertical axis and tries to find if there exists a vertical reflection
 * 
 * @param pattern 
 * @return size_t 
 */
size_t verticalMirror(const std::vector<std::vector<char>> &pattern) {
    size_t result = 0;
    int width = pattern[0].size(), height = pattern.size();

    for (int i = 0; i < width - 1; i++) {
        if(checkVerticalTillEnd(pattern, i, width - 1, height)) {
            result = i + 1;
            break;
        }
    }
    
    return result;
}

/**
 * @brief Finds reflection within pattern
 * 
 * @param pattern 
 * @param smudge 
 * @return size_t 
 */
size_t mirror(const std::vector<std::vector<char>> &pattern, bool smudge) {
    size_t result = ULLONG_MAX;

    if(smudge) {
        std::vector<std::vector<std::vector<char>>> generated = generateCombinations(pattern);
        //printPatterns(generated);
        size_t normalResultIndexVertical = verticalMirror(pattern);
        size_t normalResult = normalResultIndexVertical;

        size_t normalResultIndexHorizontal = 0;
        
        if(normalResult <= 0) {
            normalResult = horizontalMirror(pattern);
            normalResultIndexHorizontal = normalResult / 100;
        }

        size_t cnt = 0;
        for(const auto &other : generated) {
            size_t tmpResult = ULLONG_MAX, tmpResult2 = ULLONG_MAX;

            tmpResult = verticalMirror(other);
            tmpResult2 = horizontalMirror(other);


            if(tmpResult > 0 && tmpResult != ULLONG_MAX && tmpResult != normalResultIndexVertical) {
                result = tmpResult;
                cnt++;
                break;
            }

            if(tmpResult2 > 0 && tmpResult2 != ULLONG_MAX && tmpResult2 / 100 != normalResultIndexHorizontal) {
                result = tmpResult2;
                cnt++;
                break;
            }
        }

        if(cnt == 0) {
            result = normalResult;
        }
    } else {
        result = verticalMirror(pattern);
    
        if(result <= 0) {
            result = horizontalMirror(pattern);
        }
    }

    return result;
}

/**
 * @brief Sums up the values of all reflections
 * 
 * @param patterns 
 * @param smudge 
 * @return size_t 
 */
size_t summarize(const std::vector<std::vector<std::vector<char>>> &patterns, bool smudge) {
    size_t sum = 0;

    for(const auto &pattern : patterns) {
        sum += mirror(pattern, smudge);
    }

    return sum;
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);    

    /* First vector iterates through all patterns, 2nd vector iterates through rows,
        3rd vector iterates through columns
    */
    std::vector<std::vector<std::vector<char>>> patterns;

    storeLines(lines, patterns);

    std::cout << "Summary is: " << summarize(patterns, false) << std::endl;

    std::cout << "Summary with smudge: " << summarize(patterns, true) << std::endl;

    //printPatterns(patterns);

    return 0;
}