EventData processor
===================

The event library is working with event data (TODO: add link to description)

EventFilter
-----------
Process neural data

Input ports
...........

:events *(EventData, 1-256 slots)*:

:blocking_events *(EventData, 1-256 slots)*:

Output port
...........

:events *(EventData, 1 slot)*:

Options
.......

:target_event *(string)*:
  target event to be filtered out

  *Default value* : None

:blockout_time_ms *(double)*:
  time during which target events are filtered out

  *Default value* : 10.0 ms

:synch_time_ms *(double)*:
  time used to check if any blocking target event is present after a target event has been received

  *Default value* : 1.5 ms

:time_in_ms:
  time after blocking event during all arriving event are blocked

  *Default value* : 3.5 ms

:discard_warning *(bool)*:
  if true, warnings about discarded events will not be generated

  *Default value* : False

:detection_criterion *(string OR unsigned int)*:
  string or number to determine the criterion for a triggering detection; acceptable string values: 'any', 'all'
  acceptable integer values: any value between 1 (equivalent to 'any') and the number of input slots

  *Default value* : "any"

EventSource
-----------
Generate an EventData stream by randomly emitting events from a list of candidates at a predefined rate

Output ports
............

:events *(EventData, 1 slot)*:

Options
.......

:events *(list of string)*:
  list of events to emit

  *Default value* : 1 event with the default type : "default_eventsource_event"

:rate *(double)*:
  (approximate) event rate

  *Default value* : 1Hz

EventSink
---------
Takes an EventData stream and logs the arrival of a target event

Input ports
...........

:events *(EventData, 1 slot)*:

Options
.......

:target_event *(string)*:

  *Default value* : none (default  value set in eventdata)

EventSync
---------
Synchronize on the occurrence of a target event on all its input slots, before emitting the same target event

Input ports
...........

:events *(EventData, 1 slot)*:

Output ports
............

:events *(EventData, 1 slot)*:

Options
.......

:target_event *(string)*:

  *Default value* : none (default value set in eventdata)


DigitalOutput
-------------
Take an EventData stream and sets digital outputs according to an event-specific protocol

Input ports
...........

:events *(EventData, 1 slot)*:

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

