/**
 * @author Jakub Hýl <hylkuba@gmail.com>
 * @date 18.12.2023
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

#define FILE "Day18-input.txt"

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

struct TDig{
    std::pair<long long, long long> move;
    std::string color;
};

void printMap(
        std::map<std::pair<long long, long long>, bool> &mapOfDigs,
        std::pair<std::pair<long long, long long>, std::pair<long long, long long>> dimensions) {

    for (long long y = dimensions.second.second; y >= dimensions.second.first; y--) {
        for (long long x = dimensions.first.first; x <= dimensions.first.second; x++) {
            if(mapOfDigs[std::make_pair(x, y)]) {
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
}

void printDigs(const std::vector<TDig> &digs) {
    for(const auto &dig : digs) {
        if(dig.move.first > 0) {
            std::cout << "R " << dig.move.first;
        } else if(dig.move.first < 0) {
            std::cout << "L " << dig.move.first;
        } else if(dig.move.second > 0) {
            std::cout << "U " << dig.move.second;
        } else if(dig.move.second < 0) {
            std::cout << "D " << dig.move.second;
        }
        std::cout << " color: " << dig.color << std::endl;
    }
}

void storeLines(
        std::vector<std::string> &lines,
        std::vector<TDig> &digs) {
    
    for(const auto &line : lines) {
        char firstChar;
        long long number;

        std::istringstream iss(line);
        iss >> firstChar >> number;

        std::string restOfString = line.substr(line.find('#', 1));

        size_t lastSpace = restOfString.find_last_of(')');
        if (lastSpace != std::string::npos) {
            restOfString = restOfString.substr(1, lastSpace - 1);
        }

        TDig curr;
        curr.color = restOfString;
        switch (firstChar)
        {
        case 'R':
            curr.move = std::make_pair(number, 0);
            break;
        case 'L':
            curr.move = std::make_pair(-number, 0);
            break;
        case 'U':
            curr.move = std::make_pair(0, number);
            break;
        case 'D':
            curr.move = std::make_pair(0, -number);
            break;
        }
        digs.push_back(curr);
    }
}

void changeLimit(long long curr, long long &max, long long &min) {
    if(curr > max) {
        max = curr;
    } else if(curr < min) {
        min = curr;
    }
}

std::pair<std::pair<long long, long long>, std::pair<long long, long long>> digInMap(
        std::vector<TDig> &digs,
        std::map<std::pair<long long, long long>, bool> &mapOfDigs,
        size_t &numOfDigs) {
    
    std::pair<long long, long long> currPos = std::make_pair(0, 0);
    long long maxX = 0, maxY = 0, minX = 0, minY = 0;

    for(const auto &dig : digs) {
        long long currX = currPos.first + dig.move.first;
        long long currY = currPos.second + dig.move.second;

        // Dig on X or Y axis
        if(currX > currPos.first) {        
            for (long long i = currPos.first + 1; i <= currX; i++) {
                numOfDigs++;
                mapOfDigs[std::make_pair(i, currY)] = true;
            }
            changeLimit(currX, maxX, minX);
        } else if(currX < currPos.first) {
            for (long long i = currPos.first - 1; i >= currX; i--) {
                numOfDigs++;
                mapOfDigs[std::make_pair(i, currY)] = true;
            }
            changeLimit(currX, maxX, minX);
        } else if(currY > currPos.second) {
            for (long long i = currPos.second + 1; i <= currY; i++) {
                numOfDigs++;
                mapOfDigs[std::make_pair(currX, i)] = true;
            }
            changeLimit(currY, maxY, minY);
        } else if(currY < currPos.second) {
            for (long long i = currPos.second - 1; i >= currY; i--) {
                numOfDigs++;
                mapOfDigs[std::make_pair(currX, i)] = true;
            }
            changeLimit(currY, maxY, minY);
        }
        currPos.first = currX;
        currPos.second = currY;
    }

    return std::make_pair(std::make_pair(minX, maxX), std::make_pair(minY, maxY));
}

size_t cubicMeters(
        std::map<std::pair<long long, long long>, bool> &mapOfDigs,
        std::pair<std::pair<long long, long long>, std::pair<long long, long long>> dimensions) {
    
    // Find starting position
    std::pair<long long, long long> startPos;
    for (long long y = dimensions.second.first; y <= dimensions.second.second; y++) {
        bool found = false;
        for (long long x = dimensions.first.first; x <= dimensions.first.second; x++) {
            if(mapOfDigs[std::make_pair(x, y)] && !mapOfDigs[std::make_pair(x, y + 1)]) {
                startPos = std::make_pair(x, y + 1);
                found = true;
                break;
            }
        }
        if(found) {
            break;
        }
    }

    // START BFS from inside the loop, conditions will be that it cant go outside of borders
    std::queue<std::pair<long long, long long>> q;
    std::map<std::pair<long long, long long>, bool> visited;

    q.push(startPos);
    visited[startPos] = true;
    //mapOfDigs[startPos] = true;

    size_t sum = 1;

    while (!q.empty()) {
        std::pair<long long, long long> currentPos = q.front();
        q.pop();

        std::pair<long long, long long> neighbors[] = {
            {currentPos.first - 1, currentPos.second}, 
            {currentPos.first + 1, currentPos.second}, 
            {currentPos.first, currentPos.second - 1}, 
            {currentPos.first, currentPos.second + 1}  
        };

        for (const auto& neighbor : neighbors) {
            if (!visited[neighbor] && !mapOfDigs[neighbor]) {
                q.push(neighbor);
                visited[neighbor] = true;
                //mapOfDigs[neighbor] = true;
                sum++;
            }
        }
    }

    return sum;
}

void convertColorToMoves(std::vector<TDig> &digs) {
    for(auto &dig : digs) {
        std::string substring = dig.color.substr(0, 5);

        // Convert the substring to an integer and then to hex
        std::stringstream ss;
        ss << std::hex << substring;
        long long move;
        ss >> move;

        char dir = dig.color[5];
        switch (dir)
        {
        case '0':
            dig.move = std::make_pair(move, 0);
            break;
        case '1':
            dig.move = std::make_pair(0, move);
            break;
        case '2':
            dig.move = std::make_pair(-move, 0);
            break;
        case '3':
            dig.move = std::make_pair(0, move);
            break;
        }
    }
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::vector<TDig> digs;

    storeLines(lines, digs);

    std::map<std::pair<long long, long long>, bool> mapOfDigs;
    size_t numOfDigs = 0;

    std::pair<std::pair<long long, long long>, std::pair<long long, long long>> dimensions = digInMap(digs, mapOfDigs, numOfDigs);

    std::cout << "Cubic meters of lava to hold: " << (cubicMeters(mapOfDigs, dimensions) + numOfDigs) << std::endl;
    /*mapOfDigs.clear();

    convertColorToMoves(digs);

    dimensions = digInMap(digs, mapOfDigs, numOfDigs);

    std::cout << "Cubic meters of lava to hold after CONVERSION: " << (cubicMeters(mapOfDigs, dimensions) + numOfDigs) << std::endl;
    */

    return 0;
}