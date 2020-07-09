Interaction with Falcon
=======================

Falcon operation and data flow graph execution can be controlled using
keyboard commands (see Usage), but this does not expose the full set of
features. Another way to interact with a running Falcon instance is through
a network connection.

Send commands to Falcon
-----------------------

there is set of command which can be send to Falcon

- to manage the general app :

============= ===============================================================================
command       action
============= ===============================================================================
quit          quit the app
kill          stop processing and quit (kill)
info          generate basic information answer as falcon version and state
documentation generate yaml documentation with a node for each processor registered in Falcon
============= ===============================================================================

- to manage the graph : (node the graph prefix for each command)

========================== =============================================================================
command                    action
========================== =============================================================================
graph build                build the graph
graph documentation        generate yaml documentation with a node for each processor in the build graph
graph run                  start processing graph (run)
graph test                 start processing in test mode
graph stop                 stop processing graph
graph yaml                 display the current graph in yaml format
graph apply [yaml Node]    apply a exposed method in the processor
graph retrieve [yaml Node] retrieve a particular processor state/definition
graph update [yaml Node]   update a particular processor state/definition
========================== =============================================================================

Receive log messages
--------------------



