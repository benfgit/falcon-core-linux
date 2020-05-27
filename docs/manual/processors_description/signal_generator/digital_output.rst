DigitalOutput
=============
Take an EventData stream and sets digital outputs according to an event-specific protocol

Input ports
...........

:events: *type*: :ref:`EventData` , 1 slot

Options
.......

:enabled *(bool)*:
  enable/disable digital output

  *Default value* : enabled state

:lockout_period_ms *(bool)*:
  enable/disable digital output

  *Default value* : 300 ms

:pulse_width *(unsigned int)*:
  duration of digital output pulse in microseconds

  *Default value* : 400 microsec

:enable_saving *(bool)*:
  enable/disable saving stimulation events

  *Default value* : enabled state

:device:
  map specifying the digital output device. A required "type" key indicates which device should be used.
  Valid values are "dummy". The dummy device requires an additional "nchannels" key.

  *Default values* : no default type value - Always specify in the graph config file.

:protocols:
  maps events to digital output protocols.
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

States
......

:enabled *(bool)*:
  enable/disable digital output

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User* : write/read

:lockout_period_ms *(bool)*:
  enable/disable digital output

  *Initial value* : see options value

  *Processor connection* (state type, peers permission): readable, read-only

  *User* : write/read