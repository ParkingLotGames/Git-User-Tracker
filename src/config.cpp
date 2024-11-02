#include "config.hpp"
#include <fstream>

std::map<std::string, std::string> ConfigLoader::loadUserColors() {
    // Create an empty map to store the colors
    std::map<std::string, std::string> colors;
    // Get the path of the configuration file
    std::string configPath = getConfigPath();
    // Open the configuration file
    std::ifstream file(configPath);

    // Check if the file is open
    if (file.is_open()) {
        Json::Value root;
        // Parse the JSON data from the file
        file >> root; 

        // Iterate over each member in the JSON data
        for (const auto& name : root.getMemberNames()) {
            // Store the color name and its corresponding value in the map
            colors[name] = root[name].asString(); 
        }
    }
    // Return the map containing the user colors
    return colors; 
}

std::string ConfigLoader::getConfigPath() {
    const char* home = getenv("HOME");
    return home ? std::string(home) + "/.usercolorconfig.json" : "";
}