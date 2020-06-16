#include "filesystem.hpp"

#include <regex>
#include <iostream>

std::string expand_home(const std::string & x) {
    // LINUX specific
    // expand $HOME or ~ to HOME environment variable
    char *home = getenv("HOME");
    if (home!=NULL) {
        std::regex re ("(\\$HOME|~)");
        auto y = std::regex_replace(x, re, home);
        return y;
    } else {
        return x;
    }
}

fs::path parse_directory(const std::string & x, bool exists, bool create) {
    
    fs::path p{expand_home(x)};
    //p = fs::absolute(p);

    if (fs::exists(p)) {
        if (!fs::is_directory(p)) {
            throw std::runtime_error("Not a valid directory: " + p.string());
        }
    } else if (exists) {
        if (create) {
            if (!fs::create_directories(p)) {
                throw std::runtime_error("Could not create directory: " + p.string());
            }
        } else {
            throw std::runtime_error("Directory does not exist: " + p.string());
        }

    }

    return p;
}

fs::path parse_file(const std::string & x, bool exists) {

    fs::path p{expand_home(x)};
    //p = fs::absolute(p);

    if (fs::exists(p)) {
        if (!fs::is_regular_file(p)) {
           throw std::runtime_error("Not a valid file: " + p.string());
        }
    } else if (exists) {
        throw std::runtime_error("Not an existing file: " + p.string());
    }

    return p;
}