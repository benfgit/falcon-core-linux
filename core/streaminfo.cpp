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

#include "streaminfo.hpp"

bool IStreamInfo::finalized() const { return finalized_; }

void IStreamInfo::Finalize() { finalized_ = true; }

double IStreamInfo::stream_rate() const { return stream_rate_; }

void IStreamInfo::set_stream_rate(double stream_rate) {
    if (finalized()) {
        throw std::runtime_error(
            "Stream information is finalized. Cannot change stream rate.");
    }
    stream_rate_ = stream_rate;
}

void IStreamInfo::set_stream_rate(const IStreamInfo &info) {
    set_stream_rate(info.stream_rate());
}
