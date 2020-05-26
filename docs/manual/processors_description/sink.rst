Sink
====

DummySink
---------
Take any data stream and eats it. Mainly used to show and test basic graph processing functionality.

Input ports
...........

:data *(IData)*:

States
......

:tickle *(bool)*:
  trigger a log message when changed :
  True = "Hi hi, that tickles!" / False = "Why stop tickling?

  *Initial value* : false

  *Processor connection (state type, peers permission)*: readable, read-only

  *User* : write/read

Expose methods
..............

:kick: ?

----------

FileSerializer
--------------
Serializes data streams to file

Input ports
...........

:data *(IData, 1-256 slots)*:

Options
.......

:path *(string)*:
  server-side path

  *Default value* : "run://"

:encoding *(string)*:
  Only two acceptable keyword: 'binary' or 'yaml'

  *Default value* : "binary"

:format *(string)*:
  Only tree acceptable keyword: 'full', 'nodata', 'compact' (see serializer.hpp for more informations on this mode)

  *Default value* : full

:overwrite *(bool)*:
  overwrite existing file

  *Default value* : False

:throttle *(bool)*:
  throttle saving if we can't keep up

  *Default value* : False

:throttle_threshold *(double)*:
  upstream ringbuffer fill fraction (between 0-1) at which throttling takes effect

  *Default value* : 0.3

:throttle_smooth *(double)*:
  smooth level of throttle level (between 0-1)

  *Default value* : 0.5

----------

ZMQSerializer
-------------
Serializes data streams to cloud

Input ports
...........

:data *(IData, 1-256 slots)*:

Options
.......

:port *(unsigned int)*:

  *Default value* : 7777

:encoding *(string)*:
  Only two acceptable keyword: 'binary' or 'yaml'
  *Default value* : "binary"

:format *(string)*:
  Only tree acceptable keyword: 'full', 'nodata', 'compact' (see serializer.hpp for more informations on this mode)

  *Default value* : full

:interleaved *(bool)*:
  whether data streams from different input slots are interleaved

  *Default value* : False

----------

MUAEstimator
------------
Compute the Multi-Unit Activity from the spike counts provided by the spike detectors and outputs MUAData.

Input ports
...........

:spikes: *type* :ref:`SpikeData` , 1-64 slot

Output port
...........

:mua: *type*: :ref:`MUAData` , 1 slot

Options
.......

:bin_size_ms *(unsigned int)*:

  *Default value* : 10

States:
.......

:bin_size_ms *(unsigned int)*:
  *Initial value* : see options value

  *Processor connection (state type, peers permission)*: readable, read-only

  *User* : write/read

:MUA *(double)*:
  *Initial value* : 0.0

  *Processor connection (state type, peers permission)*: writable, read-only

  *User* : read-only