.. Falcon documentation master file, created by
   sphinx-quickstart on Fri May  5 15:12:39 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Documentation for Falcon
========================

Falcon is a software for real-time processing of neural signals to enable
short-latency closed-loop feedback in experiments that try to causally link
neural activity to behavior. Example use cases are the detection of hippocampal
ripple oscillations or online decoding and detection of hippocampal replay
patterns.


High level overview
-------------------

At its core, Falcon executes a user-defined data flow graph that consists of
multiple connected nodes (processors) that perform computations on one or more
streams of input data and produce one or more streams of output data. Some
types of processors produce output data without accepting input data
(sources), whereas other types of processors consume input data without
produce output data (sinks). An example data flow graph is shown in the figure
below [TODO].

The data flow graph is specified in YAML text format and defines all the
processor nodes and their interconnections. Falcon ships with a number of
built-in processors that can be used to construct custom
applications (see :ref:`processors`). Falcon can be easily extended with new kinds of processors,
although this requires modern C++ programming skills (see :ref:`extending_falcon`).

By design, Falcon software is only concerned with the execution of data flow
graphs and it does not include a graphical user interface. Rather, separate
client applications interact with a running Falcon instance through network
communication. In this way, dedicated  user interfaces may be built in any
programming language for particular user applications (as determined by the
data flow graph). A generic Python control client is shipped with Falcon (see
:ref:`generic_client`) and serves as an example for how to build a user interface.


Installation
------------

.. toctree::
   :maxdepth: 2
   :glob:

   manual/installation

User guide
----------
.. toctree::
   :maxdepth: 2

   manual/usage
   manual/graphs
   manual/configuration
   manual/troubleshoot
   testing/debug
   testing/test_mode
   testing/test_graphs


Extending Falcon
----------------
.. toctree::
   :maxdepth: 2
   :glob:

   extensions/overview
   extensions/extend_overview
   extensions/extend_processor
   extensions/extend_datatype
   extensions/tools


Building custom clients
-----------------------
.. toctree::
   :maxdepth: 2
   :glob:

   ui/interaction
   ui/create_ui
   ui/lib_python

Developer's guide
-----------------

.. toctree::
   :maxdepth: 2
   :glob:

   internals/overview
   internals/logging_system
   internals/command_system
   internals/config_system
   internals/thread_management
   internals/graph_system
   internals/utilities

Falcon core extension (separate repository)
-------------------------------------------

.. toctree::
   :maxdepth: 2
   :glob:

   core_extension_doc/overview
   core_extension_doc/datatypes
   core_extension_doc/processors
   core_extension_doc/hardware
   core_extension_doc/libs
   core_extension_doc/tools
   core_extension_doc/example
   core_extension_doc/ressource


Real-time decoding extension (separate repository)
--------------------------------------------------

Falcon control client (separate repository)
------------------------------------------

.. toctree::
   :maxdepth: 2
   :glob:

   ui/generic_control

Ripple detection client (separate repository)
---------------------------------------------
.. toctree::
   :maxdepth: 2

   examples/ripple_detection


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

