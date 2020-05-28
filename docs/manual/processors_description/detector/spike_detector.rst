SpikeDetector
=============

Detect spikes on any of the incoming MultiChannelData stream; sends SpikeData on the output port spikes and an event
"spike"/"spikes" on the output port events if one or more spikes have been detected in the received buffer

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

.. list-table:: **Output port**
   :header-rows: 1

   * - port name
     - data type
     - slots
     - description
   * - **events**
     - :ref:`EventData`
     - 1
     - A stream of events.
   * - **spikes**
     - :ref:`SpikeData`
     - 1
     - A stream of detected spikes


.. list-table:: **Options**
   :header-rows: 1

   * - port name
     - data type
     - default
     - description
   * - **threshold**
     - double
     - 60.0
     - threshold that a single channel must cross.
   * - **invert_signal**
     - bool
     - True
     - whether the signal does (true) or does not (false) need to be inverted when detecting spikes
   * - **buffer_size**
     - double
     - 0.5 ms
     - amount of data that will be used to look for spikes [ms]
   * - **peak_lifetime**
     - unsigned int
     - 8 samples
     - number of samples that will be used to look for a peak
   * - **strict_time_bin_check**
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
     - option: threshold
     - read/write
     - read-only
     -
   * - **peak_lifetime**
     - unsigned int
     - option:peak_lifetime
     - read/write
     - read-only
     -
