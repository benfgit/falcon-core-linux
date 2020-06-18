#ifndef OPTIONS_UNITS_H
#define OPTIONS_UNITS_H

#include "units/units.hpp"

namespace units {
namespace precise {

constexpr precise_unit sample_units(1., count);
constexpr precise_unit spike_units(1., count);

}
}

#endif