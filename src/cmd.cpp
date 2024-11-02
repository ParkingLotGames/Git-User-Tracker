#include "cmd.hpp"
#include <array>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

std::string GitCommand::executeCommand(const char* command) {
    // Create a fixed size buffer to store the output of the command
    std::array<char, 128> buffer;
    // Create a string to store the result of the command
    std::string result;

    // Define a deleter function for the unique_ptr that closes the file
    // Need to use a raw pointer type to FILE* here because pclose is a function and not a class with a destructor that can be invoked.
    // But then, need to use a lambda function as deleter for unique_ptr to resolve the warning about ignoring attributes on template argument.
    auto deleter = [](FILE* file){ pclose(file); };

    // Open a pipe to the command and create a unique_ptr to manage the file handle
    std::unique_ptr<FILE, decltype(deleter)> pipe(popen(command, "r"), deleter);
    
    // Throw an exception if the pipe could not be opened
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    
    // Read the output of the command from the pipe into the buffer and append it to the result string
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    // Return the result string containing the output of the command
    return result;
}
