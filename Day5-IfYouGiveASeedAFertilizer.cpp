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
    /*for (const auto &entry : myMap) {
        std::cout << "(" << entry.first.first << ", " << entry.first.second  << ")" << " -> " << entry.second << std::endl;
    }*/
}

void loopAndStoreMap(std::map<std::pair<size_t, size_t>, size_t>  &myMap, size_t destination, size_t source, size_t range) {

}

void getThreeWords(std::istringstream &iss, size_t &destination, size_t &source, size_t &range) {
    std::string tmp;
    iss >> tmp;

    if(tmp == "") return;
    //std::cout << "Val: " << tmp << std::endl;
    destination = std::stoul(tmp);
    iss >> tmp;
    //std::cout << "Val: " << tmp << std::endl;
    source = std::stoul(tmp);
    iss >> tmp;
    //std::cout << "Val: " << tmp << std::endl;
    range = std::stoul(tmp);
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
    
    std::string category = "seeds:";
    size_t destination, source, range;

    for (const auto &line : lines) {
        std::istringstream iss(line);
        if(line == "seed-to-soil map:") {
            category = "seed-to-soil";
            continue;
        } else if(line == "soil-to-fertilizer map:") {
            category = "soil-to-fertilizer";
            continue;
        } else if(line == "fertilizer-to-water map:") {
            category = "fertilizer-to-water";
            continue;
        } else if(line == "water-to-light map:") {
            category = "water-to-light";
            continue;
        } else if(line == "light-to-temperature map:") {
            category = "light-to-temperature";
            continue;
        } else if(line == "temperature-to-humidity map:") {
            category = "temperature-to-humidity";
            continue;
        } else if(line == "humidity-to-location map:") {
            category = "humidity-to-location";
            continue;
        } 

        //std::cout << "category: " << category << std::endl;
        if (category == "seeds:") {
            std::string tmp;
            iss >> tmp;
            std::string seedStr;
            while (iss >> seedStr) {
                //std::cout << "seedStr: " << seedStr << std::endl;
                if(seedStr != "\n") {
                    size_t seed = std::stoul(seedStr);
                    seeds.insert(seed);
                }
            }
            continue;
        }

        if (category == "seed-to-soil") {
            getThreeWords(iss, destination, source, range);
            seedToSoil[std::make_pair(source, source + range)] = destination;
        } else if (category == "soil-to-fertilizer") {
            getThreeWords(iss, destination, source, range);
            soilToFertilizer[std::make_pair(source, source + range)] = destination;
        } else if (category == "fertilizer-to-water") {
            getThreeWords(iss, destination, source, range);
            fertilizerToWater[std::make_pair(source, source + range)] = destination;
        } else if (category == "water-to-light") {
            getThreeWords(iss, destination, source, range);
            waterToLight[std::make_pair(source, source + range)] = destination;
        } else if (category == "light-to-temperature") {
            getThreeWords(iss, destination, source, range);
            lightToTemperature[std::make_pair(source, source + range)] = destination;
        } else if (category == "temperature-to-humidity") {
            getThreeWords(iss, destination, source, range);
            temperatureToHumidity[std::make_pair(source, source + range)] = destination;
        } else if (category == "humidity-to-location") {
            getThreeWords(iss, destination, source, range);
            humidityToLocation[std::make_pair(source, source + range)] = destination;
        }
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

    /*std::cout << "Seeds: ";
    for (size_t seed : seeds) {
        std::cout << seed << " ";
    }
    std::cout << std::endl;*/

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