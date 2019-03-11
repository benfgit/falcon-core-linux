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

Below is a list of data types that are currently available in Falcon. See TODO
for more information about how to add new data types.

multi-channel (analog) data
---------------------------

An nsamples-by-nchannels array of data.

event data
----------

An event string.

MUA data
--------

TBD

spike data
----------

TBD
