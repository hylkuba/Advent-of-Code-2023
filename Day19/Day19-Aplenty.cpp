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
    char m_name;
    char m_sign;
    int m_value;
    std::string posOutcome;

    friend std::ostream& operator<<(std::ostream& os, const TRule& rule) {
        os << "If: " << rule.m_name << rule.m_sign
        << rule.m_value << " => " << rule.posOutcome;
        return os;
    }
};

void printRules(std::map<std::string, std::vector<TRule>> &workflows,
        std::map<std::string, std::string> &negativeOutcome) {

    for(const auto &work : workflows) {
        std::cout << work.first << std::endl;
        for(const auto &rule : work.second) {
            std::cout << rule << std::endl;
        }
        std::cout << "Negative: " << negativeOutcome[work.first] << std::endl;
        std::cout << "-------------------------------------------------------------" << std::endl;
    }
}

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
        int value = stoi(pair.substr(pair.find('=') + 1));

        switch (keyChar)
        {
        case 'x':
            x = value;
            break;
        case 'm':
            m = value;
            break;
        case 'a':
            a = value;
            break;
        case 's':
            s = value;
            break;
        default:
            break;
        }
    }

    CPart myPart{x, m, a, s};

    parts.push_back(myPart);
}

void storeWorkflow(
        std::string line,
        std::map<std::string, std::vector<TRule>> &workflows,
        std::map<std::string, std::string> &negativeOutcome) {
    
    int firstPos = line.find('{');
    std::string name = line.substr(0, firstPos);
    std::string rest = line.substr(firstPos + 1, line.size() - (firstPos + 2));
    
    std::istringstream iss(rest);
    std::string pair;
    while (std::getline(iss, pair, ',')) {
        std::string::size_type pos = pair.find(':');
        
        // If not found, end loop, store value as negative outcome
        if(pos == std::string::npos) {
            negativeOutcome[name] = pair;
            break;
        }
        
        char keyChar = pair[0];
        char sign = pair[1];
        std::string number =  pair.substr(2, pos - 2);
        int value = stoi(number);
        std::string positive = pair.substr(pos + 1);
        workflows[name].push_back({keyChar, sign, value, positive});
    }
}

void storeInput(
        std::vector<std::string> &lines,
        std::vector<CPart> &parts,
        std::map<std::string, std::vector<TRule>> &workflows,
        std::map<std::string, std::string> &negativeOutcome) {
    
    bool rules = true;
    for(auto &line : lines) {
        if(line == "") {
            rules = false;
            continue;
        }

        if(rules) {
            storeWorkflow(line, workflows, negativeOutcome);
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

    std::map<std::string, std::string> negativeOutcome;

    storeInput(lines, parts, workflows, negativeOutcome);

    //printParts(parts);
    printRules(workflows, negativeOutcome);

    return 0;
}