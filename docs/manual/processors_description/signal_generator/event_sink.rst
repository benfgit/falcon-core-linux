
EventSink
=========
Takes an EventData stream and logs the arrival of a target event

Input ports
...........

:events: *type*: :ref:`EventData` , 1 slot

Options
.......

:target_event *(string)*:

  *Default value* : none (default  value set in eventdata)

EventSync
---------
Synchronize on the occurrence of a target event on all its input slots, before emitting the same target event

Input ports
...........

:events: *type*: :ref:`EventData` , 1-256 slot

Output ports
............

:events: *type*: :ref:`EventData` , 1 slot

Options
.......

:target_event *(string)*:

  *Default value* : none (default value set in eventdata)

