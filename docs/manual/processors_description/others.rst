Others
======

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