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

#include <chrono>
#include <string>
#include <vector>

#include "dummysink.hpp"
#include "idata.hpp"
#include "multichanneldata/multichanneldata.hpp"
#include "utilities/time.hpp"

void DummySink::CreatePorts() {
    data_port_ = create_input_port<AnyType>("data", AnyType::Capabilities(),
                                            PortInPolicy(SlotRange(1)));

    tickle_state_ =
        create_static_state("tickle", false, true, Permission::WRITE);
    expose_method("kick", &DummySink::Kick);
}

YAML::Node DummySink::Kick(const YAML::Node &node) {
    LOG(INFO) << name() << " says: I got kicked!";
    return YAML::Node();
}

void DummySink::Process(ProcessingContext &context) {
    uint64_t packet_counter = 0;
    uint64_t retrieve_counter = 0;
    std::vector<AnyType::Data *> data;
    auto address = data_port_->slot(0)->upstream_address();

    LOG(DEBUG) << "slot is connected to " << address.string();

    bool eos = false;
    auto start = Clock::now();
    bool tickling = false;

    while (!context.terminated()) {
        if (!data_port_->slot(0)->RetrieveDataAll(data)) {
            LOG(DEBUG) << name()
                       << " : received finish signal while waiting for data!";
            break;
        }

        for (auto &it : data) {
            if (it->eos()) {
                LOG(DEBUG) << name() << " received end of stream signal.";
                eos = true;
                break;
            }
        }

        if (eos) {
            break;
        }

        ++retrieve_counter;
        packet_counter += data.size();
        data_port_->slot(0)->ReleaseData();

        if (tickle_state_->get() != tickling) {
            tickling = !tickling;
            if (tickling) {
                LOG(INFO) << "Hi hi, that tickles!";
            } else {
                LOG(INFO) << "Why stop tickling?";
            }
        }
    }

    std::chrono::milliseconds runtime(
        std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() -
                                                              start));

    LOG(UPDATE) << name() << ": retrieved " << packet_counter << " packets in "
                << retrieve_counter << " batches over "
                << static_cast<double>(runtime.count()) / 1000.0 << "seconds ("
                << static_cast<double>(packet_counter) / retrieve_counter
                << " packets/batch and "
                << packet_counter /
                       (static_cast<double>(runtime.count()) / 1000.0)
                << " packets/second).";
}

REGISTERPROCESSOR(DummySink)
