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

void SetRealtimePriority()
{
  sched_param sch_params;
  sch_params.sched_priority = 99;
  if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sch_params))
  {
    std::cerr << "Failed to set thread scheduling: " << std::strerror(errno) << '\n';
  }
  else
  {
    std::cout << "priority set to " << sch_params.sched_priority << std::endl;
  }

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(3, &cpuset);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
}

void busysleep_until(std::chrono::_V2::system_clock::time_point t)
{
  while (std::chrono::high_resolution_clock::now() < t)
  {
  }
}

DataStreamer::DataStreamer(DataSource *source, double rate, std::string ip,
                           int port, uint64_t npackets)
    : source_(source), rate_(rate), ip_(ip), port_(port),
      max_packets_(npackets)
{

  SetRealtimePriority();

  /*create UDP socket*/
  if ((udp_socket_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    throw std::runtime_error("Unable to create socket.");
  }

  memset((char *)&server_address_, 0, sizeof(server_address_));
  server_address_.sin_family = AF_INET;
  server_address_.sin_addr.s_addr = inet_addr(ip_.c_str());
  server_address_.sin_port = htons(port_);

  if (max_packets_ == 0)
  {
    max_packets_ = std::numeric_limits<uint64_t>::max();
  }

  std::cout << "Streaming to " << ip_ << ":" << port_ << std::endl;
}

DataStreamer::~DataStreamer() { close(udp_socket_); }

bool DataStreamer::running() const { return running_; }

bool DataStreamer::terminated() const { return terminate_; }

void DataStreamer::Terminate() { terminate_ = true; }

void DataStreamer::Run()
{
  float adjustment = 0;

  uint64_t npackets = 0;
  unsigned int buffer_size;
  ssize_t sent;

  std::chrono::_V2::system_clock::time_point start;
  std::chrono::microseconds period((uint64_t)((1000000. / rate_) - adjustment));

  char *buffer;

  std::cout << "Started streaming at " << std::to_string(rate_)
            << " Hz: " << source_->string() << std::endl;

  auto begin_time = std::chrono::high_resolution_clock::now();
  auto last_time = begin_time;
  uint64_t npackets_since_last_time = 0;

  while (!terminated() && npackets < max_packets_)
  {

    start = std::chrono::high_resolution_clock::now();

    auto elapsed = start - last_time;
    if (elapsed > std::chrono::seconds{1})
    {
      std::cout << rate_ << " : " << npackets_since_last_time<< " : " << adjustment
                << std::endl;
      if (npackets_since_last_time < rate_)
      {
        adjustment += 0.1;
      }
      else
      {
        adjustment -= 0.1;
      }
      period = std::chrono::microseconds((uint64_t)((1000000. / rate_) - adjustment));
      last_time = start;
      npackets_since_last_time = 0;
    }

    if ((buffer_size = source_->Produce(&buffer)) == 0)
    {
      break;
    }

    busysleep_until(start + period);

    if ((sent = sendto(udp_socket_, (void *)buffer, buffer_size, 0,
                       (struct sockaddr *)&server_address_,
                       sizeof(server_address_))) != buffer_size)
    {
      if (sent < 0)
      {
        std::cout << "Error sending data packet: " << std::strerror(errno)
                  << std::endl;
      }
      else
      {
        std::cout << "Error sending data packet: did not send all data."
                  << std::endl;
      }
      break;
    }

    npackets++;
    npackets_since_last_time++;
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                      end_time - begin_time)
                      .count();

  std::cout << "Finished streaming: " << source_->string() << std::endl;

  std::cout << "Number of packets sent = " << npackets << " in "
            << duration / 1000. << " seconds ( average rate = "
            << npackets * 1000. / duration
            << " Hz )"
            << std::endl
            << std::endl;

  Terminate();
}

void DataStreamer::Start()
{
  if (!running())
  {
    terminate_ = false;
    thread_ = std::thread(&DataStreamer::Run, this);
    running_ = true;
  }
}

void DataStreamer::Stop()
{
  if (running())
  {
    if (!terminated())
    {
      Terminate();
    }
    thread_.join();
    terminate_ = false;
    running_ = false;
  }
}

void DataStreamer::set_source(DataSource *source)
{
  if (!running())
  {
    source_ = source;
  }
}
