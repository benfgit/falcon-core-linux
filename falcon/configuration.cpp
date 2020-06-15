// ---------------------------------------------------------------------
// This file is part of falcon-core.
// 
// Copyright (C) 2015, 2016, 2017 Neuro-Electronics Research Flanders
// 
// Falcon-server is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Falcon-server is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with falcon-core. If not, see <http://www.gnu.org/licenses/>.
// ---------------------------------------------------------------------

#include <stdexcept>
#include <string>
#include <regex>
#include <iostream>

#include "yaml-cpp/yaml.h"

#include "configuration.hpp"

void Configuration::load(std::string filename) {

    char *home = getenv("HOME");
    std::regex re ("(\\$HOME|~)");
    filename = std::regex_replace( filename, re, home );

    YAML::Node node;
    try {
        node = YAML::LoadFile( filename );
        options_.from_yaml(node);
    }
    catch (YAML::BadFile & e ) { // config file does not exist, save default configuration
        try {
            save( filename );
            std::cout << "Default configuration saved to " << filename << "." << std::endl;
        } catch ( std::exception & e ) {
            std::cout << "Warning: could not save configuration file." << std::endl;
        }
    }

}

void Configuration::save(std::string filename) {
    options_.save_yaml(filename);
}


