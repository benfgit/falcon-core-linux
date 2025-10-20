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

#include <limits>

#include "muaestimator.hpp"
#include "utilities/general.hpp"

MUAEstimator::MUAEstimator() : IProcessor() {
    add_option(BIN_SIZE, initial_bin_size_);
}

void MUAEstimator::CreatePorts() {
    data_in_port_ = create_input_port<SpikeType>(
        SPIKEDATA, SpikeType::Capabilities(), PortInPolicy(SlotRange(1, 64)));

    data_out_port_ = create_output_port<MUAType>("mua", MUAType::Capabilities(),
                                                 MUAType::Parameters(),
                                                 PortOutPolicy(SlotRange(1)));

    bin_size_ = create_static_state(BIN_SIZE, initial_bin_size_(), true,
                                    Permission::WRITE);

    mua_ = create_broadcaster_state("MUA", 0.0, Permission::READ);
}

void MUAEstimator::CompleteStreamInfo() {
    data_out_port_->streaminfo(0).set_parameters(
        MUAType::Parameters(initial_bin_size_()));
    data_out_port_->streaminfo(0).set_stream_rate(1e3 / initial_bin_size_());
}

void MUAEstimator::Prepare(GlobalContext &context) {
    // check that all incoming SpikeData have the same buffer size
    spike_buffer_size_ = data_in_port_->streaminfo(0).parameters().buffer_size;
    if (data_in_port_->number_of_slots() > 1) {
        for (SlotType s = 1; s < data_in_port_->number_of_slots(); ++s) {
            if (spike_buffer_size_ !=
                data_in_port_->streaminfo(s).parameters().buffer_size) {
                throw ProcessingConfigureError(
                    "Incoming SpikeData buffer-sizes are different.", name());
            }
        }
    }

    try {
        // work around until check_buffer_sizes has been fixed
        double x = initial_bin_size_();
        check_buffer_sizes_and_log(spike_buffer_size_, x, true,
                                   n_spike_buffers_, name());
        initial_bin_size_ = x;
    } catch (std::runtime_error &error) {
        throw ProcessingStreamInfoError(error.what(), name());
    }
    LOG(INFO) << name() << ". MUA will be computed using " << n_spike_buffers_
              << " spike buffers.";

    // TODO if the user doesn't specify a bin_size, use the one from spikedata
    // stream
}

void MUAEstimator::Process(ProcessingContext &context) {
    bool alive = true;
    SpikeType::Data *data_in = nullptr;
    MUAType::Data *data_out = nullptr;
    uint64_t hardware_timestamp = std::numeric_limits<uint64_t>::max();
    std::uint64_t spike_counter;

    while (!context.terminated()) {
        spike_counter = 0;

        current_bin_size_ = bin_size_->get();
        if (current_bin_size_ != previous_bin_size_) {
            try {
                check_buffer_sizes_and_log(spike_buffer_size_,
                                           current_bin_size_, true,
                                           n_spike_buffers_, name());
            } catch (std::runtime_error &error) {
                LOG(ERROR) << name() << ". Invalid buffer size ("
                           << error.what() << ").";
                current_bin_size_ = previous_bin_size_;
                // recompute n_spike_buffers
                check_buffer_sizes_and_log(spike_buffer_size_,
                                           current_bin_size_, true,
                                           n_spike_buffers_, name());
            }
        }

        if (current_bin_size_ != previous_bin_size_) {
            previous_bin_size_ = current_bin_size_;
            LOG(UPDATE) << ". MUA bin updated to " << current_bin_size_
                        << " ms.";
        }

        for (decltype(n_spike_buffers_) n = 0; n < n_spike_buffers_; ++n) {
            for (SlotType s = 0; s < data_in_port_->number_of_slots(); ++s) {
                alive = data_in_port_->slot(s)->RetrieveData(data_in);

                if (!alive) {
                    break;
                }

                if (s == 0) {
                    hardware_timestamp = data_in->hardware_timestamp();
                } else if (data_in->hardware_timestamp() !=
                           hardware_timestamp) {
                    throw ProcessingError("Synchronization error", name());
                }
                spike_counter += data_in->n_detected_spikes();
                data_in_port_->slot(s)->ReleaseData();
            }
        }
        data_out = data_out_port_->slot(0)->ClaimData(false);
        data_out->set_bin_size(current_bin_size_);
        data_out->set_n_spikes(spike_counter);
        data_out->set_hardware_timestamp(hardware_timestamp);
        mua_->set(data_out->mua());
        data_out_port_->slot(0)->PublishData();
    }
}

REGISTERPROCESSOR(MUAEstimator)
