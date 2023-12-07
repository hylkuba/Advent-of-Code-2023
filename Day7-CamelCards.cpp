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

void initializeMap(std::map<char, int> &myMap) {
    const std::string order = "AKQJT98765432";
    for (size_t i = 0; i < order.length(); i++) {
        myMap[order[i]] = 0;
    }
}

bool fiveOfAKind(const std::string& hand) {
    char prev = hand[0];
    for (size_t i = 1; i < hand.length(); i++) {
        if(hand[i] != prev) return false;
    }
    
    return true;
}

bool fourOfAKind(const std::string& hand) {
    std::map<char, int> existence;
    initializeMap(existence);

    for (size_t i = 0; i < hand.length(); i++) {
        existence[hand[i]] += 1;
    }

    for(const auto &element : existence) {
        if(element.second == 4) {
            return true;
        }
    }

    return false;
}

bool fullHouse(const std::string& hand) {
    std::map<char, int> existance;
    initializeMap(existance);

    for (size_t i = 0; i < hand.length(); i++) {
        existance[hand[i]] += 1;
    }

    int count = 0;
    bool two = false;
    for(const auto &element : existance) {
        if(element.second == 3) {
            count++;
        } else if(element.second == 2 && !two) {
            count++;
            two = true;
        }
    }

    return count == 2;
}

bool threeOfAKind(const std::string& hand) {
    std::map<char, int> existance;
    initializeMap(existance);

    for (size_t i = 0; i < hand.length(); i++) {
        existance[hand[i]] += 1;
    }

    for(const auto &element : existance) {
        if(element.second == 3) {
            return true;
        }
    }

    return false;
}

bool twoPair(const std::string& hand) {
    std::map<char, int> existance;
    initializeMap(existance);

    for (size_t i = 0; i < hand.length(); i++) {
        existance[hand[i]] += 1;
    }

    int cnt = 0;
    for(const auto &element : existance) {
        if(element.second == 2) {
            cnt++;
        }
    }

    return cnt == 2;
}

bool onePair(const std::string& hand) {
    std::map<char, int> existance;
    initializeMap(existance);

    for (size_t i = 0; i < hand.length(); i++) {
        existance[hand[i]] += 1;
    }

    int cnt = 0;
    for(const auto &element : existance) {
        if(element.second == 2) {
            cnt++;
        }
    }

    return cnt == 1;
}

bool letterComparator(const char a, const char b) {
    const std::string order = "AKQJT98765432";
    return order.find(a) > order.find(b);
}

// Works like operator <
bool highCard(const std::string &hand1, const std::string &hand2) {
    for (size_t i = 0; i < hand1.length(); i++) {
        if (hand1[i] != hand2[i]) {
            return letterComparator(hand1[i], hand2[i]);
        }
    }
    return false;
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

void initializeCard(TCard &card) {
    card.fiveOfAKind = false;
    card.fourOfAKind = false;
    card.fullHouse = false;
    card.threeOfAKind = false;
    card.twoPair = false;
    card.onePair = false;
}

void storeCards(const std::vector<std::string> &lines,
    std::set<TCard> &cards) {

    for(const auto &line : lines) {
        std::istringstream iss(line);
        TCard newCard;
        iss >> newCard.hand;
        iss >> newCard.value;
        initializeCard(newCard);
        if(fiveOfAKind(newCard.hand)) {
            newCard.fiveOfAKind = true;
        } else if(fourOfAKind(newCard.hand)) {
            newCard.fourOfAKind = true;
        } else if(fullHouse(newCard.hand)) {
            newCard.fullHouse = true;
        } else if(threeOfAKind(newCard.hand)) {
            newCard.threeOfAKind = true;
        } else if(twoPair(newCard.hand)) {
            newCard.twoPair = true;
        } else if(onePair(newCard.hand)) {
            newCard.onePair = true;
        }
        cards.insert(newCard);
    }        
}

size_t sumCards(const std::set<TCard> &cards) {
    size_t sum = 0;
    size_t count = 1;
    for(const auto &card : cards) {
        sum += card.value * count++;
    }
    return sum;
}

int main(void) {
    std::vector<std::string> lines;
    std::set<TCard> cards;

    readFile(lines);

    storeCards(lines, cards);

    /*for(const auto &card : cards) {
        std::cout << card << "four of a kind" << card.fourOfAKind << std::endl;
    }*/

    std::cout << "Sum of cards is: " << sumCards(cards) << std::endl;

    return 0;
}