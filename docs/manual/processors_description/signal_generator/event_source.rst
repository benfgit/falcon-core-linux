EventSource
===========
Generate an EventData stream by randomly emitting events from a list of candidates at a predefined rate

Output ports
............

:events: *type*: :ref:`EventData` , 1 slot

Options
.......

:events *(list of string)*:
  list of events to emit

  *Default value* : 1 event with the default type : "default_eventsource_event"

:rate *(double)*:
  (approximate) event rate

  *Default value* : 1Hz