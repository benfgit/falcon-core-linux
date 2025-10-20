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
#include "datastreamer.hpp"

#include <cstring>
#include <iostream>
#include <unistd.h>

#include "utilities/time.hpp"

void SetRealtimePriority() {
    sched_param sch_params;
    sch_params.sched_priority = 99;
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sch_params)) {
        std::cerr << "Failed to set thread scheduling: " << std::strerror(errno) << '\n';
    } else {
        std::cout << "priority set to " << sch_params.sched_priority << std::endl;
    }

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(3, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
}

void busysleep_until(TimePoint t) {
    while (Clock::now() < t) {
    }
}

DataStreamer::DataStreamer(DataSource *source, double rate, std::string ip, int port, uint64_t npackets)
    : source_(source), rate_(rate), ip_(ip), port_(port), max_packets_(npackets) {
    SetRealtimePriority();

    /*create UDP socket*/
    if ((udp_socket_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        throw std::runtime_error("Unable to create socket.");
    }

    memset((char *)&server_address_, 0, sizeof(server_address_));
    server_address_.sin_family = AF_INET;
    server_address_.sin_addr.s_addr = inet_addr(ip_.c_str());
    server_address_.sin_port = htons(port_);

    if (max_packets_ == 0) {
        max_packets_ = std::numeric_limits<uint64_t>::max();
    }

    std::cout << "Streaming to " << ip_ << ":" << port_ << std::endl;
}

DataStreamer::~DataStreamer() { close(udp_socket_); }

bool DataStreamer::running() const { return running_; }

bool DataStreamer::terminated() const { return terminate_; }

void DataStreamer::Terminate() { terminate_ = true; }

void DataStreamer::Run() {

    uint64_t npackets = 0;
    unsigned int buffer_size;
    ssize_t sent;

    TimePoint start;
    std::chrono::microseconds period((uint64_t)(1000000. / rate_));

    char *buffer;

    std::cout << "Started streaming at " << std::to_string(rate_) << " Hz: " << source_->string() << std::endl;

    auto begin_time = std::chrono::high_resolution_clock::now();

    while (!terminated() && npackets < max_packets_) {
        const auto start_time = Clock::now();

        const std::size_t produced_size = source_->Produce(&buffer);
        if (produced_size == 0) {
            break; // No more data to send
        }

        busysleep_until(start_time + period);

        const ssize_t bytes_sent =
            sendto(udp_socket_, static_cast<void *>(buffer), produced_size, 0,
                   reinterpret_cast<struct sockaddr *>(&server_address_), sizeof(server_address_));

        if (bytes_sent != static_cast<ssize_t>(produced_size)) {
            std::cerr << "Error sending data packet: " << (bytes_sent < 0 ? std::strerror(errno) : "Incomplete send.")
                      << std::endl;
            break;
        }

        ++npackets;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count();

    std::cout << "Finished streaming: " << source_->string() << std::endl;

    std::cout << "Number of packets sent = " << npackets << " in " << duration / 1000.
              << " seconds ( average rate = " << npackets * 1000. / duration << " Hz )" << std::endl
              << std::endl;

    Terminate();
}

void DataStreamer::Start() {
    if (!running()) {
        terminate_ = false;
        thread_ = std::thread(&DataStreamer::Run, this);
        running_ = true;
    }
}

void DataStreamer::Stop() {
    if (running()) {
        if (!terminated()) {
            Terminate();
        }
        thread_.join();
        terminate_ = false;
        running_ = false;
    }
}

void DataStreamer::set_source(DataSource *source) {
    if (!running()) {
        source_ = source;
    }
}
