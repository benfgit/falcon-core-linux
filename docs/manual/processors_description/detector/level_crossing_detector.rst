LevelCrossingDetector
=====================

Detect a threshold crossing on any of the channels in the incoming MultiChannelData stream and emits an event in response

.. image:: ../../../images/LevelCrossingDetector.png

Input ports
...........

:data:
  *type*: :ref:`MultiChannelData` <double> , 1 slot

Output port
...........

:events: *type*: :ref:`EventData` , 1 slot

  A stream of events.

Options
.......

:threshold *(double)*:
  threshold that needs to be crossed

  *Default value*: 0.0

:upslope *(bool)*:
  whether to look for upward (true) or downward (false) threshold crossings

  *Default value*: True

:post_detect_block *(unsigned int)*:
  refractory period after threshold crossing detection (in number of samples )

  *Default value*: 2

:event *(string)*:
  event to emit upon detection of threshold crossing

  *Default value* : "threshold_crossing"

States
......

:threshold *(double)*:
  threshold that needs to be crossed

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User*: write/read

:upslope *(bool)*:
  whether to look for upward (true) or downward (false) threshold crossings

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User*: write/read

:post_detect_block *(unsigned int)*:
  refractory period after threshold crossing detection (in number of samples )

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User*: write/read