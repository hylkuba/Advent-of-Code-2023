/**
 * @author Jakub Hýl <hylkuba@gmail.com>
 * @date 7.12.2023
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <iomanip>

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

bool fiveOfAKind(const std::string& hand, bool joker) {
    char prev = hand[0];
    for (size_t i = 1; i < hand.length(); i++) {        
        if(!joker && hand[i] != prev) {
            return false;
        } else {
            // Joker
            if(hand[i] == 'J') continue;
            else if (prev == 'J') prev = hand[i];
            else if(hand[i] != prev) return false;
        }
    }
    
    return true;
}

bool fourOfAKind(const std::string& hand, bool joker) {
    std::map<char, int> existence;
    initializeMap(existence);

    for (size_t i = 0; i < hand.length(); i++) {
        existence[hand[i]] += 1;
    }

    for(const auto &element : existence) {
        if(element.second == 4) {
            return true;
        } else if(joker && element.first != 'J') {
            if(element.second + existence['J'] == 4) return true;
        }
    }

    return false;
}

bool fullHouse(const std::string& hand, bool joker) {
    std::map<char, int> existence;
    initializeMap(existence);

    for (size_t i = 0; i < hand.length(); i++) {
        existence[hand[i]] += 1;
    }

    int count = 0;
    bool two = false;

    if(!joker || existence['J'] == 0) {
        for(const auto &element : existence) {
            if(element.second == 3) {
                count++;
            } else if(element.second == 2 && !two) {
                count++;
                two = true;
            }
        }
    } else {
        // TODO joker check might be wrong
        for(const auto &element : existence) {
            // exclude JOKERS from counting
            if(element.second >= 1 && element.first != 'J') {
                count++;
            }
        }
    }

    return count == 2;
}

bool threeOfAKind(const std::string& hand, bool joker) {
    std::map<char, int> existence;
    initializeMap(existence);

    for (size_t i = 0; i < hand.length(); i++) {
        existence[hand[i]] += 1;
    }

    for(const auto &element : existence) {
        if(element.second == 3)
            return true;
        else if(joker && element.second + existence['J'] == 3)
            return true;
    }

    return false;
}

bool twoPair(const std::string& hand, bool joker) {
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
    
    if(joker && existance['J'] > 0 && cnt == 1) return true;

    return cnt == 2;
}

bool onePair(const std::string& hand, bool joker) {
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
    
    if(joker && cnt == 0 && existance['J'] > 0) return true;

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

bool jokerLetterComparator(const char a, const char b) {
    const std::string order = "AKQT98765432J";
    return order.find(a) > order.find(b);
}

// Works like operator <
bool jokerHighCard(const std::string &hand1, const std::string &hand2) {
    for (size_t i = 0; i < hand1.length(); i++) {
        if (hand1[i] != hand2[i]) {
            return jokerLetterComparator(hand1[i], hand2[i]);
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
    bool joker;

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
        
        //return highCard(hand, other.hand);
        return joker ? jokerHighCard(hand, other.hand) : highCard(hand, other.hand);
    }

    friend std::ostream& operator<<(std::ostream& os,
        const TCard& c) {

        os << "Hand: " << c.hand << ", Value: " << std::setw(3) << c.value << " | five: " << c.fiveOfAKind << " | four: " << c.fourOfAKind
            << " | fullHouse: " << c.fullHouse << " | three: " << c.threeOfAKind << " | 2pair: " << c.twoPair << " | 1pair: " << c.onePair;
        return os;
    }
};

void printCards(std::set<TCard> &cards) {
    for(const auto &card : cards) {
        std::cout << card << std::endl;
    }
}

void initializeCard(TCard &card) {
    card.fiveOfAKind = false;
    card.fourOfAKind = false;
    card.fullHouse = false;
    card.threeOfAKind = false;
    card.twoPair = false;
    card.onePair = false;
}

void storeCards(const std::vector<std::string> &lines,
    std::set<TCard> &cards, std::set<TCard> &jokerCards) {

    for(const auto &line : lines) {
        std::istringstream iss(line);

        TCard normalCard;
        TCard jokerCard;

        normalCard.joker = false;
        jokerCard.joker = true;
        iss >> normalCard.hand;
        jokerCard.hand = normalCard.hand;
        iss >> normalCard.value;
        jokerCard.value = normalCard.value;

        initializeCard(normalCard);
        initializeCard(jokerCard);

        if(fiveOfAKind(normalCard.hand, false)) {
            normalCard.fiveOfAKind = true;
        } else if(fourOfAKind(normalCard.hand, false)) {
            normalCard.fourOfAKind = true;
        } else if(fullHouse(normalCard.hand, false)) {
            normalCard.fullHouse = true;
        } else if(threeOfAKind(normalCard.hand, false)) {
            normalCard.threeOfAKind = true;
        } else if(twoPair(normalCard.hand, false)) {
            normalCard.twoPair = true;
        } else if(onePair(normalCard.hand, false)) {
            normalCard.onePair = true;
        }

        if(fiveOfAKind(jokerCard.hand, true)) {
            jokerCard.fiveOfAKind = true;
        } else if(fourOfAKind(jokerCard.hand, true)) {
            jokerCard.fourOfAKind = true;
        } else if(fullHouse(jokerCard.hand, true)) {
            jokerCard.fullHouse = true;
        } else if(threeOfAKind(jokerCard.hand, true)) {
            jokerCard.threeOfAKind = true;
        } else if(twoPair(jokerCard.hand, true)) {
            jokerCard.twoPair = true;
        } else if(onePair(jokerCard.hand, true)) {
            jokerCard.onePair = true;
        }

        cards.insert(normalCard);
        jokerCards.insert(jokerCard);
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
    std::set<TCard> jokerCards;

    readFile(lines);

    storeCards(lines, cards, jokerCards);

    //printCards(cards);
    //printCards(jokerCards);

    std::cout << "Sum of cards is: " << sumCards(cards) << std::endl;
    std::cout << "Sum of joker cards is: " << sumCards(jokerCards) << std::endl;

    return 0;
}