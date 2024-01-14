/**
 * @author Jakub Hýl <hylkuba@gmail.com>
 * @date 16.12.2023
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>

#define FILE "Day16-input.txt"

#define EMPTY 0
#define UP -1
#define DOWN -2
#define LEFT -3
#define RIGHT -4
#define PIPELINE -5
#define DASH -6

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

void printMap(
        std::map<std::pair<int, int>, char> &layout,
        const std::pair<int, int> dimensions) {
            
    for (int y = 1; y <= dimensions.second; y++) {
        for (int x = 1; x < dimensions.first; x++)
        {
            std::cout << layout[std::make_pair(x, y)];
        }
        std::cout << std::endl;
    }
}

std::pair<int, int> storeMap(
        const std::vector<std::string> &lines,
        std::map<std::pair<int, int>, char> &layout) {
    
    int y = 1, width = 0;
    for(const auto &line : lines) {
        if(y == 1) {
            width = line.length();
        }

        for (size_t i = 0; i < line.length(); i++) {
            layout[std::make_pair(i + 1, y)] = line[i];   
        }
        y++;
    }
    return std::make_pair(width, y - 1);
}

void printBeamPath(
        std::map<std::pair<int, int>, int> &beamPath,
        const std::pair<int, int> &dimensions) {
    
    std::cout << std::endl;
    for (int y = 1; y <= dimensions.second; y++) {
        for (int x = 1; x <= dimensions.first; x++) {
            switch(beamPath[std::make_pair(x, y)]) {
                case 0:
                    std::cout << '.';
                    break;
                case LEFT:
                    std::cout << '<';
                    break;
                case RIGHT:
                    std::cout << '>';
                    break;
                case UP:
                    std::cout << '^';
                    break;
                case DOWN:
                    std::cout << 'v';
                    break;
                case PIPELINE:
                    std::cout << "|";
                    break;
                case DASH:
                    std::cout << "-";
                    break;
                default:
                    std::cout << beamPath[std::make_pair(x, y)];
            }
        }
        std::cout << std::endl;
    }
}

/*
    * -1 UP
    * -2 DOWN
    * -3 LEFT
    * -4 RIGHT
*/
void addToBeamPath(
        std::map<std::pair<int, int>, int> &beamPath,
        std::pair<int, int> beamPos,
        std::pair<int, int> move) {

    if(beamPath[beamPos] > 0) {
        beamPath[beamPos]++;
    } else if(beamPath[beamPos] < 0) {
        beamPath[beamPos] = 2;
    } else {
        if(move.first == -1) {
            beamPath[beamPos] = LEFT;
        } else if(move.first == 1) {
            beamPath[beamPos] = RIGHT;
        } else if(move.first == DASH) {
            beamPath[beamPos] = DASH;
        } else if(move.second == -1) {
            beamPath[beamPos] = UP;
        } else if(move.second == 1) {
            beamPath[beamPos] = DOWN;
        } else if(move.second == PIPELINE) {
            beamPath[beamPos] = PIPELINE;
        }
    }
}

std::pair<int, int> oneStep(
        std::pair<int, int> &beamPos,
        const std::pair<int, int> &dimensions,
        std::pair<int, int> &move) {

    std::pair<int, int> prevPos = beamPos;
    beamPos.first += move.first;
    beamPos.second += move.second;

    if(    beamPos.first < 1 
        || beamPos.first > dimensions.first
        || beamPos.second < 1
        || beamPos.second > dimensions.second) {
            beamPos = std::make_pair(0, 0);
            return std::make_pair(0, 0);
    }

    return prevPos;
}

