Processors
==========

BurstDetector
-------------

Detects population bursts using a threshold crossing algorithm.

Input ports
...........

:mua *(MUAData, 1 slot)*:
  Binned multi-unit activity in Hz (e.g. from MUAEstimator).

Output ports
............

:events *(EventData, 1 slot)*:
  A stream of 'burst' events.

:statistics *(MultiChannelData<double>, 1 slot)*:
  A stream of nsamples-by-2 arrays with the signal test value (first column)
  and the threshold (second column). The number of samples in each statistics
  data packet is set by the statistics_buffer_size option.

Options
.......

:threshold_dev *(double)*:
  Initial value for threshold multiplier. Units: signal standard deviations.
  Default = 6.0

:smooth_time *(double)*:
  Initial value for integration time for signal statistics.
  Default = 10.0

:detection_lockout_time_ms *(double)*:
  Initial value for lock-out time.
  Default = 30.0

:stream_events *(bool)*:
  Whether or not to stream detected burst events.
  Default = true

:stream_statistics *(bool)*:
  Whether or not to stream statistics.
  Default = true

:statistics_buffer_size *(double)*:
  Buffer size (in seconds) for statistics output stream. This value determines
  the number of samples that will be collected for each data packet streamed
  out on the statistics output port. Default = 0.5


States
......

:threshold_dev *(double, read-only)*:
  Current threshold that needs to be crossed.

:mean *(double, read-only)*:
  Current signal mean. Units: same as input signal.

:deviation *(double, read-only)*:
  Current signal deviation. Units: same as input signal.

:threshold_deviation *(double, read/write)*:
  Current multiplier for threshold. Units: signal standard deviations.

:detection_lockout_time_ms *(unsigned int, read/write)*:
  Current refractory period following threshold crossing that is not
  considered for updating signal statistics and for event detection.

:stream_events *(bool, read/write)*:
  Current flag for streaming events.

:stream_statistics *(bool, read/write)*:
  Current flag for streaming statistics.

DigitalOutput
-------------

DummySink
---------

EventFilter
-----------

EventSink
---------

EventSource
-----------

EventSync
---------

FileSerializer
--------------

LevelCrossingDetector
---------------------

MUAEstimator
------------

MultiChannelFilter
------------------

NlxReader
---------

Rebuffer
--------

RippleDetector
--------------

RunningStats
------------

SpikeDetector
-------------

ZMQSerializer
-------------
