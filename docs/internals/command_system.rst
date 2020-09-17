Command system
==============

Type of command sources implemented by three classes that derive from CommandSource base class:
 - command line : send command from the internal falcon processing
 - keyboard : send command via keyboard shortcut
 - cloud : send command via zmq network

The namespace "commands" is used to contain the command system logic. Sources are added in the CommandHandler during the
setup part of the main.
Commands are requested (serially) from sources and handled by CommandHandler class in main thread.
Graph commands are forwarded to GraphManager and handled in graph thread.
Replies are sent to the original requester of the command.
