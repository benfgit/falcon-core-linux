RippleDetector
==============

Detect ripples in a MultiChannelData stream and emits an ripple event in response

.. list-table:: **Input port**
   :header-rows: 1

   * - port name
     - data type
     - slots
     - description
   * - **data**
     - :ref:`MultiChannelData` <double>
     - 1
     -

.. list-table:: **Input port**
   :header-rows: 1

   * - port name
     - data type
     - slots
     - description
   * - **events**
     - :ref:`EventData`
     - 1
     - A stream of 'ripple' events.
   * - **statistics**
     - :ref:`MultiChannelData` <double>
     - 1
     -
.. list-table:: **Options**
   :header-rows: 1

   * - port name
     - data type
     - default
     - description
   * - **threshold_dev**
     - double
     - 6.0
     - threshold that needs to be crossed
   * - **smooth_time**
     - double
     - 10.0 ms
     - integration time for signal statistics. Must be a positive number.
   * - **detection_lockout_time_ms**
     - double
     - 30 ms
     - refractory period after threshold crossing detection that is not considered for updating of statistics
       and for detecting events. Must greater than 0 ms.
   * - **statistics_buffer_size**
     - double
     - 0.5 sec
     - Buffer size (in seconds) for statistics output buffers. Should be equal larger than zero.
   * - **statistics_downsample_factor**
     - unsigned int
     - 1
     - downsample factor of streamed statistics signal. Should larger than zero..
   * - **stream_events**
     - bool
     - True
     - enable/disable ripple event output
   * - **stream_statistics**
     - bool
     - True
     - enable/disable streaming of ripple detection statistics
   * - **use_power**
     - bool
     - True
     -


.. list-table:: **States**
   :header-rows: 1

   * - name
     - data type
     - initial value
     - external access
     - peers access
     - description
   * - **threshold**
     - double
     - 0.0
     - None
     - read-only
     - Current threshold that needs to be crossed
   * - **mean**
     - double
     - 0.0
     - None
     - read-only
     - Current signal mean. Units: same as input signal.
   * - **deviation**
     - double
     - 0.0
     - None
     - read-only
     - Current signal deviation. Units: same as input signal.
   * - **ripple**
     - bool
     - False
     - read-only
     - read-only
     -
   * - **threshold_dev**
     - double
     - option:threshold
     - read-only
     - read/write
     -
   * - **smooth_time**
     - double
     - option:smooth_time
     - read-only
     - read/write
     - integration time for signal statistics. Must be a positive number.
   * - **detection_lockout_time_ms**
     - double
     - option: detection_lockout_time_ms
     - read-only
     - read/write
     - Current refractory period following threshold crossing that is not
       considered for  updating signal statistics and for event detection.
   * - **stream_events**
     - bool
     - option:stream_events
     - read-only
     - read/write
     -
   * - **stream_statistics**
     - bool
     - option:stream_statistics
     - read-only
     - read/write
     -