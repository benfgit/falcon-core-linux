Data Types
==========

The input and output ports on processor nodes only handle dedicated data
types. To be able to connect an output port to an input port, their data
types need to be compatible. For example, a processor that generates event
data cannot be connected to a processor that expects multi-channel analog
data. Falcon will make sure that a connection is valid before constructing
the data flow graph.

Data types in Falcon form a hierarchy from generic to specific. At the top of
the hierarchy is the most generic data type "IData" that is the base for all
other data types. As long as the data type of an input port is the same or
more generic than the data type of the upstream output port, a connection can
be made. Thus, a processor node with an input port that expects the most
generic IData type, can handle incoming data streams of any other type.

Below is a list of data types that are currently available in Falcon. See :ref:`extend_data_type`
for more information about how to add new data types.

----------

.. _EventData:

EventData
---------
An event string. - datatype = "event"

Input parameters
................

event type (optional, default = "none")

Default values
..............

- default port/state name: "events"

----------

.. _spikedata:

SpikeData
---------

Input parameters
................

- buffer size (optional, default = 0.)
- number of channel (optional, default = 0)
- rate (optional, default = 0.)

Default value
.............

- default port/state name: "spikes"
- default buffer size = 12.75 ms
- default maximum number of channels for spike detection : 16
- default number of spikes in the buffer : 100

----------

.. _multichanneldata:

MultiChannelData
----------------

An nsamples-by-nchannels array of data.  - datatype = "multichannel"

Input parameters
................

- number of channel (optional, default = 0)
- number of samples (optional, default = 0)
- rate (optional, default = 1.0)

----------

.. _MUAData:

MUAData
-------
MultiUnit Activity (number of spike / size of the bin * 1e3) - datatype = "mua"

Input parameters
................

bin size (ms) : (optional, default = 0)

Others
......
the number of spike can be set separately.

----------

.. _scalardata:

ScalarData
----------
an unique scalar data - datatype = "scalar"

Input parameters
................

value : (optional, default = 0)

----------

.. _vectordata:

VectorData
----------

create a vector of data - datatype = "vector"

Input parameters
................

size of the vector

Others
......
the data are set separately, once the vector data has been created by allocating in memory the right size of data needed.


