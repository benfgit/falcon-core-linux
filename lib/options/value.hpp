#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <stdexcept>
#include <functional>

#include "yaml-cpp/yaml.h"

namespace options { 

class ValidationError : public std::runtime_error {
public:
    ValidationError( std::string msg ) : std::runtime_error( msg ) {}
};

class ConversionError : public std::runtime_error {
public:
    ConversionError( std::string msg ) : std::runtime_error( msg ) {}
};

template <typename T>
std::function<T(T)> compose(std::function<T(T)> f, std::function<T(T)> g) {
    return [f,g](T x){ return f(g(x)); };
}

template <typename T>
class validator {
public:
    friend auto operator+(std::function<T(T)> lhs, const std::function<T(T)>& rhs) {
        return compose<T>(rhs, lhs);
    }
};

template <typename T>
class inrange : public validator<T> {
public:
    using value_type = T;

public:
    inrange(T min, T max) : min_(min), max_(max) {}

    T operator() (T x) {
        if (x<min_ || x>max_) {
            throw ValidationError("Value out of range.");
        }
        return x;
    }

protected:
    T min_;
    T max_;
};

template <typename T>
class squared : public validator<T> {
public:
    T operator() (T x) {
        return x*x;
    }
};

template <typename T>
class multiplied : public validator<T> {
public:
    multiplied(T multiplier) : multiplier_(multiplier) {}

    T operator() (T x) {
        return multiplier_*x;
    }

protected:
    T multiplier_;

};

class invert : public validator<bool> {
public:
    bool operator() (bool x) {
        return !x;
    }
};

class ValueBase {
public:
    ValueBase() {}
    virtual ~ValueBase() {}

    virtual void from_yaml(const YAML::Node & node) = 0;
    virtual YAML::Node to_yaml() const = 0;
};

template <typename Derived, typename T>
class Value : public ValueBase {
public:

    using ValueType = T;

    template <typename... Args>
    Value(const T & value = T(), std::function<T(T)> validator={},
          const std::tuple<Args...> & dependencies={})
          : value_(value), validator_(validator) {
        from_yaml_func = [this,dependencies](const YAML::Node& node) {
            this->value_ = this->validate(
                std::apply(
                    &Derived::from_yaml_impl,
                    std::tuple_cat(
                        std::make_tuple(static_cast<Derived*>(this), node),
                        dependencies
                    )
                )
            );
        };
    }

    Value(const Value & other) :
        value_(other.value_), validator_(other.validator_),
        from_yaml_func(other.from_yaml_func) {}
     
    
    T validate(T value) {
        if (validator_) {
            return validator_(value);
        }
        return value;
    } 

    T from_yaml_impl(const YAML::Node& node) {
        return node.as<T>();
    }

    virtual void from_yaml(const YAML::Node & node) override final {
        try {
            from_yaml_func(node);
        } catch (YAML::BadConversion &e) {
            throw ConversionError("Could not convert yaml to value (incorrect value type).");
        } catch (YAML::Exception &e) {
            throw ConversionError("Could not convert yaml to value.");
        }
    }

    virtual YAML::Node to_yaml() const override {
        YAML::Node node;
        try {
            node = value_;
        } catch (YAML::Exception &e) {
            throw ConversionError("Could not convert value to yaml.");
        }

        return node;
    }

    const T & get_value() const {
        return value_;
    }

    const T & operator() () const {
        return get_value();
    }

    void set_value(const T & value) {
        value_ = validate(value);
    }

    Value<Derived,T>& operator=(const T & value) {
        set_value(value);
        return (*this);
    }

    Value<Derived,T>& operator=(const Value<Derived,T> & value) {
        set_value(value());
        return (*this);
    }

    void set_validator(std::function<T(T)> validator={}) {
        validator_ = validator;
    }

    template <typename... Args>
    void set_dependencies(const Args&&... args) {
        from_yaml_func = [this,args...](const YAML::Node& node) {
            this->value_ = this->validate(
                static_cast<Derived*>(this)->from_yaml_impl(
                    node, std::forward<const Args>(args)...)
            );
        };
    }


protected:
    T value_;
    std::function<T(T)> validator_;
    std::function<void(const YAML::Node &)> from_yaml_func;
};

class Bool : public Value<Bool,bool> {
public:
    // use base class constructor
    using Value<Bool,bool>::Value;

};

template <typename T>
class Number : public Value<Number<T>,T> {
public:
    // use base class constructor
    using Value<Number<T>,T>::Value;

};

}

#endif //value.hpp