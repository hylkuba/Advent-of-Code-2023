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

bool fiveOfAKind(const std::string& hand) {

}

bool fourOfAKind(const std::string& hand) {

}

bool fullHouse(const std::string& hand) {

}

bool threeOfAKind(const std::string& hand) {

}

bool twoPair(const std::string& hand) {

}

bool onePair(const std::string& hand) {

}

bool letterComparator(const char a, const char b) {
    const std::string order = "AKQJT98765432";
    return order.find(a) > order.find(b);
}

// Works like operator <
bool highCard(const std::string &hand1, const std::string &hand2) {
    for (size_t i = 0; i < hand1.length(); i++) {
        if(hand1[i] != hand2 [i]) {
            return letterComparator(hand1[i], hand2[i]);
        }
    }
    return true;
}

struct TCard {
    std::string hand;
    size_t value;
    bool fiveOfAKind;
    bool fourOfAKind;
    bool fullHouse;
    bool threeOfAKind;
    bool twoPair;
    bool onePair;

    bool operator<(const TCard& other) const {
        if(fiveOfAKind && !other.fiveOfAKind) return false;
        else if(!fiveOfAKind && other.fiveOfAKind) return true;

        if(fourOfAKind && !other.fourOfAKind) return false;
        else if(!fourOfAKind && other.fourOfAKind) return true;

        if(fullHouse && !other.fullHouse) return false;
        else if(!fullHouse && other.fullHouse) return true;

        if(threeOfAKind && !other.threeOfAKind) return false;
        else if(!threeOfAKind && other.threeOfAKind) return true;

        if(twoPair && !other.twoPair) return false;
        else if(!twoPair && other.twoPair) return true;

        if(onePair && !other.onePair) return false;
        else if(!onePair && other.onePair) return true;

        return highCard(hand, other.hand);
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
        newCard.fiveOfAKind = fiveOfAKind(newCard.hand);
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