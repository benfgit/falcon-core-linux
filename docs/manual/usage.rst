Usage
=====

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


Keyboard commands
-----------------

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
