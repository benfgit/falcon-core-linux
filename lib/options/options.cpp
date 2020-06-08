#include "options.hpp"

using namespace options;

bool options::get_nested_yaml_node(
    const YAML::Node& root,
    const std::vector<std::string>& path,
    YAML::Node & out) {
    
    out.reset(root);

    for (auto & p : path) {
        if (out[p]) {
            out.reset(out[p]);
        } else {
            return false;
        }
    }

    return true;
}



OptionBase::OptionBase(std::string name, ValueBase & value, std::string description, bool required)
: name_(name), description_(description), required_(required), value_(value) {
    
    if (name.size()==0) {
        throw std::runtime_error("Option name cannot be empty.");
    }

    path_ = split(name, '/');
}

std::string OptionBase::name() const {
    return name_;
}

std::string OptionBase::description() const {
    return description_;
}

const std::vector<std::string> & OptionBase::path() const {
    return path_;
}

bool OptionBase::is_required() const {
    return required_;
}

void OptionBase::from_yaml(const YAML::Node& node) {
    value_.from_yaml(node);
}

typename YAML::Node OptionBase::to_yaml() const {
    YAML::Node node;

    //node[name_] = value_.to_yaml();
    node = value_.to_yaml();

    return node;
}

OptionBase & OptionBase::required() {
    required_ = true;
    return *(this);
}

OptionBase & OptionBase::optional() {
    required_ = false;
    return *(this);
}

OptionBase & OptionBase::describe(std::string description) {
    description_ = description;
    return *(this);
}


OptionBase& OptionList::operator[](std::string key) {
    for (auto & option : options_) {
        if (option.name()==key) {
            return option;
        }
    }
    throw std::runtime_error("No such option.");
}

void OptionList::remove(std::string key) {
    options_.remove_if([key](const OptionBase & x) {return x.name()==key;} );
}

std::vector<std::string> OptionList::options() const {
    
    std::vector<std::string> opts;
    opts.reserve(options_.size());

    for (auto & option : options_) {
        opts.push_back(option.name());
    }

    return opts;
}

std::vector<std::string> OptionList::required_options() const {
    std::vector<std::string> opts;

    for (auto & option : options_) {
        if (option.is_required()) {
            opts.push_back(option.name());
        }
    }

    return opts;
}

bool OptionList::has_option(std::string name) const {
    return std::any_of(
        options_.begin(), options_.end(),
        [name](const OptionBase & x) {
            return x.name()==name;
            }
        );
}

void OptionList::from_yaml(
    const YAML::Node & node,
    const option_error_handler & handler) {
    
    if (not node.IsMap()) {
        throw std::runtime_error("Expecting YAML map.");
    }

    YAML::Node x;

    // loop through options
    for (auto & option : options_) {
        // check if available in YAML node
        // treat "/" in option name special (e.g. recurse into maps)
        if (get_nested_yaml_node(node, option.path(), x)) {
            try {
                option.from_yaml(x);
            } catch (ConversionError & e) {
                if (!handler || !handler(option.name(), option.is_required(), OptionError::conversion_from_yaml_failed, e.what())) {
                    throw std::runtime_error("Error setting option " + option.name() + ": " + e.what());
                }
            } catch (ValidationError & e) {
                if (!handler || !handler(option.name(), option.is_required(), OptionError::validation_failed, e.what())) {
                    throw std::runtime_error("Error setting option " + option.name() + ": " + e.what());
                }
            }
        } else if (option.is_required()) {
            if (!handler || !handler(option.name(), option.is_required(), OptionError::requirement_failed, "")) {
                throw std::runtime_error("Missing required option " + option.name() + ".");
            }
        }
    }
}

YAML::Node OptionList::to_yaml(const option_error_handler & handler) const {

    YAML::Node root = YAML::Node(YAML::NodeType::Map);
    YAML::Node x;

    for (auto & option : options_) {
        if (option.path().size()==0) {
            // do nothing
        } else if (option.path().size()==1) {
            try {
                root[option.path()[0]] = option.to_yaml();
            } catch (ConversionError & e) {
                if (handler && !handler(option.name(), option.is_required(), OptionError::conversion_to_yaml_failed, e.what())) {
                    throw std::runtime_error("Error exporting option " + option.name() + ": " + e.what());
                }
            } catch (...) {
                throw std::runtime_error("Unknown error for option " + option.name());
            }
        } else {
            x.reset(root);
            
            for (auto it = option.path().begin(); it != option.path().end(); ++it) {

                if (std::next(it)==option.path().end()) {
                    try {
                        x[*it] = option.to_yaml();
                    } catch (ConversionError & e) {
                        if (handler && !handler(option.name(), option.is_required(), OptionError::conversion_to_yaml_failed, e.what())) {
                            throw std::runtime_error("Error exporting option " + option.name() + ": " + e.what());
                        }
                    } catch (...) {
                        throw std::runtime_error("Unknown error for option " + option.name());
                    }
                } else if (!x[*it]) {
                    x[*it] = YAML::Node(YAML::NodeType::Map);
                }

                x.reset(x[*it]);
                
            }
        }
        
    }

    return root;
}

void OptionList::load_yaml(std::string filename, const option_error_handler & handler) {
    
    YAML::Node root = YAML::LoadFile(filename);
    from_yaml(root, handler);

}

void OptionList::save_yaml(std::string filename, const option_error_handler & handler) const {

    auto node = to_yaml(handler);

    YAML::Emitter yaml_emitter;
    yaml_emitter << node;

    std::ofstream out(filename);
    out << yaml_emitter.c_str();

}
