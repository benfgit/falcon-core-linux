NlxReader
=========

Read raw data from a Neuralynx Digilynx data acquisition system and turns it into multiple MultiChannelData output streams
based on a channel mapping

.. list-table:: **Output ports**
   :header-rows: 1

   * - port name
     - data type
     - slots
     - description
   * - **(configurable) data_ports**
     - :ref:`MultiChannelData` <double>
     - 1
     - create an output port for each channel in the channelmap

The channelmap defines the output port names and for each port lists the AD channels that will be copied to the
MultiChannelData buckets on that port. The channelmap option should be specified as follows:

.. code-block:: yaml

    channelmap:
      portnameA: [0,1,2,3,4]
      portnameB: [5,6]
      portnameC: [0,5]

.. tabularcolumns:: |p{4cm}|p{3cm}|p{2.5cm}|p{5.5cm}|

.. list-table:: **Options**
   :header-rows: 1

   * - name
     - data type
     - default
     - description
   * - **address**
     - string
     - "127.0.0.1"
     - IP address of Digilynx system
   * - **port**
     - unsigned int
     - 5000
     - port of Digilynx system
   * - **channelmap**
     - map of vector<int>
     - No default value
     - mapping between AD channels and output ports
   * - **npackets**
     - uint64_t
     - 0
     - number of raw data packets to read before exiting (0 = continuous streaming)
   * - **nchannels**
     - unsigned int
     - 128
     - number of channels in Digilynx system batch_size - how many samples to pack into single MultiChannelData bucket
   * - **update_interval**
     - unsigned int
     - 20
     - time interval (in seconds) between log updates
   * - **hardware_trigger**
     - bool
     - False
     - enable use of hardware triggered dispatching
   * - **hardware_trigger_channel**
     - uint8
     - 0
     - which DIO channel to use as trigger