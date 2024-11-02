#pragma once
#include <json/json.h>
#include <string>
#include <map>

class ConfigLoader {
public:
    std::map<std::string, std::string> loadUserColors();
private:
    std::string getConfigPath();
};