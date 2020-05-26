.. _usage:

Usage
=====

Launch Falcon
-------------

.. code-block:: bash

   falcon graph.yaml

Command-line options
--------------------

.. code-block:: bash

   usage: falcon [options] ... [graph file] ...
   options:
     -c, --config         configuration file (string [=$HOME/.falcon/config.yaml])
     -a, --autostart      auto start processing (needs graph)
     -d, --debug          show debug messages
         --noscreenlog    disable logging to screen
         --nocloudlog     disable logging to cloud
     -t, --test           turn testing on by default
     -?, --help           print this message

A configuration file can be used to specify automatically this options + others used to affine the control in the whole system.
Check out the `next section <configuration.rst>`_.

.. note:: The option specified in command line are prioritary on the options specified in the config file.

Control commands with a Falcon running graph
--------------------------------------------

Keyboard commands
.................

=== ===============================
key action
=== ===============================
q   quit
i   info
r   start processing graph (run)
t   start processing in test mode
s   stop processing graph
k   stop processing and quit (kill)
=== ===============================

Falcon-client gui
.................

The graph is also controllable from gui in remote or not. See this section for more details : `falcon-client <../ui/generic_control.rst>`_


