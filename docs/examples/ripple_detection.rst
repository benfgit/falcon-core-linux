Hippocampal ripple detection
============================

Overview
--------


Implementation
--------------

processors
----------

- source : :ref:`NlxReader`
- ripple filter 1 : :ref:`MultiChannelFilter`
- ripple filter 2 : :ref:`MultiChannelFilter`
- HIPPOCAMPUS detector: :ref:`RippleDetector`
- CORTEX detector: :ref:`RippleDetector`
- event filter : :ref:`EventFilter`
    * blocking event : Event from the cortex
    * input event : Event from the hippocampus
- network sink : :ref:`ZMQSerializer`
- event sink : :ref:`EventLogger`
- datasink ev : :ref:`FileSerializer`
- datasink ripple_stats : :ref:`FileSerializer`
- ttl output: SerialOutput (not yet ported on the new falcon)


States writable by the user
---------------------------

Processor :ref:`RippleDetector`:

   - threshold dev (double)
   - smooth time (double): integration time for signal statistics. Must be a positive number.
   - detection lockout time (double): Current refractory period following threshold crossing that is not
       considered for  updating signal statistics and for event detection.
   - stream events (bool)
   - stream statistics (bool)


.. note:: As their is two RippleDetectors, these states are available for each processor.

Output
------

- ripple events : :ref:`EventData`
- ripple statistics


.. include:: graph.rst
