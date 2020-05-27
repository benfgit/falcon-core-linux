RippleDetector
==============

Detect ripples in a MultiChannelData stream and emits an ripple event in response

Input ports
...........

:data: *type*: :ref:`MultiChannelData` <double>, 1 slot

Output port
...........

:events: *type*: :ref:`EventData` , 1 slot

  A stream of 'ripple' events.

:statistics: *type*: :ref:`MultiChannelData` <double>, 1 slot

Options
.......

:threshold_dev *(double)*:
  threshold that needs to be crossed

  *Default value*: 6.0

:smooth_time *(double)*:
  integration time for signal statistics. Must be a positive number.

  *Default value*: 10.0

:detection_lockout_time_ms *(double)*:
  refractory period after threshold crossing detection that is not considered for updating of statistics
  and for detecting events. Must greater than 0 ms.

  *Default value*: 30

:stream_events *(bool)*:
  enable/disable ripple event output

  *Default value*: True

:stream_statistics *(bool)*:
  enable/disable streaming of ripple detection statistics

  *Default value*: True

:statistics_buffer_size *(double)*:
  Buffer size (in seconds) for statistics output buffers. Should be equal larger than zero.

  *Default value*: 0.5 second

:statistics_downsample_factor *(unsigned int)*:
  downsample factor of streamed statistics signal. Should larger than zero.

  *Default value*: 1

:use_power *(bool)*:
  *Default value*: True

States
......

:threshold *(double)*:
  Current threshold that needs to be crossed.

  *Initial value* : 0.0

  *Processor connection* (state type, peers permission): writable, None

  *User* : read-only

:mean *(double)*:
  Current signal mean. Units: same as input signal.

  *Initial value* : 0.0

  *Processor connection* (state type, peers permission): writable, None

  *User* : read-only

:deviation *(double)*:
  Current signal deviation. Units: same as input signal.

  *Initial value* : 0.0

  *Processor connection* (state type, peers permission): writable, None

  *User* : read-only


:threshold_dev *(double)*:
  Current multiplier for threshold. Units: signal standard deviations.

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User*: write/read

:detection_lockout_time_ms *(double)*:
  Current refractory period following threshold crossing that is not
  considered for  updating signal statistics and for event detection.

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User*: write/read

:stream_events *(bool)*:
  Current flag for streaming events.

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User*: write/read

:stream_statistics *(bool)*:
  Current flag for streaming statistics.

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User*: write/read

:smooth_time *(double)*:
  integration time for signal statistics. Must be a positive number.

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User*: write/read

:ripple *(bool)*:
  *Initial value* : False

  *Processor connection* (state type, peers permission): writable, read-only

  *User* : read-only