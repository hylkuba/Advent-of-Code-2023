#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <iomanip>
#include <limits>

#define FILE "Day7-input.txt"

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

bool compare(std::string hand1, std::string hand2) {
    return true;
}
struct TCard {
    std::string hand;
    size_t value;

    bool operator<(const TCard& other) const {
        return compare(hand, other.hand);
    }

    friend std::ostream& operator<<(std::ostream& os,
        const TCard& c) {

        os << "Hand: " << c.hand << ", Value: " << c.value;
        return os;
    }
};

void storeCards(const std::vector<std::string> &lines,
    std::set<TCard> &cards) {

    for(const auto &line : lines) {
        std::istringstream iss(line);
        TCard newCard;
        iss >> newCard.hand;
        iss >> newCard.value;
        cards.insert(newCard);
    }        
}

int main(void) {
    std::vector<std::string> lines;
    std::set<TCard> cards;

    readFile(lines);

    storeCards(lines, cards);

    for(const auto &card : cards) {
        std::cout << card << std::endl;
    }

    return 0;
}