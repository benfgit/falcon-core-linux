
BurstDetector
-------------
Detect population bursts using a threshold crossing algorithm.

.. image:: ../../../images/BurstDetector.png

Input ports
...........

:mua: *type* : :ref:`MUAData` , 1 slot

  Binned multi-unit activity in Hz (e.g. from MUAEstimator).

Output ports
............

:events: *type* : :ref:`EventData` , 1 slot

  A stream of 'burst' events.

:statistics: *type*: :ref:`MultiChannelData` <double>, 1 slot

  A stream of nsamples-by-2 arrays with the signal test value (first column)
  and the threshold (second column). The number of samples in each statistics
  data packet is set by the statistics_buffer_size option.

Options
.......

:threshold_dev *(double)*:
  *Initial value* for threshold multiplier. Units: signal standard deviations.

  *Default value* : 6.0

:smooth_time *(double)*:
  *Initial value* for integration time for signal statistics. Must be a positive number (sec).

  *Default value* : 10.0 second

:detection_lockout_time_ms *(double)*:
  *Initial value* for lock-out time. - must be greater than 0 ms.

  *Default value* : 30.0

:stream*_events *(bool)*:
  Whether or not to stream detected burst events.

  *Default value* : True

:stream_statistics *(bool)*:
  Whether or not to stream statistics.

  *Default value* : True

:statistics_buffer_size *(double)*:
  Buffer size (in seconds) for statistics output stream. This value determines
  the number of samples that will be collected for each data packet streamed
  out on the statistics output port. must be either equals or greater than 0.

  *Default value* : 0.5

States
......

:threshold_uv2 *(double)*:
  Current threshold that needs to be crossed.

  *Initial value* : 0.0

  *Processor connection* (state type, peers permission): writable, read-only

  *User* : read-only

:mean_uv2 *(double)*:
  Current signal mean. Units: same as input signal.

  *Initial value* : 0.0

  *Processor connection* (state type, peers permission): writable, read-only

  *User* : read-only

:deviation_uv2 *(double)*:
  Current signal deviation. Units: same as input signal.

  *Initial value* : 0.0

  *Processor connection* (state type, peers permission): writable, read-only

  *User*: read-only

:burst *(bool)*:
  *Initial value* : False

  *Processor connection* (state type, peers permission): writable, read-only

  *User*: read-only

:bin_size *(double)*:
  *Initial value* : 1.0

  *Processor connection* (state type, peers permission): writable, read-only

  *User*: write/read

:threshold_deviation *(double)*:
  Current multiplier for threshold. Units: signal standard deviations.

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User*: write/read

:detection_lockout_time_ms *(unsigned int)*:
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