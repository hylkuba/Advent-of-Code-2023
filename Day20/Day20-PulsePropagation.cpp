#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

#define FILE "Day20-input.txt"

#define BUTTON_PRESS 1

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
    bool status; // OFF initially
    // false = OFF, true = ON

    // CONJUNCTION
    bool recentPulse; // LOW pulse initially
    bool low; // True if all inputs so far were HIGH
    // false = LOW, true = HIGH
};

void printModules(
        const std::map<std::string, TType> &modules) {
    
    for(const auto &pair : modules) {
        if(pair.second.type == '%') {
            std::cout << "Flip-flop: " << pair.first << " | Status: ";
            std::cout << (pair.second.status ? "ON" : "OFF");
        } else {
            std::cout << "Conjunction: " << pair.first << " | Recent pulse: ";
            std::cout << (pair.second.status ? "HIGH" : "LOW");
        }
        std::cout << std::endl;
    }
}

void printDestinations(
        const std::map<std::string, std::vector<std::string>> &destinations) {

    for(const auto &pair : destinations) {
        std::cout << pair.first << " -> ";
        for(const auto &dest : pair.second) {
            std::cout << dest << ", ";
        }
        std::cout << std::endl;
    }
}

void printChangeOfSignal(
        std::pair<std::pair<std::string, std::string>, bool> change) {

    std::cout << change.first.second;
    std::cout << " -"<< (change.second ? "high" : "low") << "-> " 
        << change.first.first << std::endl;
}

void storeModules(
        std::vector<std::string> &lines,
        std::map<std::string, TType> &modules,
        std::vector<std::string> &broadcaster,
        std::map<std::string, std::vector<std::string>> &destinations) {

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
                
                broadcaster.push_back(currWord);
            }
        } else {
            TType tmp;
            tmp.type = currWord[0];
            std::string currName = currWord.erase(0, 1);
            modules[currName] = tmp;

            // Skip the "->"
            iss >> currWord;

            // Extract each string value separately into the vector
            while (iss >> currWord) {
                // Remove commas from the current word
                currWord.erase(std::remove_if(currWord.begin(), currWord.end(),
                    [](char c) { return c == ','; }), currWord.end());
                
                destinations[currName].push_back(currWord);
            }
        }
    }
}

/**
 * @brief Changes signal for current module, and sends appropriate one
 * 
 * @param modules 
 * @param destinations 
 * @param currModule 
 * @param currSignal 
 * @return std::vector<std::pair<std::pair<std::string, std::string>, bool>> 
 *          * First pair stores string of (Next, Prev), bool value is signal
 */
std::vector<std::pair<std::pair<std::string, std::string>, bool>> changeSignal(
        std::map<std::string, TType> &modules,
        std::map<std::string, std::vector<std::string>> &destinations,
        std::pair<std::pair<std::string, std::string>, bool> &curr) {
    
    std::vector<std::pair<std::pair<std::string, std::string>, bool>> changes;

    std::string currModule = curr.first.first;
    std::string prevModule = curr.first.second;
    bool currSignal = curr.second;

    // Check if currModule has destinations
    if(destinations.count(currModule) < 1) {
        return changes;
    }

    //printChangeOfSignal(curr);

    for(const auto &destination : destinations[currModule]) {
        if(modules.count(destination) > 0) {
            if(modules[destination].type == '%') {
                if(!currSignal) {
                    modules[destination].status = !modules[destination].status;
                    changes.push_back(std::make_pair(std::make_pair(destination, currModule), modules[destination].status));
                }
            } else {
                modules[destination].recentPulse = currSignal;
                if(!currSignal) {
                    modules[destination].low = true;
                }
                changes.push_back(std::make_pair(std::make_pair(destination, currModule), modules[destination].low));
            }
        }
    }
    
    return changes;
}

size_t sumTotal(
        std::vector<std::string> &broadcaster,
        std::map<std::string, TType> &modules,
        std::map<std::string, std::vector<std::string>> &destinations) {
    
    size_t lowPules = 0, highPulses = 0;

    // Repeat BUTTON_PRESS times
    for (size_t i = 0; i < BUTTON_PRESS; i++) {
        // Queue is current piece and bool is value of signal, FALSE = LOW, true otherwise
        std::queue<std::pair<std::pair<std::string, std::string>, bool>> q;

        // Fill the queue with broadcaster
        for(auto &broad : broadcaster) {
            q.push(std::make_pair(std::make_pair(broad, "Broadcaster"), false));
        }

        std::cout << std::endl;
        printChangeOfSignal(std::make_pair(std::make_pair("Broadcaster", "Button"), false));
        
        while(!q.empty()) {
            std::pair<std::pair<std::string, std::string>, bool> curr = q.front();
            q.pop();
            printChangeOfSignal(curr);
            
            std::vector<std::pair<std::pair<std::string, std::string>, bool>> changes
                = changeSignal(modules, destinations, curr);
            
            if(changes.size() > 0) {
                for(auto &pair : changes) {
                    q.push(pair);
                }
            }
        }
    }

    return lowPules * highPulses;
}

int main(void) {
    std::vector<std::string> lines;

    readFile(lines);

    std::map<std::string, TType> modules;
    std::map<std::string, std::vector<std::string>> destinations;

    std::vector<std::string> broadcaster;

    storeModules(lines, modules, broadcaster, destinations);

    //printModules(modules);
    //printDestinations(destinations);

    std::cout << "Total number of pulses sent: " << sumTotal(broadcaster, modules, destinations) << std::endl;

    return 0;
}