/**
 * @author Jakub Hýl <hylkuba@gmail.com>
 * @date 19.12.2023
*/

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

/**
 * @brief Part class, holds information of all values for current part
 * 
 */
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

    int sum() {
        return m_x + m_a + m_m + m_s;
    }

    int m_x;
    int m_m;
    int m_a;
    int m_s;
};

/**
 * @brief Rule struct that holds important variables
 * 
 */
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

/**
 * @brief 
 * 
 * @param workflows 
 * @param negativeOutcome 
 */
void printRules(
        std::map<std::string, std::vector<TRule>> &workflows,
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

/**
 * @brief Prints vector of parts
 * 
 * @param parts 
 */
void printParts(const std::vector<CPart> &parts) {
    for(const auto &part : parts) {
        std::cout << part << std::endl;
    }
}

/**
 * @brief Separates line into parts format
 * 
 * @param line 
 * @param parts 
 */
void storePart(
        std::string line,
        std::vector<CPart> &parts) {
    
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

/**
 * @brief Separates line into format of rules
 * 
 * @param line 
 * @param workflows 
 * @param negativeOutcome 
 */
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

/**
 * @brief Loops through lines. Divides them into parts and workflows.
 * Calls appropriate functions: storeWorkflow(), storePart()
 * 
 * @param lines 
 * @param parts 
 * @param workflows 
 * @param negativeOutcome 
 */
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

/**
 * @brief Checks whether the current rule (equation) is true
 * 
 * @param part 
 * @param rule 
 * @return true 
 * @return false 
 */
bool followsRule(
        CPart &part,
        const TRule &rule) {
    
    switch (rule.m_name)
    {
    case 'x':
        if(rule.m_sign == '>' && part.m_x > rule.m_value) {
            return true;
        } else if(rule.m_sign == '<' && part.m_x < rule.m_value) {
            return true;
        }
        break;
    case 'a':
        if(rule.m_sign == '>' && part.m_a > rule.m_value) {
            return true;
        } else if(rule.m_sign == '<' && part.m_a < rule.m_value) {
            return true;
        }
        break;
    case 'm':
        if(rule.m_sign == '>' && part.m_m > rule.m_value) {
            return true;
        } else if(rule.m_sign == '<' && part.m_m < rule.m_value) {
            return true;
        }
        break;
    case 's':
        if(rule.m_sign == '>' && part.m_s > rule.m_value) {
            return true;
        } else if(rule.m_sign == '<' && part.m_s < rule.m_value) {
            return true;
        }
        break;
    }
    return false;
}

/**
 * @brief loops through all rules, calls followsRule() to decide, whether
 * one of the conditions is true, otherwise returns the negativeOutcome
 * 
 * @param key 
 * @param part 
 * @param workflows 
 * @param negativeOutcome 
 * @return std::string 
 */
std::string workflowStep(
        std::string key,
        CPart &part,
        std::map<std::string, std::vector<TRule>> &workflows,
        std::map<std::string, std::string> &negativeOutcome) {
    
    for(const auto &work : workflows[key]) {
        if(followsRule(part, work)) {
            return work.posOutcome;
        }
    }

    return negativeOutcome[key];    
}

/**
 * @brief Loops through parts, for each part, calls workflowStep(). If the part
 * is accepted, adds the sum of parts(x,m,a,s) to the final sum
 * 
 * @param parts 
 * @param workflows 
 * @param negativeOutcome 
 * @return size_t 
 */
size_t sumParts(
        std::vector<CPart> &parts,
        std::map<std::string, std::vector<TRule>> &workflows,
        std::map<std::string, std::string> &negativeOutcome) {
    
    size_t sum = 0;
    for(auto &part : parts) {
        std::string returnVal = workflowStep("in", part, workflows, negativeOutcome);
        while(returnVal != "A" && returnVal != "R") {
            returnVal = workflowStep(returnVal, part, workflows, negativeOutcome);
        }

        if(returnVal == "A") {
            sum += part.sum();
        }
    }

    return sum;
}

//! SLOW APPROACH, LINEAR, takes a lot of time
/**
 * @brief Sums all possible combinations
 * 
 * @param workflows 
 * @param negativeOutcome 
 * @return size_t 
 */
size_t sumAllCombinations(
        std::map<std::string, std::vector<TRule>> &workflows,
        std::map<std::string, std::string> &negativeOutcome) {
    
    const int MAX = 4000;
    size_t sum = 0;

    for (int x = 1; x <= MAX; x++) {
        for (int m = 1; m <= MAX; m++) {
            for (int a = 1; a <= MAX; a++) {
                for (int s = 1; s <= MAX; s++) {
                    CPart tmp{x, m, a, s};
                    std::string returnVal = workflowStep("in", tmp, workflows, negativeOutcome);

                    while(returnVal != "A" && returnVal != "R") {
                        returnVal = workflowStep(returnVal, tmp, workflows, negativeOutcome);
                    }

                    if(returnVal == "A") {
                        sum += x + m + a + s;
                    }
                }
            }
        }
    }
    return sum;  
}

int main(void) {
    /**
     * @brief Stores lines from file input
     * 
     */
    std::vector<std::string> lines;

    readFile(lines);

    /**
     * @brief Vector of parts, containing their values
     * 
     */
    std::vector<CPart> parts;

    /**
     * @brief Map of rules, key is the name of rule,
     * value is vector of exact conditions
     * 
     */
    std::map<std::string, std::vector<TRule>> workflows;

    /**
     * @brief Map of negative outcomes for specific workflows
     * If non of the conditions in workflows are true, this holds
     * the return value
     * 
     */
    std::map<std::string, std::string> negativeOutcome;

    storeInput(lines, parts, workflows, negativeOutcome);

    std::cout << "Sum of all positive parts is: " << sumParts(parts, workflows, negativeOutcome) << std::endl;

    std::cout << "Sum of all combinations is: " << sumAllCombinations(workflows, negativeOutcome) << std::endl;

    return 0;
}