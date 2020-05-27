EventFilter
-----------
Process neural data

Input ports
...........

:events: *type*: :ref:`EventData` , 1 slot

:blocking_events: *type*: :ref:`EventData` , 1-256 slot

Output port
...........

:events: *type*: :ref:`EventData` , 1 slot

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