#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

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

/*
    ! FLIP FLOP '%'
        * Either ON or OFF
        * HIGH Pulse - NOTHING
        * LOW Pulse - SENDS: OFF to ON -> HIGH PULSE
        *                    ON to OFF -> LOW PULSE
 */

/*
    ! CONJUNCTION '&'
        * Remember the type of the most recent pulse received
            * from each of their connected input modules
        ? DEFAULT - low pulse
        * When a pulse is received, the conjunction module first
            * updates its memory for that input
        * THEN SENDS: HIGH -> LOW
        *             LOW -> HIGH
*/

/*
    ! BROADCASTER
    * Sends same signal it recieves to all of its directions
*/

/*
    ! BUTTON MODULE
    * When pressed, LOW signal sent to BROADCASTER
*/

/*
    * Pulses are always processed in the order they are sent.
    * So, if a pulse is sent to modules a, b, and c, and then
    * module a processes its pulse and sends more pulses, the
    * pulses sent to modules b and c would have to be handled first
*/

struct TType{
    char type;
    std::string name;
    
    // FLIP FLOP
    bool status;

    // CONJUNCTION
    bool recentPulse;
};

void storeModules(
        std::vector<std::string> &lines,
        std::map<std::string, TType> &modules,
        std::vector<std::string> &broadcaster) {

    for(const auto &line : lines) {
        std::istringstream iss(line);
        std::string currWord;
        iss >> currWord;

        if(currWord == "broadcaster") {
            // Skip the "->"
            iss >> currWord;

            // Extract each string value separately into the vector
            while (iss >> currWord) {
                // Remove commas from the current word
                currWord.erase(std::remove_if(currWord.begin(), currWord.end(),
                    [](char c) { return c == ','; }), currWord.end());
                
                // Store the current word in the vector
                broadcaster.push_back(currWord);
            }
        }
    }
}

size_t sumTotal(
        std::vector<std::string> &lines,
        std::map<std::string, TType> &modules) {

}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::map<std::string, TType> modules;

    std::vector<std::string> broadcaster;

    storeModules(lines, modules, broadcaster);

    for(const auto &broad : broadcaster) {
        std::cout << broad << std::endl;
    }

    std::cout << "Total number of pulses sent: " << sumTotal(lines, modules) << std::endl;

    return 0;
}