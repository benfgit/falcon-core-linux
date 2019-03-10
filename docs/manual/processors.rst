Processors
==========

BurstDetector
-------------

Detects population bursts using a threshold crossing algorithm.

Slots
.....

========= ====== ========= =====
direction name   type      slots
========= ====== ========= =====
in        mua    MUAData   1
out       events EventData 1
========= ====== ========= =====

States
......

========================= ============ ====== ==========================================
state                     type         access description
========================= ============ ====== ==========================================
threshold_dev             double       read   threshold that needs to be crossed
mean                      double       read   signal mean
deviation                 double       read   signal deviation
threshold_deviation       double       write  multiplier for threshold (# of deviations)
detection_lockout_time_ms unsigned int write  refractory period after threshold crossing
                                              detection that is not considered for
                                              updating of statistics and for detecting
                                              events
stream_events             bool         write  enable/disable burst event output
stream_statistics         bool         write  enable/disable streaming of burst
                                              detection statistics
========================= ============ ====== ==========================================

Options
.......

============================ ============ ==========================================
options                      type         description
============================ ============ ==========================================
threshold_dev                double       default threshold multiplier
smooth_time                  double       integration time for signal statistics
detection_lockout_time_ms    double       default lock-out time
stream_events                bool         default enable/disable burst event output
stream_statistics            bool         default enable/disable streaming of burst
                                          detection statistics
statistics_buffer_size       double       buffer size (in seconds) for statistics
                                          output buffers
statistics_downsample_factor unsigned int downsample factor of streamed
                                          statistics signal
============================ ============ ==========================================

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
