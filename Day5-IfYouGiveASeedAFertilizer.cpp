#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <iomanip>

#define FILE "Day5-input.txt"

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

void printMap(const std::map<std::pair<size_t, size_t>, size_t>  &myMap) {
    std::cout << "Map size: " << myMap.size() << std::endl;
    for (const auto &entry : myMap) {
        std::cout << "(" << entry.first.first << ", " << entry.first.second << " -> " << entry.second << std::endl;
    }
}

void loopAndStoreMap(std::map<std::pair<size_t, size_t>, size_t>  &myMap, size_t destination, size_t source, size_t range) {

}

bool getThreeWords(std::istringstream &iss, size_t &destination, size_t &source, size_t &range) {
    std::string tmp;
    iss >> tmp;

    if(tmp == "\n") return false;
    std::cout << "Val: " << tmp << std::endl;
    destination = std::stoul(tmp);
    iss >> tmp;
    std::cout << "Val: " << tmp << std::endl;
    source = std::stoul(tmp);
    iss >> tmp;
    std::cout << "Val: " << tmp << std::endl;
    range = std::stoul(tmp);
    return true;
}

void storeToMaps(
    std::vector<std::string> &lines,
    std::set<size_t> &seeds,
    std::map<std::pair<size_t, size_t>, size_t> &seedToSoil,
    std::map<std::pair<size_t, size_t>, size_t> &soilToFertilizer,
    std::map<std::pair<size_t, size_t>, size_t> &fertilizerToWater,
    std::map<std::pair<size_t, size_t>, size_t> &waterToLight,
    std::map<std::pair<size_t, size_t>, size_t> &lightToTemperature,
    std::map<std::pair<size_t, size_t>, size_t> &temperatureToHumidity,
    std::map<std::pair<size_t, size_t>, size_t> &humidityToLocation) {
    
    std::string category = "";
    size_t destination, source, range;
    bool skipLine = false;

    for (const auto &line : lines) {
        std::istringstream iss(line);

        if((category == "seeds" || category == "seed-to-soil" || category == "soil-to-fertilizer")
            && !skipLine) {
            skipLine = true;
        } else {
            iss >> category;
            std::cout << "category: " << category << std::endl;
            continue;
        }

        if (category == "seeds:") {
            std::string seedStr;
            while (iss >> seedStr) {
                if(seedStr != "\n") {
                    size_t seed = std::stoul(seedStr);
                    seeds.insert(seed);
                }
            }
            continue;
        }

        if (category == "seed-to-soil") {
            // Continue to get new line as a input
            if(!getThreeWords(iss, destination, source, range)) break;
            seedToSoil[std::make_pair(source, source + range)] = destination;
        } else if (category == "soil-to-fertilizer") {
            if(!getThreeWords(iss, destination, source, range)) break;
            soilToFertilizer[std::make_pair(source, source + range)] = destination;
        } else if (category == "fertilizer-to-water") {
            if(!getThreeWords(iss, destination, source, range)) break;
            fertilizerToWater[std::make_pair(source, source + range)] = destination;
        } else if (category == "water-to-light") {
            if(!getThreeWords(iss, destination, source, range)) break;
            waterToLight[std::make_pair(source, source + range)] = destination;
        } else if (category == "light-to-temperature") {
            if(!getThreeWords(iss, destination, source, range)) break;
            lightToTemperature[std::make_pair(source, source + range)] = destination;
        } else if (category == "temperature-to-humidity") {
            if(!getThreeWords(iss, destination, source, range)) break;
            temperatureToHumidity[std::make_pair(source, source + range)] = destination;
        } else if (category == "humidity-to-location") {
            if(!getThreeWords(iss, destination, source, range)) break;
            humidityToLocation[std::make_pair(source, source + range)] = destination;
        }
        skipLine = false;
    }
}

int main(void) {
    std::vector<std::string> lines;

    std::set<size_t> seeds;
    /*
        !Key is (starting pos, ending position), Value is starting position to skip to
    */
    std::map<std::pair<size_t, size_t>, size_t> seedToSoil;
    std::map<std::pair<size_t, size_t>, size_t> soilToFertilizer;
    std::map<std::pair<size_t, size_t>, size_t> fertilizerToWater;
    std::map<std::pair<size_t, size_t>, size_t> waterToLight;
    std::map<std::pair<size_t, size_t>, size_t> lightToTemperature;
    std::map<std::pair<size_t, size_t>, size_t> temperatureToHumidity;
    std::map<std::pair<size_t, size_t>, size_t> humidityToLocation;
    
    readFile(lines);

    storeToMaps(lines, seeds, seedToSoil, soilToFertilizer,
    fertilizerToWater, waterToLight, lightToTemperature,
    temperatureToHumidity, humidityToLocation);

    std::cout << "Seeds: ";
    for (size_t seed : seeds) {
        std::cout << seed << " ";
    }
    std::cout << std::endl;

    std::cout << "seedToSoil: ";
    printMap(seedToSoil);

    std::cout << "soilToFertilizer: ";
    printMap(soilToFertilizer);

    std::cout << "fertilizerToWater: ";
    printMap(fertilizerToWater);

    std::cout << "waterToLight: ";
    printMap(waterToLight);

    std::cout << "lightToTemperature: ";
    printMap(lightToTemperature);

    std::cout << "temperatureToHumidity: ";
    printMap(temperatureToHumidity);

    std::cout << "humidityToLocation: ";
    printMap(humidityToLocation);    

    return 0;
}