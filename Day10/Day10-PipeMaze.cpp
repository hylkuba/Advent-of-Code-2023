/**
 * @author Jakub Hýl <hylkuba@gmail.com>
 * @date 10.12.2023
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <iomanip>

#define FILE "Day10-input.txt"

/*
 * '|' is a vertical pipe connecting north and south.
 * '-' is a horizontal pipe connecting east and west.
 * 'L' is a 90-degree bend connecting north and east.
 * 'J' is a 90-degree bend connecting north and west.
 * '7' is a 90-degree bend connecting south and west.
 * 'F' is a 90-degree bend connecting south and east.
 * '.' is ground; there is no pipe in this tile.
 * 'S' is the starting position of the animal; there is a pipe on this tile, but your sketch doesn't show what shape the pipe has.
 * 
 */

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

void printMaze(std::map<std::pair<size_t, size_t>, char> &maze, size_t edge) {
    for (size_t i = 1; i <= edge; i++) {
        for (size_t m = 1; m <= edge; m++) {
            std::cout << maze[std::make_pair(m, i)];
        }
        std::cout << std::endl;
    }
}

void printMap(const std::map<std::pair<size_t, size_t>, char> &maze) {
    for(const auto &element : maze) {
        std::cout << "(" << element.first.first << ", " << element.first.second << ") -> " << element.second << std::endl;
    }
}

