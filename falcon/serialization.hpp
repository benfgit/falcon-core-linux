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

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <string>

namespace Serialization {

static const uint8_t VERSION = 1;

enum class Format { NONE=-1, FULL, COMPACT, HEADERONLY, STREAMHEADER };
// NONE: no packet header, no data header, no data
// FULL: packet header, data header and data
// HEADERONLY: packet header, data header, no data
// STREAMHEADER: packet header, no data header, no data
// COMPACT: data only

std::string format_to_string( Format fmt );

Format string_to_format( std::string s );

} // namespace Serialization 

#endif //serialization.hpp
