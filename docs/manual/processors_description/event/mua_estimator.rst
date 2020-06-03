MUAEstimator
============
Compute the Multi-Unit Activity from the spike counts provided by the spike detectors and outputs MUAData.


.. list-table:: **Input port**
   :header-rows: 1

   * - port name
     - data type
     - slots
     - description
   * - **spikes**
     - :ref:`SpikeData`
     - 1-64
     -

.. list-table:: **Output port**
   :header-rows: 1

   * - port name
     - data type
     - slots
     - description
   * - **mua**
     - :ref:`MUAData`
     - 1
     -

.. list-table:: **Options**
   :header-rows: 1

   * - port name
     - data type
     - default
     - description
   * - **bin_size_ms**
     - unsigned int
     - 20
     - target event to be filtered out


.. list-table:: **Readable states**
   :header-rows: 1

   * - name
     - data type
     - initial value
     - external access
     - peers access
     - description
   * - **bin_size_ms**
     - unsigned int
     - options:bin_size_md
     - read-only
     - read/write
     -

.. list-table:: **Writable states**
   :header-rows: 1

   * - name
     - data type
     - initial value
     - external access
     - peers access
     - description
   * - **MUA**
     - double
     - 0.0
     - read-only
     - read-only
     -