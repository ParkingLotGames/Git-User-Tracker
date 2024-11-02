#pragma once
#include <string>

class GitCommand {
public:
    static std::string executeCommand(const char* command);
};