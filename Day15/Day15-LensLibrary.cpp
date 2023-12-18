#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <iomanip>

#define FILE "Day15-input.txt"

/**
 * @brief Reads file defined as FILE
 * 
 * @param lines vector of strings where all lines of file will be stored
 */
void readFile(std::string &word) {
    std::ifstream inputFile(FILE);

    if (inputFile.is_open()) {
        std::getline(inputFile, word);
        inputFile.close();
    } else {
        std::cerr << "Unable to open the file." << std::endl;
    }
}

/**
 * @brief Prints vector of strings
 * 
 * @param words 
 */
void printWords(const std::vector<std::string> &words) {
    for(const auto &word : words) {
        std::cout << word << std::endl;
    }
}

/**
 * @brief Stores a line read from file into vector of strings, separates them by comma ','
 * 
 * @param line 
 * @param words 
 */
void storeWords(std::string &line, std::vector<std::string> &words) {
    size_t i = 0;
    std::string tmp = "";

    while(i != line.length()) {
        if(line[i] == ',') {
            words.push_back(tmp);
            tmp = "";
        } else {
            tmp += line[i];
        }
        i++;
    }
    words.push_back(tmp);
}

int main(void) {
    std::string line;

    readFile(line);

    std::vector<std::string> words;

    storeWords(line, words);

    printWords(words);

    return 0;
}