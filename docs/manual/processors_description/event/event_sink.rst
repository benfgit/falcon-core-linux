EventSink
=========
Takes an EventData stream and logs the arrival of a target event


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

   * - port name
     - data type
     - default
     - description
   * - **target_event**
     - string
     - none (default  value set in :ref:`EventData`)
     -

