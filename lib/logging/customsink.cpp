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
#include <iostream>
#include <string>

#include "customsink.hpp"
#include "../utilities/zmqutil.hpp"

std::string ScreenSink::FormatMessage(g3::LogMessage &msg) {
  if (msg.level() == "DEBUG") {
    std::string out;
    out.append("\n" + msg.timestamp() + "\t" + msg.level() + " [" + msg.file() +
               " L: " + msg.line() + "]\t");
    out.append(msg.message());
    return out;
  } else if (msg.wasFatal()) {
    return "";
  } else {
    std::string out;
    out.append("\n" + msg.timestamp() + "\t" + msg.level() + "\t" +
               msg.message());
    return out;
  }
}

void ScreenSink::ReceiveLogMessage(g3::LogMessageMover message) {
  std::cout << FormatMessage(message.get()) << std::flush;
}

ZMQSink::ZMQSink(zmq::context_t &context, int port) {
  // set up PUB
  publisher = new zmq::socket_t(context, ZMQ_PUB);

  char buffer[15];
  snprintf(buffer, 14, "tcp://*:%d", port);
  publisher->bind(buffer);
}

ZMQSink::~ZMQSink() {
  // clean up PUB
  delete publisher;
}

std::deque<std::string> ZMQSink::FormatMessage(g3::LogMessage &msg) {
  std::deque<std::string> out;
  out.push_back(msg.level());
  out.push_back(msg.timestamp());

  if (!msg.wasFatal()) {
    out.push_back(msg.message());
    if (msg.level() == "DEBUG") {
      out.push_back("[" + msg.file() + " L: " + msg.line() + "]");
    }
  }
  return out;
}

void ZMQSink::ReceiveLogMessage(g3::LogMessageMover message) {
    zmq_frames msg = FormatMessage(message.get());
    s_send_multi(*publisher, msg);
}
