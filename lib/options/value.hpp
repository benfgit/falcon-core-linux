#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <stdexcept>
#include <functional>

#include "yaml-cpp/yaml.h"

#include "units/units.hpp"
#include "validation.hpp"

namespace options { 

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

        if (is_nullable() && is_null()) {
            return node;
        }

        try {
            node = toyaml_(value_);
        } catch (YAML::Exception &e) {
            throw ConversionError("Could not convert value to yaml.");
        }

        return node;
    }

    virtual 

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

    void set_toyaml(const ToYAMLType & toyaml) {
        toyaml_ = toyaml;
    }

    void set_fromyaml(const FromYAMLType & fromyaml) {
        fromyaml_ = fromyaml;
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


template <typename T>
class measurement_toyaml {
public:
    measurement_toyaml(units::precise_unit u) : units_(u) {}

    YAML::Node operator() (const T & x) {
        YAML::Node node;

        node = std::to_string(x) + " " + units::to_string(units_);
        
        return node;
    } 

protected:
    units::precise_unit units_;
};

template <typename T>
class measurement_fromyaml {
public:
    measurement_fromyaml(units::precise_unit u) : units_(u) {}

    T operator() (const YAML::Node & node) {
        std::string s = node.as<std::string>();
        auto m = units::measurement_from_string(s);
        
        if (!units_.has_same_base(m.units())) {
            throw std::runtime_error("Incorrect units. Not same base.");
        }
        
        double value = m.value_as(units_);

        if (std::isnan(value)) {
            throw std::runtime_error("Incorrect units: NaN");
        }

        return T(value);
    }

protected:
    units::precise_unit units_;
};

template <typename T, bool Nullable=false>
class Measurement : public Value<T, Nullable> {
public:
    Measurement(T value, units::precise_unit u=units::precise::one, ValidatorFunc<T> validator={})
    : Value<T, Nullable>(
        value,
        validator,
        measurement_toyaml<T>(u),
        measurement_fromyaml<T>(u)), units_(u) {}

    void set_units(const units::precise_unit & u) {
        units_ = u;
        this->set_fromyaml(measurement_fromyaml<T>(u));
        this->set_toyaml(measurement_toyaml<T>(u));
    }

    units::precise_unit unit() const {
        return units_;
    }

    std::string to_string(std::string dest_unit="") {
        if (!dest_unit.size()==0) {
            auto du = units::unit_from_string(dest_unit);
            T v = units::precise_measurement(this->get_value(), units_).value_as(du);
            return (std::to_string(v) + " " + dest_unit);
        } else {
            return (std::to_string(this->get_value()) + " " + units::to_string(units_));
        }
    }

protected:
    units::precise_unit units_;

};

template <typename T, bool Nullable>
class MultiMeasurement : public Measurement<T,Nullable> {
public:
    MultiMeasurement(T value, units::precise_unit u, std::vector<units::precise_unit> alt_u={}, ValidatorFunc<T> validator={})
    : Measurement<T,Nullable>(value, u, validator), default_units_(u), alt_units_(alt_u) {
        this->set_fromyaml([this](const YAML::Node & node){return this->fromyaml_impl(node);});
        //this->set_fromyaml(&this->fromyaml_impl);
    }

    T fromyaml_impl(const YAML::Node & node) {
        std::string s = node.as<std::string>();
        auto m = units::measurement_from_string(s);

        if (m.units()==units::precise::invalid || m.units()==units::precise::error) {
            throw std::runtime_error("Could not parse measurement.");
        }

        if (m.units().has_same_base(units::precise::one) ||
            m.units().has_same_base(default_units_)) {
            // default unit
            this->set_units(default_units_);
            return T(m.value_as(default_units_));
        } else {

            double value = std::nan("");
            bool units_matched = false;

            for (auto & u : alt_units_) {
                if (!u.has_same_base(m.units())) {
                    continue;
                }

                units_matched = true;

                value = m.value_as(u);
                this->set_units(u);

                //if (std::isnan(value)) {
                //    throw std::runtime_error("Incorrect units: NaN");
                //}
                break;
            }   

            if (!units_matched) {
                throw std::runtime_error("Measurement in unsupported units " + units::to_string(m.units()) + ".");
            }      

            return T(value);
        }
    }


protected:
    units::precise_unit default_units_;
    std::vector<units::precise_unit> alt_units_;
}; 

}

#endif //value.hpp