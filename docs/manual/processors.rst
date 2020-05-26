.. _processors:

Processors
==========

Built-in processors can be found in the falcon-fklab-extensions [TODO / Link]

Each processor can be described with 4 categories :

- Input ports: useful in the connection part of the graph file
- Output ports : useful in the connection part of the graph file
- Options : useful in the option part relative to each processor in the graph file. The processor will set its parameters
  at the initialization time either with the value specify in the graph file or if not, with its default value.
- States :  useful in the States part of the graph file. The concept is states is very important and are developed here [TODO add link].
  Two interesting informations are how it can be connected to another processor and how it can be used by an external user.

.. toctree::
   :maxdepth: 2
   :glob:

   processors_description/detectors
   processors_description/event
   processors_description/multichanneldata
   processors_description/sink




