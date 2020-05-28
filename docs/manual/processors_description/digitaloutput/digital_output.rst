DigitalOutput
=============
Take an EventData stream and sets digital outputs according to an event-specific protocol

.. list-table:: **Input ports**
   :header-rows: 1

   * - port name
     - data type
     - slots
     - description
   * - **events**
     - :ref:`EventData`
     - 1
     -
.. list-table:: **Options**
   :header-rows: 1

   * - name
     - data type
     - default
     - description
   * - **enabled**
     - bool
     - enabled state
     - enable/disable digital output
   * - **lockout_period_ms**
     - unsigned int
     - 300 ms
     - set the maximal stimulation frequency. If equal to zero, it disabled the feature.
   * - **pulse_width**
     - unsigned int
     - 300 ms
     - duration of digital output pulse in microseconds
   * - **enable_saving**
     - bool
     - enabled state
     - enable/disable saving stimulation events
   * - **device**
     - unsigned int
     - no default type value - Always specify in the graph config file.
     - map specifying the digital output device. A required "type" key indicates which device should be used.
       Valid values are "dummy". The dummy device requires an additional "nchannels" key.
   * - **protocols**
     -
     -
     - maps events to digital output protocols.


The protocols option specifies a map with  for each target event a map of actions for selected digital output channels.
Note that each channel can only be associated with a single action (even if it is listed more than once).
There are 4 possible actions: high, low, toggle and pulse. Events that are not in the protocols map are ignored.
Example configuration for protocols option:

.. code-block::

      protocols:
        event_a:
          high: [0,1]
        event_b:
          low: [0]
          toggle: [1]
        event_c:
          pulse: [2]


.. list-table:: **States**
   :header-rows: 1

   * - name
     - data type
     - initial value
     - external access
     - peers access
     - description
   * - **enabled**
     - bool
     - option:enabled
     - write/read
     - read-only
     -
   * - **lockout_period_ms**
     - bool
     - option:lockout_period_ms
     - write/read
     - read-only
     -

