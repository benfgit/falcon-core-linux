
SpikeDetector
=============

Detect spikes on any of the incoming MultiChannelData stream; sends SpikeData on the output port spikes and an event
"spike"/"spikes" on the output port events if one or more spikes have been detected in the received buffer


Input ports
...........

:data: *type*: :ref:`MultiChannelData` <double>, 1 slot


Output port
...........

:events: *type*: :ref:`EventData` , 1 slot

  A stream of events.

:spikes: *type* :ref:`SpikeData` , 1 slot

  A stream of detected spikes


Options
.......

:threshold *(double)*:
  threshold that a single channel must cross.

  *Default value*: 60.0

:invert_signal *(bool)*:
  whether the signal does (true) or does not (false) need to be inverted when detecting spikes

  *Default value*: True

:buffer_size *(double)*:
  amount of data that will be used to look for spikes [ms]

  *Default value*: 0.5 second

:peak_lifetime *(unsigned int)*:
  number of samples that will be used to look for a peak

  *Default value*: 8

:strict_time_bin_check *(bool)*:
  *Default value*: True

States
......

:threshold *(double)*:
  Current threshold that needs to be crossed.

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User*: read/write

:peak_lifetime *(unsigned int)*:
  number of samples that will be used to look for a peak.

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User*: read/write