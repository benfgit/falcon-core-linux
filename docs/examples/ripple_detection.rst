Hippocampal ripple detection
============================

Overview
--------


Implementation
--------------

processors :
-----------

- source : :ref:`NlxReader`
- ripple_filter_1 : :ref:`MultiChannelFilter`
- ripple_filter_2 : :ref:`MultiChannelFilter`
- HIPPOCAMPUS_detector: :ref:`RippleDetector`
- CORTEX_detector: :ref:`RippleDetector`
- eventfilter : :ref:`EventFilter`
    * blocking event : Event from the cortex
    * input event : Event from the hippocampus
- networksink : :ref:`ZMQSerializer`
- eventsink : :ref:`EventLogger`
- datasink_ev : :ref:`FileSerializer`
- datasink_ripple_stats : :ref:`FileSerializer`
- ttl_output: SerialOutput (not yet ported on the new falcon)

Options
-------

States writable by the user
---------------------------

Processor :ref:`RippleDetector`:

   - threshold_dev (double)
   - smooth_time (double): integration time for signal statistics. Must be a positive number.
   - detection_lockout_time_ms (double): Current refractory period following threshold crossing that is not
       considered for  updating signal statistics and for event detection.
   - stream_events (bool)
   - stream_statistics (bool)

.. note:: As their is two RippleDetectors, these states are available for each processor.

Output
------

There is two output data serialized in a yaml file:
- ripple events : :ref:`EventData`
- ripple statistics

See the full  :ref:`ripple graph` definition.