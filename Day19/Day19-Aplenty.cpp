#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>

#define FILE "Day19-input.txt"

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

class CPart {
public:
    CPart(int x, int m, int a, int s) : m_x(x), m_m(m), m_a(a), m_s(s) {}

    friend std::ostream& operator<<(std::ostream& os, const CPart& obj) {
        os << "m_x: " << std::setw(4)<< obj.m_x << ", "
        << "m_m: " << std::setw(4)<< obj.m_m << ", "
        << "m_a: " << std::setw(4)<< obj.m_a << ", "
        << "m_s: " << std::setw(4)<< obj.m_s;
        return os;
    }

    int m_x;
    int m_m;
    int m_a;
    int m_s;
};

struct TRule {
    std::string m_name;
    char m_sign;
    int m_value;
    std::string posOutcome;
    std::string negativeOutcome;    // null string if no negative outcome

    friend std::ostream& operator<<(std::ostream& os, const TRule& rule) {
        os << "Name: " << rule.m_name << ", "
        << "Sign: " << rule.m_sign << ", "
        << "Value: " << rule.m_value << ", "
        << "Positive Outcome: " << rule.posOutcome << ", "
        << "Negative Outcome: " << rule.negativeOutcome;
        return os;
    }
};

void printParts(const std::vector<CPart> &parts) {
    for(const auto &part : parts) {
        std::cout << part << std::endl;
    }
}

void storePart(std::string line, std::vector<CPart> &parts) {
    line = line.substr(1, line.size() - 2);

    std::istringstream iss(line);
    std::string pair;

    int x, m, a, s;

    while (std::getline(iss, pair, ',')) {
        char keyChar = pair[0];
        std::string value = pair.substr(pair.find('=') + 1);

        switch (keyChar)
        {
        case 'x':
            x = stoi(value);
            break;
        case 'm':
            m = stoi(value);
            break;
        case 'a':
            a = stoi(value);
            break;
        case 's':
            s = stoi(value);
            break;
        default:
            break;
        }
    }

    CPart myPart{x, m, a, s};

    parts.push_back(myPart);
}

void storeWorkflow(std::string line, std::map<std::string, std::vector<TRule>> &workflows) {

}

void storeInput(
        std::vector<std::string> &lines,
        std::vector<CPart> &parts,
        std::map<std::string, std::vector<TRule>> &workflows) {
    
    bool rules = true;
    for(auto &line : lines) {
        if(line == "") {
            rules = false;
            continue;
        }

        if(rules) {
            storeWorkflow(line, workflows);
        } else {
            storePart(line, parts);
        }
    }
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::vector<CPart> parts;
    std::map<std::string, std::vector<TRule>> workflows;

    storeInput(lines, parts, workflows);

    printParts(parts);

    return 0;
}