void moveBeam(
        std::map<std::pair<int, int>, char> &layout,
        std::map<std::pair<int, int>, int> &beamPath,
        std::pair<int, int> beamPos,
        const std::pair<int, int> &dimensions,
        std::pair<int, int> move,
        std::set<std::pair<int, int>> &usedSepartions) {
    
    std::pair<int, int> prevPos;

    while(true) {
        prevPos = oneStep(beamPos, dimensions, move);
        //std::cout << beamPos.first << ", " << beamPos.second << std::endl;
        
        if(beamPos == std::make_pair(0, 0) || usedSepartions.find(beamPos) != usedSepartions.end()) {
            break;
        }

        switch(layout[beamPos]) {
            case '.':
                addToBeamPath(beamPath, beamPos, move);
                break;
            case '-':
                if(prevPos.second != beamPos.second) {
                    usedSepartions.insert(beamPos);

                    move = std::make_pair(-1, 0);
                    moveBeam(layout, beamPath, beamPos, dimensions, move, usedSepartions);

                    move = std::make_pair(1, 0);
                    moveBeam(layout, beamPath, beamPos, dimensions, move, usedSepartions);

                    addToBeamPath(beamPath, beamPos, std::make_pair(DASH, move.second));
                } else {
                    addToBeamPath(beamPath, beamPos, move);
                }
                break;
            case '|':
                if(prevPos.first != beamPos.first) {
                    usedSepartions.insert(beamPos);

                    move = std::make_pair(0, -1);
                    moveBeam(layout, beamPath, beamPos, dimensions, move, usedSepartions);

                    move = std::make_pair(0, 1);
                    moveBeam(layout, beamPath, beamPos, dimensions, move, usedSepartions);

                    addToBeamPath(beamPath, beamPos, std::make_pair(move.first, PIPELINE));
                } else {
                    addToBeamPath(beamPath, beamPos, move);
                }
                break;
            case '\\':
                if(prevPos.first < beamPos.first) {
                    move = std::make_pair(0, 1);
                } else if(prevPos.first > beamPos.first) {
                    move = std::make_pair(0, -1);
                } else if(prevPos.second > beamPos.second) {
                    move = std::make_pair(-1, 0);
                } else if(prevPos.second < beamPos.second) {
                    move = std::make_pair(1, 0);
                }
                addToBeamPath(beamPath, beamPos, move);
                break;
            case '/':
                if(prevPos.first < beamPos.first) {
                    move = std::make_pair(0, -1);
                } else if(prevPos.first > beamPos.first) {
                    move = std::make_pair(0, 1);
                } else if(prevPos.second > beamPos.second) {
                    move = std::make_pair(1, 0);
                } else if(prevPos.second < beamPos.second) {
                    move = std::make_pair(-1, 0);
                }
                addToBeamPath(beamPath, beamPos, move);
                break;
        }
    }
}

void initializeBeamPath(
        std::map<std::pair<int, int>, int> &beamPath,
        const std::pair<int, int> dimensions) {

    for (int y = 1; y <= dimensions.second; y++) {
        for (int x = 1; x <= dimensions.first; x++) {
            beamPath[std::make_pair(x, y)] = EMPTY;
        }
    }
}

size_t count(
        std::map<std::pair<int, int>, int> &beamPath,
        const std::pair<int, int> dimensions) {
    
    size_t sum = 0;
    for (int y = 1; y <= dimensions.second; y++) {
        for (int x = 1; x <= dimensions.first; x++) {
            if(beamPath[std::make_pair(x, y)] != EMPTY) {
                sum++;
            }
        }
    }
    return sum;
}

size_t energize(
        std::map<std::pair<int, int>, char> &layout,
        const std::pair<int, int> dimensions,
        std::pair<int, int> startingPos,
        std::pair<int, int> move) {

    std::map<std::pair<int, int>, int> beamPath;

    initializeBeamPath(beamPath, dimensions);

    std::set<std::pair<int, int>> usedSepartions;
    moveBeam(layout, beamPath, startingPos, dimensions, move, usedSepartions);

    //printBeamPath(beamPath, dimensions);

    return count(beamPath, dimensions);
}

size_t findBest(std::map<std::pair<int, int>, char> &layout,
        const std::pair<int, int> dimensions) {

    size_t sum = 0;
    for (int x = 1; x <= dimensions.first; x++) {
        size_t curr = energize(layout, dimensions, std::make_pair(x, 0), std::make_pair(0, 1));

        if(curr > sum) {
            sum = curr;
        }

        curr = energize(layout, dimensions, std::make_pair(x, dimensions.second + 1), std::make_pair(0, -1));

        if(curr > sum) {
            sum = curr;
        }
    }

    for (int y = 1; y <= dimensions.second; y++) {
        size_t curr = energize(layout, dimensions, std::make_pair(0, y), std::make_pair(1, 0));

        if(curr > sum) {
            sum = curr;
        }

        curr = energize(layout, dimensions, std::make_pair(dimensions.first + 1, y), std::make_pair(-1, 0));

        if(curr > sum) {
            sum = curr;
        }
    }

    return sum;
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::map<std::pair<int, int>, char> layout;

    std::pair<int, int> dimensions = storeMap(lines, layout);

    //printMap(layout, dimensions);

    std::cout << "Tiles that end up being energized: " << energize(layout, dimensions,
        std::make_pair(0, 1), std::make_pair(1, 0)) << std::endl;

    std::cout << "Most tiles energized after finding correct start: " << findBest(layout, dimensions) << std::endl;

    return 0;
}