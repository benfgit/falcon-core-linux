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

#include <string>

#include "options/options.hpp"

class Configuration {
public:

    void load(std::string filename);
    void save(std::string filename);

    template <typename TValue>
    void add_option(std::string name, TValue & value, std::string description="", bool required=false) {
        options_.add(name, value, description, required);
    }

protected:
    options::OptionList options_;
};

class FalconConfiguration : public Configuration {
public:
    FalconConfiguration() : Configuration() {
        add_option("graph/file", graph_file, "");
        add_option("graph/autostart", graph_autostart, "");
        add_option("debug/enabled", debug_enabled, "");
        add_option("testing/enabled", testing_enabled, "");
        add_option("network/port", network_port, "");
        add_option("logging/path", logging_path, "");
        add_option("logging/screen/enabled", logging_screen_enabled, "");
        add_option("logging/cloud/enabled", logging_cloud_enabled, "");
        add_option("logging/cloud/port", logging_cloud_port, "");
        add_option("server_side_storage/environment", server_side_storage_environment, "");
        add_option("server_side_storage/resources", server_side_storage_resources, "");
        add_option("server_side_storage/custom", server_side_storage_custom, "");
    }

// CONFIG OPTIONS
public:
    options::String graph_file{""};
    options::Bool debug_enabled{false};
    options::Bool testing_enabled{false};
    options::Bool graph_autostart{false};
    options::Int network_port{5555};
    options::String logging_path{"./", options::isdir()};
    options::Bool logging_screen_enabled{true};
    options::Bool logging_cloud_enabled{true};
    options::Int logging_cloud_port{5556};
    options::String server_side_storage_environment{"./", options::isdir()};
    options::String server_side_storage_resources{"./", options::isdir()};
    options::ValueMap<options::String> server_side_storage_custom;
};



