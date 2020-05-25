Detector
========
LevelCrossingDetector
---------------------
Detect a threshold crossing on any of the channels in the incoming MultiChannelData stream and emits an event in response

Input ports
...........

:data *(MultiChannelData<double>, 1 slot)*:
  A stream of data to process

Output port
...........

:events *(EventData, 1 slot)*:
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

BurstDetector
-------------
Detect population bursts using a threshold crossing algorithm.

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
  *Initial value* for threshold multiplier. Units: signal standard deviations.

  *Default value* : 6.*0

:smooth*_tim*e *(double)*:
  *Initial value* for integration time for signal statistics. Must be a positive number.

  *Default value* : 10*.0

:detect*ion_*lockout_time_ms *(double)*:
  *Initial value* for lock-out time. - must be greater than 0 ms.

  *Default value* : 30*.0

:stream*_eve*nts *(bool)*:
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


RippleDetector
--------------
Detect ripples in a MultiChannelData stream and emits an ripple event in response

Input ports
...........

:data *(MultiChannelData<double>, 1 slot)*:
  A stream of data to process

Output port
...........

:events *(EventData, 1 slot)*:
  A stream of 'ripple' events.

:statistics *(MultiChannelData<double>, 1 slot)*:

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

SpikeDetector
-------------

Detect spikes on any of the incoming MultiChannelData stream; sends SpikeData on the output port spikes and an event
"spike"/"spikes" on the output port events if one or more spikes have been detected in the received buffer


Input ports
...........

:data *(MultiChannelData<double>, 1 slot)*:
  A stream of data to process

Output port
...........

:events *(EventData, 1 slot)*:
  A stream of events.

:spikes *(SpikeData, 1 slot)*:
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

  *Processor connection* (state type, peers permission): writable, read-only

  *User*: read/write

:peak_lifetime *(unsigned int)*:
  number of samples that will be used to look for a peak.

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): writable, read-only

  *User*: read/write