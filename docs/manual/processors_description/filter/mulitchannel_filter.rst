MultiChannelFilter
==================

Input ports
...........

:data: *type*: :ref:`MultiChannelData` <double>, 1-256 slot

Output port
...........

:data: *type*: :ref:`MultiChannelData` <double>, 1-256 slot

Options
.......

:filter *(string OR definition structure)*:
  YAML filter definition or name of file that contains filter

  *Default value*: No default value - the definition of this parameter in the graph file is mandatory.

  Here are some example filter configurations:

.. code-block:: yaml

    filter:
        file: filters://butter_lpf_0.1fs.filter

    filter:
       type: biquad
       gain: 3.405376527201278e-04
       coefficients:
          - [1.0, 2.0, 1.0, 1.0, -1.032069405319709, 0.275707942472944]
          - [1.0, 2.0, 1.0, 1.0, -1.142980502539903, 0.412801598096190]
          - [1.0, 2.0, 1.0, 1.0, -1.404384890471583, 0.735915191196473]
       description: 6th order butterworth low pass filter with cutoff at 0.1 times the sampling frequency

    filter:
       type: fir
       description: 101 taps low pass filter with cutoff at 0.1 times the sampling frequency
       coefficients: [-6.24626469088e-19, -0.000309386982441, -0.000528204854007, ...]

----------

NlxReader
---------
Read raw data from a Neuralynx Digilynx data acquisition system and turns it into multiple MultiChannelData output streams
based on a channel mapping

Output port
...........

:(configurable) data_ports:  *type*: :ref:`MultiChannelData` <double>, 1 slot

  create an output port for each channel in the channelmap

The channelmap defines the output port names and for each port lists the AD channels that will be copied to the
MultiChannelData buckets on that port. The channelmap option should be specified as follows:

.. code-block:: yaml

    channelmap:
      portnameA: [0,1,2,3,4]
      portnameB: [5,6]
      portnameC: [0,5]

Options
.......

:address *(string)*:
  IP address of Digilynx system

  *Default value*: "127.0.0.1"

:port  *(unsigned int)*:
  port of Digilynx system

  *Default value*: 5000

:channelmap *(map of vector<int>)*:
  mapping between AD channels and output ports

  *Default value*: No default value

:npackets  *(uint64_t)*:
  number of raw data packets to read before exiting (0 = continuous streaming)

  *Default value*: 0

:nchannels  *(unsigned int)*:
  number of channels in Digilynx system batch_size - how many samples to pack into single MultiChannelData bucket

  *Default value*: 128

:update_interval  *(unsigned int)*:
  time interval (in seconds) between log updates

  *Default value*: 20

:hardware_trigger  *(bool)*:
  enable use of hardware triggered dispatching

  *Default value*: False

:hardware_trigger_channel  *(uint8)*:
  which DIO channel to use as trigger

  *Default value*: 0