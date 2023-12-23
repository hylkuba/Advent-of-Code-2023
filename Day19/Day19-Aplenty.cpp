#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

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

    CPart() : m_x(0), m_m(0), m_a(0), m_s(0) {
        
    }

    int m_x;
    int m_m;
    int m_a;
    int m_s;

    friend std::ostream& operator<<(std::ostream& os, const CPart& obj) {
        os << "m_x: " << obj.m_x << ", "
        << "m_m: " << obj.m_m << ", "
        << "m_a: " << obj.m_a << ", "
        << "m_s: " << obj.m_s;
        return os;
    }
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

void storePart(std::string line, std::vector<CPart> &parts) {

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
            storePart(line, parts);
        } else {
            storeWorkflow(line, workflows);
        }
    }
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::vector<CPart> parts;
    std::map<std::string, std::vector<TRule>> workflows;

    storeInput(lines, parts, workflows);

    return 0;
}