#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <stdexcept>
#include <functional>
#include <optional>
#include <type_traits>

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
using ValidatorFunc = std::function<T(const T &)>;

template <typename T>
ValidatorFunc<T> compose(ValidatorFunc<T> f, ValidatorFunc<T> g) {
    return [f,g](T x){ return f(g(x)); };
}

template <typename T>
class validator {
public:
    friend auto operator+(const ValidatorFunc<T>& lhs, const ValidatorFunc<T>& rhs) {
        return compose<T>(rhs, lhs);
    }
};

template <typename T>
class inrange : public validator<T> {

static_assert(std::is_arithmetic<T>::value, "inrange validator only supports numerical types.");

public:
    using value_type = T;

public:
    inrange(T min, T max) : min_(min), max_(max) {}

    T operator() (const T & x) const {
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
class clamped : public validator<T> {

static_assert(std::is_arithmetic<T>::value, "clamped validator only supports numerical types.");

public:
    using value_type = T;

public:
    clamped(T min, T max) : min_(min), max_(max) {}

    T operator() (const T & x) const {
        T y = x;
        std::clamp(y, min_, max_);
        return y;
    }

protected:
    T min_;
    T max_;
};

template <typename T>
class squared : public validator<T> {

static_assert(std::is_arithmetic<T>::value, "squared validator only supports numerical types.");

public:
    T operator() (const T & x) const {
        return x*x;
    }
};

template <typename T>
class multiplied : public validator<T> {

static_assert(std::is_arithmetic<T>::value, "multiplier validator only supports numerical types.");

public:
    multiplied(T multiplier) : multiplier_(multiplier) {}

    T operator() (const T & x) const {
        return multiplier_*x;
    }

protected:
    T multiplier_;

};

class invert : public validator<bool> {
public:
    bool operator() (const bool & x) const {
        return !x;
    }
};

template <typename T>
class notempty : public validator<T> {

public:
    T operator() (const T & x) {
        if (x.size()==0) {
            throw ValidationError("Container or value cannot be empty.");
        }
        return x;
    }
};

template <typename T>
class each : public validator<std::vector<T>> {
public:
    each(ValidatorFunc<T> validator) : validator_(validator) {}

    std::vector<T> operator() (const std::vector<T> & x) {
        std::vector<T> y = x;
        std::transform(x.begin(), x.end(), y.begin(), validator_);
        return x;
    }
protected:
    ValidatorFunc<T> validator_;
};

template <typename T>
class positive : public validator<T> {
public:

    positive(bool strict=false) : strict_(strict) {}

    T operator() (const T & x) const {
        if ((strict_ && x<=0) || x<0) {
            throw ValidationError("Value cannot be negative" + strict_ ? " or zero." : ".");
        }
        return x;
    }

protected:
    bool strict_;
};

class ValueBase {
public:
    ValueBase(){}
    virtual ~ValueBase() {}

    virtual void from_yaml(const YAML::Node & node) = 0;
    virtual YAML::Node to_yaml() const = 0;

    virtual bool is_nullable() const = 0;
    
    virtual bool is_null() const = 0; 
    virtual void set_null() = 0;

protected:
    virtual void unset_null() = 0;

};

template <typename T>
T generic_fromyaml(const YAML::Node & node) {
    return node.as<T>();
};

template <typename T>
YAML::Node generic_toyaml(const T& x) {
    
    YAML::Node node;
    node = x;
    
    return node;
};

template <typename T, bool Nullable=true>
class Value : public ValueBase {
public:
    using ValueType = T;
    using ValidatorType = ValidatorFunc<T>;
    using ToYAMLType = std::function<YAML::Node(const T&)>;
    using FromYAMLType = std::function<T(const YAML::Node &)>;

    Value(const T& value, ValidatorType validator={},
           ToYAMLType toyaml=generic_toyaml<T>, FromYAMLType fromyaml=generic_fromyaml<T>)
    : ValueBase(), validator_(validator), toyaml_(toyaml), fromyaml_(fromyaml) {
        set_value(value);
    }

    Value(ValidatorType validator={},
           ToYAMLType toyaml=generic_toyaml<T>, FromYAMLType fromyaml=generic_fromyaml<T>)
    : ValueBase(), validator_(validator), toyaml_(toyaml), fromyaml_(fromyaml) {
        if constexpr (!Nullable) {
            set_value(T());
        }
    }


    T validate(T value) {
        if (validator_) {
            return validator_(value);
        }
        return value;
    } 

    virtual void from_yaml(const YAML::Node & node) override final {
        try {
            set_value(fromyaml_(node));
        } catch (YAML::BadConversion &e) {
            throw ConversionError("Could not convert yaml to value (incorrect value type).");
        } catch (YAML::Exception &e) {
            throw ConversionError("Could not convert yaml to value.");
        }
    }

    virtual YAML::Node to_yaml() const override final {
        YAML::Node node;

        if (is_null()) {
            return node;
        }

        try {
            node = toyaml_(value_);
        } catch (YAML::Exception &e) {
            throw ConversionError("Could not convert value to yaml.");
        }

        return node;
    }

    const T & get_value() const {
        return value_;
    }

    const T & operator() () const {
        if constexpr (Nullable) {
            if (!is_null()) {
                throw std::runtime_error("Value has not been set.");
            }
        }
        return get_value();
    }

    void set_value(const T & value) {
        value_ = validate(value);
        if constexpr (Nullable) {
            unset_null();
        }
    }

    Value<T,Nullable>& operator=(const T & value) {
        set_value(value);
        return (*this);
    }

    Value<T,Nullable>& operator=(const Value<T> & value) {
        set_value(value());
        return (*this);
    }

    void set_validator(ValidatorType validator={}) {
        validator_ = validator;
    }

    virtual bool is_nullable() const override final {
        return Nullable;
    }    
    
    virtual bool is_null() const override final {
        if constexpr(Nullable) {
            return value_is_null_;
        } else {
            throw std::runtime_error("Value::is_null : value is not nullable.");
        }
    }

    virtual void set_null() override final {
        if constexpr(Nullable) {
            value_is_null_ = true;
        } else {
            throw std::runtime_error("Value::set_null : value is not nullable.");
        }
    }

protected:
    virtual void unset_null() override final {
        if constexpr(Nullable) {
            value_is_null_ = false;
        } else {
            throw std::runtime_error("Value::unset_null : value is not nullable.");
        }
    }

private:
    T value_;
    ValidatorType validator_;
    ToYAMLType toyaml_;
    FromYAMLType fromyaml_;
    bool value_is_null_ = true;
};

using Bool = Value<bool, false>;
using NullableBool = Value<bool, true>;

using Double = Value<double, false>;
using NullableDouble = Value<double, true>;

using Int = Value<int, false>;
using NullableInt = Value<int, true>;

using String = Value<std::string, false>;
using NullableString = Value<std::string, true>;

template <typename T, bool Nullable=false>
class ConstrainedValue : public Value<T, Nullable> {

static_assert(std::is_arithmetic<T>::value, "ConstrainedValue only supports numerical types.");

public:
    ConstrainedValue(T min, T max, T value)
    : Value<T,Nullable>(value, inrange<T>(min, max)) {}

    ConstrainedValue(T min, T max)
    : Value<T,Nullable>(inrange<T>(min, max)) {}

    ConstrainedValue<T,Nullable>& operator=(const T & value) {
        return static_cast<ConstrainedValue<T,Nullable>&>(Value<T,Nullable>::operator=(value));
    }

};

template <typename T, bool Nullable=false>
class ClampedValue : public Value<T, Nullable> {

static_assert(std::is_arithmetic<T>::value, "ClampedValue only supports numerical types.");

public:
    ClampedValue(T min, T max, T value)
    : Value<T,Nullable>(value, clamped<T>(min, max)) {}

    ClampedValue(T min, T max)
    : Value<T,Nullable>(clamped<T>(min, max)) {}

    ClampedValue<T,Nullable>& operator=(const T & value) {
        return static_cast<ClampedValue<T,Nullable>&>(Value<T,Nullable>::operator=(value));
    }
};

template <typename T>
std::vector<T> vector_fromyaml(const YAML::Node & node) {
    if (node.IsSequence()) {
        return node.as<std::vector<T>>();
    } else if (node.IsNull()) {
        return std::vector<T>();
    } else {
        return std::vector<T>({node.as<T>()});
    }
};

template <typename T, bool Nullable=false>
class Vector : public Value<std::vector<T>, Nullable> {
public:

    Vector(const std::vector<T> & value={}, ValidatorFunc<std::vector<T>> validator={}) :
    Value<std::vector<T>,Nullable>(
        value,
        validator,
        &generic_toyaml<std::vector<T>>,
        &vector_fromyaml<T>) {}
};


}

#endif //value.hpp