void printLoop(std::map<std::pair<size_t, size_t>, bool> &loop, size_t size) {
    for (size_t i = 1; i <= size; i++) {
        for (size_t m = 1; m <= size; m++) {
            if(loop[std::make_pair(m, i)]) {
                std::cout << "#";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

void printLoopWithSpaces(std::map<std::pair<size_t, size_t>, char> &loop,
    std::set<std::pair<size_t, size_t>> &found, size_t size) {
    
    for (size_t i = 1; i <= size; i++) {
        for (size_t m = 1; m <= size; m++) {
            if(loop[std::make_pair(m, i)] != ' ') {
                std::cout << loop[std::make_pair(m, i)];
            } else if(found.find(std::make_pair(m, i)) != found.end()){
                std::cout << "\033[31m#\033[0m";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

void initializeLoop(std::map<std::pair<size_t, size_t>, char> &loop, size_t size) {
    for (size_t i = 1; i <= size; i++) {
        for (size_t m = 1; m <= size; m++) {
            loop[std::make_pair(m, i)] = ' ';
        }
    }
}

size_t storeMaze(const std::vector<std::string> &lines, std::map<std::pair<size_t, size_t>, char> &maze, std::pair<size_t, size_t> &sPos) {
    size_t y = 1;
    for(const auto &line : lines) {
        for(size_t i = 0; i < line.size(); i++) {
            maze[std::make_pair(i + 1, y)] = line[i];
            if(line[i] == 'S') {
                sPos = std::make_pair(i + 1, y);
            }
        }
        y++;
    }
    return y - 1;
}

std::pair<size_t, size_t> start(std::map<std::pair<size_t, size_t>, char> &maze, std::pair<size_t, size_t> &currPos, size_t &edge) {
    std::pair<size_t, size_t> nextPos = {0, 0};
    size_t x = currPos.first;
    size_t y = currPos.second;
    
    if(x - 1 >= 1) {
        char c = maze[std::make_pair(x - 1, y)];
        if(c == 'F' || c == '-' || c == 'L') {
            return std::make_pair(x - 1, y);
        }
    }
    
    if(x + 1 <= edge) {
        char c = maze[std::make_pair(x + 1, y)];
        if(c == 'J' || c == '-' || c == '7') {
            return std::make_pair(x + 1, y);
        }
    }
    
    if(y - 1 >= 1) {
        char c = maze[std::make_pair(x, y - 1)];
        if(c == 'L' || c == '|' || c == 'J') {
            return std::make_pair(x, y - 1);
        }
    }
    
    if(y + 1 <= edge) {
        char c = maze[std::make_pair(x, y + 1)];
        if(c == 'F' || c == '|' || c == '7') {
            return std::make_pair(x, y + 1);
        }
    }

    return nextPos;
}

std::pair<size_t, size_t> move(std::map<std::pair<size_t, size_t>, char> &maze,
    std::pair<size_t, size_t> &currPos, size_t &edge, std::pair<size_t, size_t> &prevPos) {
    
    char symbol = maze[currPos];
    std::pair<size_t, size_t> nextPos;

    switch(symbol) {
        case '|':
            return currPos.second + 1 != prevPos.second ?
                std::make_pair(currPos.first, currPos.second + 1) :
                    std::make_pair(currPos.first, currPos.second - 1);
        case '-':
            return currPos.first + 1 != prevPos.first ?
                std::make_pair(currPos.first + 1, currPos.second) :
                    std::make_pair(currPos.first - 1, currPos.second);
        case 'L':
            return currPos.first + 1 != prevPos.first ?
                std::make_pair(currPos.first + 1, currPos.second) :
                    std::make_pair(currPos.first, currPos.second - 1);
        case 'J':
            return currPos.first - 1 != prevPos.first ?
                std::make_pair(currPos.first - 1, currPos.second) :
                    std::make_pair(currPos.first, currPos.second - 1);
        case '7':
            return currPos.first - 1 != prevPos.first ?
                std::make_pair(currPos.first - 1, currPos.second) :
                    std::make_pair(currPos.first, currPos.second + 1);
        case 'F':
            return currPos.first + 1 != prevPos.first ?
                std::make_pair(currPos.first + 1, currPos.second) :
                    std::make_pair(currPos.first, currPos.second + 1);
    }
    return nextPos;
}

size_t getSteps(std::map<std::pair<size_t, size_t>, char> &maze, std::pair<size_t, size_t> &sPos,
    size_t &edge, std::map<std::pair<size_t, size_t>, char> &loop) {

    size_t steps = 0;
    std::pair<size_t, size_t> currPos = start(maze, sPos, edge);
    std::pair<size_t, size_t> prevPos = sPos;
    loop[sPos] = true;

    steps++;

    while(true) {
        if(currPos == sPos) {
            break;
        }
        std::pair<size_t, size_t> tmp = currPos;
        currPos = move(maze, currPos, edge, prevPos);
        loop[currPos] = maze[currPos];
        prevPos = tmp;
        steps++;
    }

    return steps / 2;
}

// TODO Mistake lies here!!
bool checkAround(size_t x, size_t y, std::map<std::pair<size_t, size_t>,
    char> &loop, size_t size, std::set<std::pair<size_t, size_t>> &found) {

    return (x != 1) && (x != size) && (y != 1) && (y != size) &&
        (loop[std::make_pair(x - 1, y)] != ' ' || found.find(std::make_pair(x - 1, y)) != found.end()) &&
        (loop[std::make_pair(x + 1, y)] != ' ' || found.find(std::make_pair(x + 1, y)) != found.end()) &&
        (loop[std::make_pair(x, y - 1)] != ' ' || found.find(std::make_pair(x, y - 1)) != found.end()) &&
        (loop[std::make_pair(x, y + 1)] != ' ' || found.find(std::make_pair(x, y + 1)) != found.end());
}

size_t getEnclosedCount(std::map<std::pair<size_t, size_t>, char> &loop, size_t size) {
    size_t sum = 0;
    std::set<std::pair<size_t, size_t>> found;
    for (size_t i = 1; i <= size; i++) {
        for (size_t m = 1; m <= size; m++) {
            if(loop[std::make_pair(m, i)] == ' ' && checkAround(m, i, loop, size, found)) {
                found.insert(std::make_pair(m, i));
                sum++;
            }
        }
    }

    printLoopWithSpaces(loop, found, size);

    return sum;
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    // Key is (x, y)
    std::map<std::pair<size_t, size_t>, char> maze;

    std::pair<size_t, size_t> sPos;
    size_t size = storeMaze(lines, maze, sPos);

    std::map<std::pair<size_t, size_t>, char> loop;
    initializeLoop(loop, size);

    std::cout << "Number of steps to get to the furthest point: " << getSteps(maze, sPos, size, loop) << std::endl;

    std::cout << "Number of tiles enclosed by the loop: " << getEnclosedCount(loop, size) << std::endl;
    
    //printMaze(maze, size);
    //printLoop(loop, size);

    return 0;
}