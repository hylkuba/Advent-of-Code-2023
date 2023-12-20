#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <limits>
#include <queue>
#include <algorithm>

#define FILE "Day17-input.txt"

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

void printBlocks(
        std::map<std::pair<int, int>, int> &blocks,
        std::pair<int, int> dimensions) {
    
    for (int y = 1; y <= dimensions.second; y++) {
        for (int x = 1; x < dimensions.first; x++) {
            std::cout << blocks[std::make_pair(x, y)];
        }
        std::cout << std::endl;
    }
}

std::pair<int, int> storeBlocks(
        std::vector<std::string> &lines,
        std::map<std::pair<int, int>, int> &blocks) {
    
    int width = 0, y = 1;
    for(const auto &line : lines) {
        if(y == 1) {
            width = line.length();
        }

        for (size_t x = 0; x < line.length(); x++) {
            blocks[std::make_pair(x + 1, y)] = line[x] - 48;
        }
        y++;
    }

    return std::make_pair(width, y - 1);
}

int findLowestSumPath(std::map<std::pair<int, int>, int>& blocks, std::pair<int, int> dimensions) {
    std::queue<std::pair<int, int>> q;
    std::map<std::pair<int, int>, int> values;
    std::set<std::pair<int, int>> visited;
    std::map<std::pair<int, int>, std::pair<int, int>> parents;

    std::pair<int, int> start = std::make_pair(1, 1);
    q.push(start);
    values[start] = blocks[start];
    visited.insert(start);

    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    while (!q.empty()) {
        std::pair<int, int> current = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nx = current.first + dx[i];
            int ny = current.second + dy[i];

            if (nx >= 1 && nx <= dimensions.first && ny >= 1 && ny <= dimensions.second &&
                (visited.find(std::make_pair(nx, ny)) == visited.end())) {

                int sum = values[current] + blocks[std::make_pair(nx, ny)];
                std::pair<int, int> newPair = std::make_pair(nx, ny);

                if (visited.find(newPair) == visited.end() || sum < values[newPair]) {
                    q.push(newPair);
                    values[newPair] = sum;
                    visited.insert(newPair);
                    parents[newPair] = current;
                }
            }
        }
    }

    std::pair<int, int> current = dimensions;
    std::vector<std::pair<int, int>> path;

    while (current != start) {
        path.push_back(current);
        current = parents[current];
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end());

    std::cout << "Reconstructed Path: ";
    for (const auto& point : path) {
        std::cout << "(" << point.first << ", " << point.second << ") ";
    }
    std::cout << std::endl;

    return values[dimensions];
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::map<std::pair<int, int>, int> blocks;

    std::pair<int, int> dimensions = storeBlocks(lines, blocks);

    //printBlocks(blocks, dimensions);

    std::cout << "Least heat that can occur is: " << findLowestSumPath(blocks, dimensions) << std::endl;

    return 0;
}