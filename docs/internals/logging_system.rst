Logging system
==============


Falcon's logging system is based on the
`g3log <https://github.com/KjellKod/g3log>`_ library - version 1.3.3,
which is fetch from the repository at build time.

The logging library is used to provide information about the internal
state and operation to the user (or developer). There are a number of
different types of log messages (i.e "log levels") defined, each with
their own format and usage pattern as listed below:

:DEBUG: debug info
:INFO: general info
:WARNING: general warning
:FATAL: run-time errors info from falcon and other fatal errors info before killing the software.

:STATE: log the state of the graph
:UPDATE: update info on the run-time processor parameters
:ERROR: error info specific to the falcon process

The actual implementation is in the falcon lib logging with the custom sinks and new logging levels.
To log messages in the code, one needs to include the *logging/log.hpp* header file and then do for example:

.. code-block:: c++

    LOG(INFO) << "my informative message";

    LOG_IF(DEBUG, condition) << "If [true], then this text will be logged";

Log level for falcon can be added in the logging/g3loglevels. Other log levels specifics to an extension can be added
in the extension repository in a similar way.

In Falcon, three destinations ("sinks") for log messages are defined.
This custom sink are developped in logging/customsink.hpp.

First, log messages are always saved to a log file. The path of this file
is set using the *logging.path* configuration option
(see :ref:`manual-configuration`). Second, log messages are displayed in
the terminal in which Falcon was started (but only if the
*logging.screen.enabled* configuration option is set to true).
Finally, log messages are broadcast to clients using a ZMQ publisher network
socket (if the *logging.cloud.enabled* configuration option is true). The
network port is configurable (see :ref:`manual-configuration`).

Here is an example in Python how to receive log messages broadcast to
port 5556 on the local computer:

.. code-block:: python

    import zmq

    context = zmq.Context()
    socket = context.socket( zmq.SUB )

    socket.connect( "tcp://localhost:5556" )

    socket.setsockopt(zmq.SUBSCRIBE, "")

    while True:
        message = socket.recv_multipart()
        print( message )


