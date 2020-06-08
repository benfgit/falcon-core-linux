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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <map>
#include <list>
#include <fstream>
#include <functional>

#include "yaml-cpp/yaml.h"

#include "../utilities/string.hpp"
#include "value.hpp"

namespace options {

enum class OptionError {
    validation_failed,
    conversion_to_yaml_failed,
    conversion_from_yaml_failed,
    requirement_failed
    };

bool get_nested_yaml_node(
    const YAML::Node& root,
    const std::vector<std::string>& path,
    YAML::Node & out);


class OptionBase {
public:

    OptionBase(std::string name, ValueBase & value, std::string description="", bool required=false);

    std::string name() const;

    std::string description() const;

    const std::vector<std::string> & path() const;

    bool is_required() const;

    void from_yaml(const YAML::Node& node);

    typename YAML::Node to_yaml() const;

    OptionBase & required();

    OptionBase & optional();

    OptionBase & describe(std::string description);

protected:
    std::string name_;
    std::string description_;
    std::vector<std::string> path_;
    bool required_ = false;
    ValueBase & value_;
};

template <typename T>
class Option : public OptionBase {
public:

    Option(std::string name, T & value, std::string description="", bool required=false)
    : OptionBase(name, value, description, required) {}


    Option<T> & init(const typename T::ValueType & value) {
        static_cast<T&>(this->value_).set_value(value);
        return *(this);
    }

    Option<T> & validate(std::function<typename T::ValueType(typename T::ValueType)> validator={}) {
        static_cast<T&>(this->value_).set_validator(validator);
        return *(this);
    }

    template <typename... Args>
    Option<T> & dependencies(const Args&&... args) {
        static_cast<T&>(this->value_).set_dependencies(std::forward<const Args>(args)...);
        return *(this);
    }

    Option<T> & required() {
        return static_cast<Option<T>&>(OptionBase::required());
    }

    Option<T> & optional() {
        return static_cast<Option<T>&>(OptionBase::optional());
    }

    Option<T> & describe(std::string description) {
        return static_cast<Option<T>&>(OptionBase::describe(description));
    }

    const typename T::ValueType & get_value() const {
        return static_cast<T&>(this->value_).operator()();
    }

    void set_value(const typename T::ValueType & value) {
        static_cast<T&>(this->value_).set_value(value);
    }

    void operator=(const typename T::ValueType & value) {
        set_value(value);
    }

    const typename T::ValueType & operator()() const {
        return get_value();
    }
};

typedef std::function<bool(std::string name, bool required,
    OptionError error, std::string msg)> option_error_handler;

class OptionList{
public:

    template <typename TValue>
    void add(std::string name, TValue & value, std::string description="", bool required=false) {
        add(Option<TValue>(name, value, description, required));
    }

    template<typename TValue>
    void add(const Option<TValue> & value) {
        
        if (has_option(value.name())) {
            throw std::runtime_error("Option with same name already exists.");
        }

        options_.push_back(value);
    }

    OptionBase& operator[](std::string key);

    void remove(std::string key);

    std::vector<std::string> options() const;

    std::vector<std::string> required_options() const;

    bool has_option(std::string name) const;

    void from_yaml(
        const YAML::Node & node,
        const option_error_handler & handler = {}
    );
    
    YAML::Node to_yaml(
        const option_error_handler & handler = {}
    ) const;

    void load_yaml(std::string filename, const option_error_handler & handler={});

    void save_yaml(std::string filename, const option_error_handler & handler={}) const;

protected:
    std::list<OptionBase> options_;
};

}

#endif //options.hpp